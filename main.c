// Add M3 timer - check
// Add real sensor data - calibration requried
// power optimization
//data communication with parity
//average the data before send
// EEPROM
//security



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "timex.h"
#include "thread.h"
#include "mutex.h"
#include "ztimer.h"

#include "shell.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#ifndef EMCUTE_ID
#define EMCUTE_ID ("gertrud")
#endif
#define EMCUTE_PORT (1883U)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

/* Add lps331ap related include here */
#include "lpsxxx.h"
#include "lpsxxx_params.h"


/* Add isl29020 related include here */
#include "isl29020.h"
#include "isl29020_params.h"
#include "isl29020-internal.h"

/* Declare the lps331ap device variable here */
static lpsxxx_t lpsxxx;
static isl29020_t dev;

static int16_t temp = 0;
static uint16_t pres =0;
static int lux = 0;

// struct that contains sensors
typedef struct sensors{
  int16_t temperature;
  uint16_t pressure;
  int lightLevel;
}t_sensors;


static char stack[THREAD_STACKSIZE_DEFAULT];
/* Add your thread stack and handler function here */
static char lps331ap_stack_T[THREAD_STACKSIZE_MAIN];
/* Add your thread stack and handler function here */
static char lps331ap_stack_P[THREAD_STACKSIZE_MAIN];
/* Add your thread stack and handler function here */
static char isl29020_stack[THREAD_STACKSIZE_MAIN];


static msg_t queue[8];



static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; /* should never be reached */
}

// function that disconnects from the mqttsn gateway
static int discon(void){
    int res = emcute_discon();
    if (res == EMCUTE_NOGW) {
        puts("error: not connected to any broker");
        return 1;
    }
    else if (res != EMCUTE_OK) {
        puts("error: unable to disconnect");
        return 1;
    }
    puts("Disconnect successful");
    return 0;
}


/* Add your lps331ap Temperature thread  here */
static void *thread_handler_lps331ap_T(void *arg){
    
    (void)arg;
    
    while (1){
        
        lpsxxx_read_temp(&lpsxxx,&temp);
        ztimer_sleep(ZTIMER_MSEC, 3000);
        }
               
    return NULL;

}


/* Add your lps331ap Pressure thread  here */
static void *thread_handler_lps331ap_P(void *arg){
    
    (void)arg;
    
    while (1){
        
        lpsxxx_read_pres(&lpsxxx,&pres);
        ztimer_sleep(ZTIMER_MSEC, 5000);
        }
               
    return NULL;

}


/* Add your isl29020 Pressure thread  here */
static void *thread_handler_isl29020(void *arg){
    
    (void)arg;
    
    while (1) {
        
       lux =  isl29020_read(&dev);
       ztimer_sleep(ZTIMER_MSEC, 500);
       }
               
    return NULL;

}

// function that publish messages to the topic
// it takes as input the topic, the message to send and
// the value of qos
static int pub(char* topic, char* data, int qos){
  emcute_topic_t t;
  unsigned flags = EMCUTE_QOS_0;

  switch (qos) {
      case 1:
        flags |= EMCUTE_QOS_1;
        break;
      case 2:
        flags |= EMCUTE_QOS_2;
        break;
      default:
        flags |= EMCUTE_QOS_0;
        break;
  }



  /* step 1: get topic id */
  t.name = topic;
  if (emcute_reg(&t) != EMCUTE_OK) {
      puts("error: unable to obtain topic ID");
      return 1;
  }

  /* step 2: publish data */
  if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
      printf("error: unable to publish data to topic '%s [%i]'\n",
              t.name, (int)t.id);
      return 1;
  }

  printf("published %s on topic %s\n", data, topic);

  return 0;
}

// function that connects to the mqtt gateway
// it takes as input the ip address and the port
static int con(char* addr, int port){
  sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
  gw.port = port;

  /* parse address */
  if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, addr) == NULL) {
      printf("error parsing IPv6 address\n");
      return 1;
  }

  if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
      printf("error: unable to connect to [%s]:%i\n", addr, port);
      return 1;
  }
  printf("Successfully connected to gateway at [%s]:%i\n", addr, port);
  return 0;
}



      
  // function that generate sensor values
static void sensors_values(t_sensors* sensors){
  sensors->temperature = temp/100;
  sensors->pressure = pres;
  sensors->lightLevel = lux;
}    
      




