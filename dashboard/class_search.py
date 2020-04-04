import json
import os
import time


class SearchJson:

    def __init__(self, start):
        # start represents the time, expressed in seconds, sent back in the search
        self.oraPart = time.time() - start
        self.counter = 0  # count of all files found
        self.JsonFiles = []  # list json file found

    # search for files in the time interval
    def searchFileInterval(self):
        for file in os.listdir("data/"):
            oraFile = float(file[:13])
            if oraFile > self.oraPart:
                self.JsonFiles.append(str(file))
                self.counter += 1
        print("file found --> " + str(self.counter))
        return self.JsonFiles

    # adds the contents of the list files into an array
    def parseValue(self, name_list):
        array = []
        for name_file in name_list:
            with open("data/" + name_file) as f:
                data = json.load(f)
                array.append(data)
        return str(array)
