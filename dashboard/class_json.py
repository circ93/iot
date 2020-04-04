import time


class SaveJson:

    def __init__(self, json):
        self.json = str(json)

    # rename the file with epoch Time in chronological order
    def saveJsonToTime(self):
        lenght = len(self.json) - 1
        data = self.json[2:lenght]
        epochTime = time.time()
        name_file = "data/" + str(epochTime) + ".json"
        with open(name_file, 'w') as json_file:
            json_file.write(data)
