# IoT-Mini-Project-1

Edit a file config.conf (vim config.conf) with the following content:


## Introduction

University of Oulu,Group Project,Getting sensor data form FIT IoT Lab.
We are only using free AWS services for this project.


## Requirements

- AWS Account

- 


# Sensor Reading

## add some debug output
trace_output protocol
   
## listen for MQTT-SN traffic on UDP port 1885
listener 1885 INADDR_ANY mqtts
  ipv6 true
   
## listen to MQTT connections on tcp port 1886
listener 1886 INADDR_ANY
  ipv6 true

## Network Layer 


## AWS 

Amazon Web Services (AWS) is a leading cloud platform offering a comprehensive suite of services for data storage, visualization, and alerting. 

https://docs.aws.amazon.com/index.html 

![1 consol](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/58818511/49f29d6d-afdc-4c8c-9ca6-81ae0961c3d6)



## AWS IoT Core

AWS IoT Core is a managed cloud service that facilitates secure communication between IoT devices and the AWS Cloud. It ensures encrypted connectivity, device management, and seamless integration with AWS services. With features like device shadows and a scalable architecture, it's ideal for building secure and scalable IoT applications. 

https://docs.aws.amazon.com/iot/ 


![iot core](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/58818511/fd9b551f-bc3a-40f5-9816-d71e174e4d57)


The border router publishes sensor data from FIT IoT Lab to the specific topic in AWS IoT core. There are rules to control data received to the IoT core.  

Create a thing 

A thing resource is a digital representation of a physical device or logical entity in AWS IoT. Your device or entity needs a thing resource in the registry to use AWS IoT features such as Device Shadows, events, jobs, and device management features. 

Follow the below steps to create a thing 

AWS IoT Core > Manage > All Device > Things > Create Things 

    Specify thing properties 

	 Configure device certificate 

	 Attach policies to the certificate 

Finally, you have to download the device certificate, key files, and Root CA Certificates. These certificates should be added to the code. 

![3](https://github.com/shalikadulaj/IoT-Mini-Project-1/assets/58818511/df5bf67f-3725-4837-8467-de4bcd8478ba)

As well as need to add the End point to code. You can get the Endpoint from below path 

IoT Core  > Settings > Device data endpoint 

## AWS Timestream 

AWS Timestream is a fully managed, serverless time-series database service provided by Amazon Web Services (AWS). It is specifically designed to handle time-series data at scale. Time-series data is characterized by data points associated with timestamps 

In this project Data from the IoT core is injected into the AWS Timestream database using AWS rules 

JSON values of Date and Time,Station ID,Device ID ,Temperature ,Pressure and Light level insert in to the particular table 


## AWS Managed Grafana

AWS Managed Grafana is a fully managed and scalable service that simplifies the deployment, operation, and scaling of Grafana for analytics and monitoring. It integrates seamlessly with other AWS services, offering a user-friendly interface for creating dashboards and visualizations to gain insights from diverse data sources. 

Visualizing data using AWS Timestream as a data source. 


## AWS SNS

Amazon Simple Notification Service (SNS) is a fully managed messaging service by AWS. It enables the publishing of messages to a variety of endpoints, including mobile devices, email, and more. SNS simplifies the creation and management of message-driven applications, providing flexibility and scalability for diverse communication scenarios. 

 

Here rules are set for triggering email alerts using the AWS SNS service. 

Write sql quarries to trigger AWS SNS 



## AWS Dynamodb

AWS DynamoDB, a fully managed NoSQL database, we are using this for storing alert data. With seamless scalability and low-latency access, DynamoDB ensures reliable and fast retrieval of alert information. Its flexible schema accommodates evolving data needs, making it a robust solution for storing and retrieving dynamic alert data. 

 



