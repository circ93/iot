import base64
import ttn
from class_controller import Controller

app_id = "weather_station_iotlab"
access_key = "ttn-account-v2.mRwSzIdDG-oa4P1JQCQdAsBy-Ms9v1qYEsBkxJ_4CAo"


def uplink_callback(msg, client):
    print("Received uplink from ", msg.dev_id)
    payload = base64.b64decode(msg.payload_raw).decode()
    controller.publishSensorRiotToTopic(payload)
    print("Successfully sent to aws!")


def ttn_connect():
    handler = ttn.HandlerClient(app_id, access_key)

    # using mqtt client
    mqtt_client = handler.data()
    mqtt_client.set_uplink_callback(uplink_callback)
    mqtt_client.connect()
    print("Connect to ttn!")

    a = 0
    while (True):
        a += 1


if __name__ == "__main__":
    controller = Controller("sensor_client", "data_read")
    controller.awsConnect()
    ttn_connect()
