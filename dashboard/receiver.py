try:
    import random
    import datetime
    import time
    import json
    from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
    from class_json import SaveJson

    print("All Modules Loaded...")
except Exception as e:
    print("Some Modules are Missing {}".format(e))


def custom_callback(client, userdata, message):
    print("Received a message and saved in json file.")
    # saving the message in a json file
    myJson = SaveJson(message.payload)
    myJson.saveJsonToTime()


# connection to Aws IoT
def aws_connection():
    # sensor_client -> id_client
    # data_read -> topic
    myMQTTReceiver = AWSIoTMQTTClient("receiver_client")
    # endpoint and port
    myMQTTReceiver.configureEndpoint("a3hb251ijbp00t-ats.iot.eu-central-1.amazonaws.com", 8883)
    # certificates of thing
    myMQTTReceiver.configureCredentials("cert/root-CA.crt", "cert/receiver/e59dcd1ffa-private.pem.key",
                                        "cert/receiver/e59dcd1ffa-certificate.pem.crt")
    myMQTTReceiver.configureOfflinePublishQueueing(-1)
    myMQTTReceiver.configureDrainingFrequency(2)
    myMQTTReceiver.configureConnectDisconnectTimeout(10)
    myMQTTReceiver.configureMQTTOperationTimeout(5)
    # connection
    myMQTTReceiver.connect()
    print("Connect success...")
    topic = "data_read"
    print("Subscribe " + topic + "...")
    # subscribe to the topic
    while 1:
        # the callback is invoked for each message received
        myMQTTReceiver.subscribe(topic, 1, custom_callback)


if __name__ == "__main__":
    aws_connection()
