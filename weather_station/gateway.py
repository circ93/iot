import paho.mqtt.client as mqtt
from class_controller import Controller

# callback for message send by broker
def on_message(client, userdata, message):
    data = str(message.payload.decode("utf-8"))
    print("message received --> " + data)
    #print("message topic=",message.topic)
    print("forwarding message to aws ...")
    controller.publishSensorRiotToTopic(data)
    print("forwarded message!")

# connection to broker and subsribe to topic
def mosquittoConnect(id_client, topic):
    client = mqtt.Client(id_client)
    client.on_message = on_message
    client.connect("localhost", 1886)
    print("Connected to Broker on port -> 1886")
    client.subscribe(topic)
    print("Subscribe to topic " + topic)
    client.loop_forever()


if __name__ == "__main__":
    controller = Controller("sensor_client", "data_read")
    controller.awsConnect()
    mosquittoConnect("client1", "data_read")