from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import MQTTSNclient
import json

jsonP = ''

# Clients for MQTT and MQTTS
MQTTClient = AWSIoTMQTTClient("MQTTSNbridge")
MQTTSNClient = MQTTSNclient.Client("bridge", port=1885)

class Callback:
    # Function that replies a message from the MQTTSN broker to the MQTT one
    # and inserts into the database the message just arrived
    def messageArrived(self, topicName, payload, qos, retained, msgid):
        message = payload.decode("utf-8")
        jsonP = json.loads(message)
        print(topicName, message)
        MQTTClient.publish(topicName, message, qos)
        # DynamoDB integration (commented out for simplicity)
        # dynamoTable.put_item(Item=jsonP)
        return True


# Path that indicates the certificates position (ssh root@node-a8-?)
path = "/home/root/IoT-Mini-Project-1/certs/"

# Configure the access with the AWS MQTT broker
MQTTClient.configureEndpoint("a5hi9k1blxeai-ats.iot.eu-west-1.amazonaws.com", 8883)
MQTTClient.configureCredentials(
    path + "AmazonRootCA1.pem",
    path + "41caa97615d76184172427b6764bded91448dede2bfe1f8a7411c59de3300978-private.pem.key",
    path + "41caa97615d76184172427b6764bded91448dede2bfe1f8a7411c59de3300978-certificate.pem.crt"
)

# Configure the MQTT broker
MQTTClient.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
MQTTClient.configureDrainingFrequency(2)  # Draining: 2 Hz
MQTTClient.configureConnectDisconnectTimeout(10)  # 10 sec
MQTTClient.configureMQTTOperationTimeout(5)  # 5 sec

# Register the callback
MQTTSNClient.registerCallback(Callback())

# Make connections to the clients
MQTTClient.connect()
MQTTSNClient.connect()

station_ids = ""
print("Enter the ID of the station, one by one, that you want to subscribe to.")
print("Type 'stop' to interrupt the process.\n")
while True:
    current_id = input("")
    if current_id == 'stop':
        break
    else:
        station_ids += current_id + " "

# Subscribe to the topics chosen by the user
for id in station_ids:
    MQTTSNClient.subscribe("sensor/station" + id)
print("Subscribed to stations with ID: " + station_ids)

# Cycle that waits for a command to close the program
while True:
    if input("Enter 'quit' to exit from the program.\n") == "quit":
        break

# Disconnect from the clients
MQTTSNClient.disconnect()
MQTTClient.disconnect()
