# :octocat: IoT-Mini-Project-1
---

### Group Members

* Shalika Dulaj
* Ana Ferreira
* Kavinda Rathnayaka


## Introduction

This project was created as part of the first mini-project assignment for the Internet of Things course at the University of Oulu. The primary goal of 
this assignment is to establish an IoT pipeline for retrieving temperature, atmospheric pressure, and lux level data from a low-end device in a remote location, and then storing, processing, and visualizing in the cloud platform.

## Tools and Requirement

- we have used [FIT IOT-LAB](https://www.iot-lab.info/) to use real sensors and microcontrollers to retrieve data and forward them to a cloud platform. FIT IOT-LAB platform provides numerous boards,  open-source libraries, packages, and drivers to experiment with various IoT projects.
  
- [RIOT](https://doc.riot-os.org/index.html) - RIOT is an open-source microkernel-based operating system, designed to match the requirements of Internet of Things (IoT) devices and other embedded devices. we will use RIOT to build firmware for our low-end devices ( sensors & border router).

- For the data  management and visualization we have  used the [AWS-IOT](https://aws.amazon.com/iot-core/?c=i&sec=srv) free credit version. with the free credit version, you will get
  Amazon SNS, Amazon Grafana, and DynamoDB tools to play around with your data.

the following image will give you an overview of this implementation. 

<div align="center">


![Screenshot 2023-12-14 at 15 10 48](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/153508129/e2855b5c-4e2a-486e-92f7-92ac7335c862)

</div>



This project is divided into three main layers. the main objective is to achieve the maximum efficiency, power consumption, and reliability of each layer.

* Sensing
* Network Layer
* Data Management 

## Sensing Layer

In this implementation, we utilized the FIT [IOT-LAB M3](https://www.iot-lab.info/docs/boards/iot-lab-m3/) microcontroller for capturing temperature, atmospheric pressure, and LUX level data. (IOT-LAB node m3-1)

The FIT IOT-LAB M3 is built on an STM32 (ARM Cortex M3) microcontroller, featuring an ATMEL radio interface operating at 2.4 GHz, and equipped with four sensors.

### * Sensors
Four sensors connected to the MCU via the I2C bus are embedded into the IoT-LAB M3 board:

- the light sensor: this measures ambient light intensity in lux.  [ISL29020](https://www.iot-lab.info/assets/misc/docs/iot-lab-m3/ISL29020.pdf)
- the pressure and temperature sensor: this measures atmospheric pressure in hPa.  [LPS331AP](https://www.iot-lab.info/assets/misc/docs/iot-lab-m3/LPS331AP.pdf)
- the accelerometer/magnetometer: this provides feedback on an object’s acceleration, and can be used to detect movement. By determining a threshold, 
  it generates a state change on one of the MCU’s digital inputs/outputs to create an interrupt, which can be used to bring the MCU out of 
  standby mode.  [LSM303DLHC](https://www.iot-lab.info/assets/misc/docs/iot-lab-m3/LSM303DLHC.pdf)
- the gyroscope: this measures the orientation of an object in space and can be used, for example, to determine the orientation of the screen of a 
  tablet or a smartphone.  [L3G4200D](https://www.iot-lab.info/assets/misc/docs/iot-lab-m3/L3G4200D.pdf)

Given our emphasis on weather data measurement, our implementation exclusively incorporates the ISL29020 sensor for reading lux levels and the LPS331AP sensor for capturing temperature and atmospheric pressure.

To make the sensor layer more efficient we are considering implementing/will implement the following

* Implementing multi-threading to periodically read sensor data.
* Utilizing a sleep mode during idle periods to conserve power.
* I2C low-power bus for efficient retrieval of embedded sensor data.
* Implementing the functionality to save data in FLASH memory (yet to be implemented).
  

## Network Layer 

### * Border Router

A broader router is used to propagate public IPv6 addresses across the local network, facilitating the connection between the FIT IOT-LAB M3 sensor node and AWS services. In the assignment, we have used the [Border-router](https://www.iot-lab.info/learn/tutorials/riot/riot-public-ipv6-m3/) example provided by the FIT IOT-LAB. In this project, we are using a separate FIT IOT-LAB-M3 board for the border router setup. ( IOT-LAB node m3-2)

![Screenshot 2023-12-13 at 17 15 30](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/153508129/ccf00e39-a42f-46b4-902c-ee3c116c1a1b)

FIT IOT-LAB provides a pool of IPV6/64 addresses per site that can be used to build an IPV6 network. [Read More](https://www.iot-lab.info/docs/getting-started/ipv6/)

![Screenshot 2023-12-13 at 17 17 34](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/153508129/b9aaf43d-ca39-4287-b6b7-cdf43b4b8663)

### * MQTT

Since we are using AWS-IOT core to store and visualize the data, we decided to use the MQTT/MQTT-SN protocol. 
MQTT stands for "Message Queuing Telemetry Transport" and was built by IBM. it is a lightweight and widely adopted messaging protocol specially designed for low-power IoT devices. MQTT is useful when streaming data or event-based data. the main advantage of this protocol is that it consumes very little power when sending payloads.

##### -MQTTSN Architecture

MQTT has a publisher-subscriber model and supports many-to-many communication. The sender and receiver of the messages are decoupled. There are two basic components in this architecture.

* Client 

* Broker

Clients can publish messages on a topic to the broker. The subscribing client can fetch messages from that topic through the broker. Thus the broker decouples the publisher and the subscriber. The broker is responsible for receiving all messages, filtering the messages, determining who is subscribed to each message, and sending the message to these subscribed clients.


<div align="center">



![Screenshot 2023-12-13 at 17 18 16](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/153508129/3bbdb3a3-970f-46b9-8829-fb951fec3994)

</div>	

##### * RSMB - Really Small Message Broker

RSMB is a server  Implementation of the MQTT and MQTT-SN protocols. Any client that implements this protocol properly can use this server for sending and receiving messages. we used RSMB to establish the connection from the sensor node to the MQTTSN client through the border router which sends data to the AWS-IOT. [Read More](https://eclipse.dev/paho/index.php?page=components/mqtt-sn-transparent-gateway/index.php)

local RSMB broker will run on a separate [IOT-LAB A8-M3](https://www.iot-lab.info/docs/boards/iot-lab-a8-m3/) node and the Sensor node publishes topics to port 1885. The IoT-LAB A8-M3 board is based on a TI SITARA AM3505 which is a high-performance ARM Cortex-A8 microprocessor. Its 600 MHz clock speed enables it to run an embedded Linux. RSMB needs to run as a service hence we used A8 board to serve the purpose. 

## Getting Started 

<details>

<summary> STEP 1. Setup RIOT OS environment </summary>

#### logged in to FIT IOT-LAB and ssh to the Grenoble site.

> Note: we recommend that you use Grenoble for this experiment as we are using A8-M3 nodes and there are plenty of boards available.
  
Connect to the SSH frontend of the Grenoble site of FIT/IoT-LAB by using the username you created when you registered with the testbed:

```ruby
   ssh <username>@grenoble.iot-lab.info
```
#### Clone the RIOT OS repository from GitHub:

```ruby
   username@grenoble:~$ git clone https://github.com/RIOT-OS/RIOT.git
```

</details>
<details>

<summary> STEP 2. Download Sensor Node Firmware </summary>

#### Clone the Sensor Node repository from GitHub:

```ruby

    username@grenoble:~$ cd RIOT/examples
    username@grenoble:~/RIOT/examples$ git clone https://github.com/shalikadulaj/IoT-Mini-Project-1.git
```

> Note: you will also put the folder everywhere you want, but you must be careful to edit the Makefile correctly by entering the correct path to the RIOT folder. Therefore modify this line in the Makefile:

```ruby
   RIOTBASE ?= $(CURDIR)/../..
```
</details>

<details>

<summary> STEP 3. Submit an experiment in FIT IOT-LAB Grenoble site using CLI tools </summary>

You can find more about CLI tools [here](https://www.iot-lab.info/legacy/tutorials/iotlab-experimenttools-client/index.html)

In the same terminal and execute the following:

```ruby
   username@grenoble:~$ iotlab-auth -u <username>
   username@grenoble:~$ iotlab-experiment submit -n riot_mqtt -d 180 -l 2,archi=m3:at86rf231+site=grenoble -l 1,archi=a8:at86rf231+site=grenoble
   username@grenoble:~$ iotlab-experiment wait
```

</details>

<details>

<summary> STEP 4. Build the gnrc_border_router firmware  </summary>

Build the gnrc_border_router firmware with the appropriate baud rate for M3 nodes, which is 500,000:

The border firmware is built using the RIOT [gnrc_border_router example](https://www.iot-lab.info/learn/tutorials/riot/riot-public-ipv6-m3/).

```ruby
 username@grenoble:~$ cd RIOT
 username@grenoble:~/RIOT/$ source /opt/riot.source
 username@grenoble:~/RIOT/$ make ETHOS_BAUDRATE=500000 DEFAULT_CHANNEL=11 BOARD=iotlab-m3 -C examples/gnrc_border_router clean all
```
Use the CLI-Tools to flash the gnrc_border_router firmware that you have just built on the first M3 node. Here we use m3-1 but it may change in your case:
```ruby
 username@grenoble:~/RIOT/$ iotlab-node --flash examples/gnrc_border_router/bin/iotlab-m3/gnrc_border_router.elf -l grenoble,m3,1
```
Now you can configure the network of the border router on m3-1 and propagate an IPv6 prefix with ethos_uhcpd.py

```ruby
username@grenoble:~/RIOT/$ cd
username@grenoble:~$ sudo ethos_uhcpd.py m3-1 tap0 2001:660:5307:3100::1/64
```
The network is finally configured and you will see similar response like below:

```ruby
net.ipv6.conf.tap0.forwarding = 1
net.ipv6.conf.tap0.accept_ra = 0
----> ethos: sending hello.
----> ethos: activating serial pass through.
----> ethos: hello reply received
```
> Note 1: leave the terminal open (you don’t want to kill ethos_uhcpd.py, it bridges the BR to the front-end network)

> Note 2: If you have an error “Invalid prefix – Network overlapping with routes”, it’s because another experiment is using the same ipv6 prefix
> (e.g. 2001:660:5307:3100::1/64).

</details>

<details>

<summary>STEP 5. Build the Sensor_Read firmware </summary>

Now, in another terminal, SSH to the SSH frontend and build the required firmware for the other node.

```ruby
   username@grenoble:~$ cd RIOT
   username@grenoble:~/RIOT/$ source /opt/riot.source
   username@grenoble:~/RIOT/$ make ETHOS_BAUDRATE=500000 DEFAULT_CHANNEL=11 BOARD=iotlab-m3 -C examples/IoT-Mini-Project-1 clean all
```
Use the CLI-Tools to flash the Sensor_Read firmware that you have just built on the first M3 node. Here we use m3-2 but it may change in your case:
```ruby
   username@grenoble:~/RIOT/$ iotlab-node --flash examples/IoT-Mini-Project-1/bin/iotlab-m3/Sensing_Layer.elf -l grenoble,m3,2

```
Open the Sensor Node m3-2 shell  and check the Global IPV6 prefix is obtained from the border router subnet using help -> ifconfig

In this case it is - inet6 addr: 2001:660:5307:3100:b8d0:5b74:8db9:353f  scope: global  VAL

```ruby
 username@grenoble:~/RIOT$ nc m3-2 20000
> help
Command              Description
---------------------------------------
start                Start the station
6ctx                 6LoWPAN context configuration tool
ifconfig             Configure network interfaces
nib                  Configure neighbor information base
ping                 Ping via ICMPv6
pm                   interact with layered PM subsystem
ps                   Prints information about running threads.
reboot               Reboot the node
rpl                  rpl configuration tool ('rpl help' for more information)
version              Prints current RIOT_VERSION
> ifconfig
Iface  5  HWaddr: 35:3F  Channel: 11  NID: 0x23  PHY: O-QPSK 
          
          Long HWaddr: BA:D0:5B:74:8D:B9:35:3F 
           TX-Power: 0dBm  State: IDLE  max. Retrans.: 3  CSMA Retries: 4 
          AUTOACK  ACK_REQ  CSMA  L2-PDU:102  MTU:1280  HL:64  RTR  
          RTR_ADV  6LO  IPHC  
          Source address length: 8
          Link type: wireless
          inet6 addr: fe80::b8d0:5b74:8db9:353f  scope: link  VAL
          inet6 addr: 2001:660:5307:3100:b8d0:5b74:8db9:353f  scope: global  VAL
          inet6 group: ff02::2
          inet6 group: ff02::1
          inet6 group: ff02::1:ffb9:353f
          inet6 group: ff02::1a
          
          Statistics for Layer 2
            RX packets 5  bytes 334
            TX packets 5 (Multicast: 2)  bytes 264
            TX succeeded 5 errors 0
          Statistics for IPv6
            RX packets 4  bytes 368
            TX packets 5 (Multicast: 2)  bytes 354
            TX succeeded 5 errors 0


```
Leave the Terminal Open. Do not close the Shell.
</details>

<details>

<summary>STEP 6. Configure and Start Mosquito.RSMB </summary>

 in another terminal, log on to the remaining A8 node, node-a8-1. We are going to configure and start the MQTT-SN broker as follows:

```ruby
  my_computer$ ssh <login>@grenoble.iot-lab.info
  login@grenoble:~$ ssh root@node-a8-1
```
> Note that mosquito.rsmb is pre-installed in these  A8 devices which makes it easier to configure. if it is not installed get it from [here](https://github.com/eclipse/mosquitto.rsmb).

Edit a file config.conf (vim config.conf) with the following content:

```ruby

# add some debug output
trace_output protocol
   
# listen for MQTT-SN traffic on UDP port 1885
listener 1885 INADDR_ANY mqtts
ipv6 true
   
# listen to MQTT connections on tcp port 1886
listener 1886 INADDR_ANY
ipv6 true
```

Important, note the global IPv6 address of this node, since we’ll use it to connect to the MQTT broker from the node:
```ruby
root@node-a8-1:~# ip -6 -o addr show eth0
2: eth0    inet6 2001:660:5307:3000::67/64 scope global \       valid_lft forever preferred_lft forever
2: eth0    inet6 fe80::fadc:7aff:fe01:95f8/64 scope link \       valid_lft forever preferred_lft forever

```

start the broker: broker_mqtts config.conf
```ruby
root@node-a8-1:~# broker_mqtts config.conf
20170715 001526.077 CWNAN9999I Really Small Message Broker
20170715 001526.084 CWNAN9998I Part of Project Mosquitto in Eclipse
(http://projects.eclipse.org/projects/technology.mosquitto)
20170715 001526.088 CWNAN0049I Configuration file name is config.conf
20170715 001526.099 CWNAN0053I Version 1.3.0.2, Jul 11 2017 14:55:20
20170715 001526.102 CWNAN0054I Features included: bridge MQTTS 
20170715 001526.104 CWNAN9993I Authors: Ian Craggs (icraggs@uk.ibm.com), Nicholas O'Leary
20170715 001526.111 CWNAN0300I MQTT-S protocol starting, listening on port 1885
20170715 001526.115 CWNAN0014I MQTT protocol starting, listening on port 1886

```

if you see the global ipv6 prefix address configured then start the communication with MQTTs broker by executing the following command in the node shell.
```ruby
 username@grenoble:~/RIOT$ nc m3-102 20000
 >start 2001:660:5307:3000::67 1885 station1
 
```
<div align="center">
	
<img width="1073" alt="Screenshot 2023-12-14 at 15 01 40" src="https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/153508129/dfb98d68-f702-450c-8c3d-07711863ada3">

</div>div


</details>
<details>

<summary>STEP 7. Install the AWS IoT Device SDK for Python into a8-1 node </summary>

Run the following commands in a New Terminal to install the AWS IoT Device SDK for Python:

```ruby
   my_computer$ ssh <login>@grenoble.iot-lab.info
  login@grenoble:~$ ssh root@node-a8-1
```
```ruby
   root@node-a8-1:~# cd ~
   root@node-a8-1:~# git clone https://github.com/aws/aws-iot-device-sdk-python.git
   root@node-a8-1:~# cd aws-iot-device-sdk-python
   root@node-a8-1:~# python setup.py install
```

</details>

<details>

<summary>STEP 8. Start MQTTSN Client </summary>

before you proceed further, you need aws-iot core certificates/keys to access the aws-iot dashboard. follow the link given below to get your keys.


[Jump to a Create thing and Certificates](#Createathing)

In the same terminal,

use the following link to clone the below in the client node a8-1.
```ruby
    root@node-a8-1:~# git clone https://github.com/shalikadulaj/IoT-Mini-Project-1.git
```
#### Important
after you Obtain the AWS-IOT certifications, you need to create a clone link using your repository named "certs". 
```ruby
     root@node-a8-1:~# cd IoT-Mini-Project-1
     root@node-a8-103:~/IoT-Mini-Project-1# git clone https://github.com/"<username>"/certs.git
```
<a name="aftercreate"> </a>

once you followed the above Step 7 and 8 you will see the following directories created in the a8-1 node.

<div align="center">


<img width="537" alt="Screenshot 2023-12-14 at 03 49 15" src="https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/153508129/05329d7b-cc9b-4c22-82ae-1c22d4862c25">

</div>

before you run the Python script make sure you have set the path to aws certificates correctly in the file MQTTSbridge.py. 

path that indicates the certificates position (ssh root@node-a8-?)

```ruby
#path that indicates the certificates position (ssh root@node-a8-?)
path = "/home/root/IoT-Mini-Project-1/certs/"

# configure the access with the AWS MQTT broker:
MQTTClient.configureEndpoint("", 8883)
MQTTClient.configureCredentials(path+"",
                                path+"",
                                path+"")

```
```ruby
   root@node-a8-103:~/IoT-Mini-Project-1# python3 MQTTSNbridge.py
```
<div align="center">

<img width="1093" alt="Screenshot 2023-12-14 at 15 02 28" src="https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/153508129/1a8331fd-d107-45e2-a2d7-041629ca8ad3">

</div>


</details>



## Data Management and Visualization

## :cloud: AWS 

Amazon Web Services (AWS) is a leading cloud platform offering a comprehensive suite of services for data storage, visualization, and alerting. 

https://docs.aws.amazon.com/index.html 



## AWS IoT Core

AWS IoT Core is a managed cloud service that facilitates secure communication between IoT devices and the AWS Cloud. It ensures encrypted connectivity, device management, and seamless integration with AWS services. With features like device shadows and a scalable architecture, it's ideal for building secure and scalable IoT applications. 

https://docs.aws.amazon.com/iot/ 


The border router publishes sensor data from FIT IoT Lab to the specific topic in AWS IoT core. There are rules to control data, which receive to the IoT core.  






<a name="Createathing"> </a>

## Create a thing and Certificates

A thing resource is a digital representation of a physical device or logical entity in AWS IoT. Your device or entity needs a thing resource in the registry to use AWS IoT features such as Device Shadows, events, jobs, and device management features. 

Follow the below steps to create a thing 

	AWS IoT Core > Manage > All Device > Things > Create Things 
- Specify thing properties 

- Configure device certificate 

- Attach policies to the certificate 


Finally, you must download the device certificate, key files, and Root CA Certificates. These certificates should be added to the code. It is mentioned in the code, that you can replace the certificates with yours's. 


Now need to add the End point to the code. You can get the Endpoint from the below path 

	AWS IoT  > Settings > Device data endpoint 

[Go back to step 8](#aftercreate)




At this moment you can check whether the data is receiving. If not, you have to check the above steps again. To check follow the below steps. 

	AWS IoT > Test > MQTT test client > Subscribe to a topic (sensor/station1) > Subscribe 

Replace the topic with your topic. Now you can see the data is receiving as below. 

<div align="center">


![5](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/58818511/edc49f9e-cd09-487c-ad08-107e6074c859)

</div>



## AWS Timestream 

AWS Timestream is a fully managed, serverless time-series database service provided by Amazon Web Services (AWS). It is specifically designed to handle time-series data at scale. Time-series data is characterized by data points associated with timestamps. In this project, the data from the IoT core is ingested into the AWS Timestream database using AWS rules. 

**Ingesting data into Timestream**

Sample JSON data

	{
	  "id": "0",
	  "datetime": "2023-12-13 03:55:20",
	  "temperature": "-12",
	  "pressure": "345",
	  "lightLevel": "38"
	}

First, you need to add rules. Follow below steps to add rules 

	AWS IoT > Message Routing > Rules > Create rule 

- Specify rule properties 

- Configure SQL statement 
	- Write this quarry to select all the data coming from the topic, and ingest to the timestream. 

			SELECT * FROM 'sensor/station1'   

- Attach rule actions - This is the action when receiving data. 
	- Select - “Timestream table (write message into a Timestream table)” 
	- Add database - If you have not created a database, you can create a database by clicking on “Create Timetream database”. Select standard database. 
	- Add Table – Click on create timestream table 
	- Add a IAM role – Click on create new role 

- Review and create 


## AWS Managed Grafana

AWS Managed Grafana is a fully managed and scalable service that simplifies the deployment, operation, and scaling of Grafana for analytics and monitoring. It integrates seamlessly with other AWS services, offering a user-friendly interface for creating dashboards and visualizations to gain insights from diverse data sources. We are using Grafana for visualizing data using AWS Timestream as a data source. 

You can create the workspace as below 

	Amazon Managed Grafana > All workspaces > Create workspace 

- Specify workspace details 
	- Give a unique name 
	- Select Grafana version – We are using 8.4  

 

- Configure settings 
	- Select Authentication access - “AWS IAM Identity Center (successor to AWS SSO)” 

- Service managed permission settings 
	- Select data sources  - “Amazon TimeStream” 

- Review and create 

**Creating user**

	Amazon Managed Grafana > All workspaces > Select workspace created above > Authentication > Assign new user or group > Select User > Action > Make admin 

If you can't find a user, you have to add a user by below method 

	IAM Identity Center > Users >  Add user (giving email and other information) 

After adding you can see the user under "configure users" in your workspace 
 

Loging to Grafana workspace 

	Amazon Managed Grafana > All workspaces > Select workspace created above >  Click on “Grafana workspace URL” 

Sign in with AWS SSO 

	Add Data Source > Select Amazon Timestream > Select default region (should be equal to Endpoint region) 

Add data base, table and measure. Then save.

Now you are successfully connected the data source. Then using Grafana, you can create a dashboard as you need. 



## AWS SNS

Amazon Simple Notification Service (SNS) is a fully managed messaging service by AWS. It enables the publishing of messages to a variety of endpoints, including mobile devices, email, and more. SNS simplifies the creation and management of message-driven applications, providing flexibility and scalability for diverse communication scenarios. In here rules are set for triggering email alerts using AWS SNS service. To set rules follow below steps. 

	AWS IoT > Message Routing > Rules > Create rule 

- Specify rule properties 

- Configure SQL statement 
	- Write this quarry to select  data once the temperature value is greater than 50. And data will be sent to the topic.  


			SELECT *,timestamp() as ts FROM 'sensor/station1' WHERE temperature > 50 

- Attach rule actions - This is the action when receiving data. 
	- Select - “Simple Notification Service (SNS)” 
	- Add SNS topic – Create SNS topic 
	- Add a IAM role – Click on create new role 

- Review and create 

**Create a subscription**

	Amazon SNS > Topics > Select notification which you create above > Create subscription 

- Select protocol as “Email”. If you need to send any notification, you select any other option. 
	- Add Endpoint – email address  
	- Create Subscription 

- Login to email and confirm the subscription 
 

Here rules are set for triggering email alerts using the AWS SNS service. Write SQL quarries to trigger AWS SNS 



## AWS Dynamodb


AWS DynamoDB, a fully managed NoSQL database, we are using this for storing alert data. With seamless scalability and low-latency access, DynamoDB ensures reliable and fast retrieval of alert information. Its flexible schema accommodates evolving data needs, making it a robust solution for storing and retrieving dynamic alert data. 

**Create rule for ingesting alert data into dynamoDB**

	AWS IoT > Message Routing > Rules > Select rule which you create above (for sending email) > Edit
- Add another rule action  
	- Choose an action - “DyanamoDBv2 (Split message into multiple colums of a DynamoDB table” 
	- Add table – Click on “Create DynamoDB table” 
	- Add a IAM role – Click on “create new role” 


