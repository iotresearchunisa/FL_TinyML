import numpy as np

def parseData(lines = []):
    return np.asarray([float(x.strip()) for x in lines])

def aggregateWeights(weights=[]):
    weight = parseData(weights[0]["data"])
    
    for index in range(len(weights)):
        if index > 0:
            weight += parseData(weights[index]["data"])
    weights[index]["done"] = False
    weights[index]["data"] = []
    
    weight /= len(weights)
    
    return weight


