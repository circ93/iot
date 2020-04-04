from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
from class_stazione import Stazione


class Controller:
    def __init__(self, id_client, topic):
        self.id_client = id_client
        self.topic = topic
        self.stationList = []
        self.myMQTTClient = None

    # add the stations to a list
    def addStation(self, station):
        self.stationList.append(station)

    # connection to Aws Iot
    def awsConnect(self):
        # sensor_client -> id_client
        # data_read -> topic
        self.myMQTTClient = AWSIoTMQTTClient(self.id_client)
        # endpoint and port
        self.myMQTTClient.configureEndpoint("a3hb251ijbp00t-ats.iot.eu-central-1.amazonaws.com", 8883)
        # certificates of thing
        self.myMQTTClient.configureCredentials("cert/root-CA.crt", "cert/sensor/e7296f5b9d-private.pem.key",
                                               "cert/sensor/e7296f5b9d-certificate.pem.crt")
        self.myMQTTClient.configureOfflinePublishQueueing(-1)
        self.myMQTTClient.configureDrainingFrequency(2)
        self.myMQTTClient.configureConnectDisconnectTimeout(10)
        self.myMQTTClient.configureMQTTOperationTimeout(5)
        # connection
        self.myMQTTClient.connect()
        print("Connected to Aws!")

    # publication on the topic
    def publishSensorPythonToTopic(self):
        # scroll the list of stations
        for station in self.stationList:
            # update the station data
            station.updateData()
            data = station.getData()
            # publish the station data on the topic
            self.myMQTTClient.publish(self.topic, data, 0)
            print('Published topic -> %s: %s\n' % (self.topic, data))

    # publication on the topic
    def publishSensorRiotToTopic(self, data):
        # publish the message forwarded by broker
        self.myMQTTClient.publish(self.topic, data, 0)
        print('Published topic -> %s: %s\n' % (self.topic, data))

    
