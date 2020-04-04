import json
import random


class Stazione:

    def __init__(self, id):
        self.id = id
        self.temperature = 0
        self.humidity = 0
        self.wind_direction = 0
        self.wind_intensity = 0
        self.rain_height = 0

    # update the sensor data with random values
    def updateData(self):
        self.temperature = random.randint(-50, 50)
        self.humidity = random.randint(0, 100)
        self.wind_direction = random.randint(0, 360)
        self.wind_intensity = random.randint(0, 100)
        self.rain_height = random.randint(0, 50)

    # return the data in a json _dict_
    def getData(self):
        messages = json.dumps(self.__dict__)
        return messages
