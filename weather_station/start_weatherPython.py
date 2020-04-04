import time
from class_controller import Controller
from class_stazione import Stazione

# initialize a station
stazione1 = Stazione(1)
stazione2 = Stazione(2)
# initialize a controller
controller = Controller("sensor_client", "data_read")
# add the stations to the controller
controller.addStation(stazione1)
controller.addStation(stazione2)
# connection to Aws Iot
controller.awsConnect()

loopCount = 0
# number of publications on the topic
while loopCount < 100:
    # publication of data on the topic
    controller.publishSensorPythonToTopic()
    loopCount += 1
    # waiting between one publication and another
    time.sleep(11)