// new shell command: start the station
// the function takes in input ip address and port of the gateway,
// and the id of the specified station
// every five seconds it generates new sensor values and publish them to 
// sensor/station + station id
static int cmd_start(int argc, char **argv){
  if (argc < 4) {
      printf("usage: %s <address> <port> <id>\n", argv[0]);
      return 1;
  }

  t_sensors sensors;  
  // name of the topic
  char topic[32];
  sprintf(topic,"sensor/station1");
  
  // json that it will published
  char json[256];
  
  while(1){
    // it tries to connect to the gateway
    if (con(argv[1], atoi(argv[2]))) {
      continue;
    }
          
      
   char datetime[20];  
      
   
        // Get the current time as a time_t value
    time_t currentTime = time(NULL);

    // Convert the time_t value to a struct tm in the local timezone
    struct tm *timeinfo = localtime(&currentTime);

    if (timeinfo == NULL) {
        perror("localtime");
        return 1;
    }

    // Display the current date and time
    //printf("Current date and time: %s", asctime(timeinfo));

    // Make a copy of the struct tm before modifying it
    struct tm modifiedTimeinfo = *timeinfo;

    // Modify some fields of the copied struct tm
    modifiedTimeinfo.tm_year += 2023-1970;  // Add 2 hours
    modifiedTimeinfo.tm_mon += 11;  // Add 30 minutes
    modifiedTimeinfo.tm_mday += 14; 
    modifiedTimeinfo.tm_hour +=12 ;   
    modifiedTimeinfo.tm_min += 30 ;
    // Display the modified date and time directly
    //printf("Modified date and time: %s", asctime(&modifiedTimeinfo));
      
      
    int c = strftime(datetime, sizeof(datetime), "%Y-%m-%d %T",&modifiedTimeinfo);  
    
     
     if(c == 0) {
      printf("Error! Invalid format\n");
      return 0;
    }     
     
      
      
      
      
    sensors_values(&sensors); 
      
    char stationID [20];  
    sprintf(stationID,"%d",atoi(argv[3]));  

    // fills the json document
    sprintf(json, "{\"id\": \"%s\", \"datetime\": \"%s\", \"temperature\": "
                  "\"%d\", \"pressure\": \"%d\", \"lightLevel\": \"%d\"}",
                  stationID, datetime, sensors.temperature, sensors.pressure,sensors.lightLevel);
      
    // publish to the topic
    pub(topic, json, 0);

    // it disconnects from the gateway
    discon();

    // it sleeps for five seconds
    ztimer_sleep(ZTIMER_MSEC,1000);
  }

  return 0;
}

static const shell_command_t shell_commands[] = {
    {"start", "Start the station", cmd_start},
    {NULL, NULL, NULL}};

int main(void)
{
    
    /* Initialize the lps331ap sensor here */
    lpsxxx_init(&lpsxxx ,&lpsxxx_params[0]);
    
    /* Initialize the isl29020 sensor here */
    isl29020_init(&dev, &isl29020_params[0]);
    
    
    puts("MQTT-SN application\n");
    puts("Type 'help' to get started. Have a look at the README.md for more "
         "information.");

    /* the main thread needs a msg queue to be able to run `ping6`*/
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");
    
        /* Create the thread here */
    thread_create(lps331ap_stack_T,sizeof(lps331ap_stack_T),THREAD_PRIORITY_MAIN - 1,0,thread_handler_lps331ap_T,NULL,"lps331ap_thread_Temperature");
    /* Create the thread here */
    thread_create(lps331ap_stack_P,sizeof(lps331ap_stack_P),THREAD_PRIORITY_MAIN - 2,0, thread_handler_lps331ap_P,NULL,"lps331ap_thread_Pressure");
    /* Create the thread here */
    thread_create(isl29020_stack,sizeof(isl29020_stack),THREAD_PRIORITY_MAIN - 3,0,thread_handler_isl29020,NULL,"isl29020_LUX");
    
   
        
     //sensors_values(&sensors);     
     //printf("Temperature: %i.%u°C\n",(sensors.temperature/100),(sensors.temperature % 100)); // check this one with sprintf
     //printf("Pressure: %uhPa/n",sensors.pressure);  
     //printf("Light value: %5i LUX\n", sensors.lightLevel);   
    

    /* start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
