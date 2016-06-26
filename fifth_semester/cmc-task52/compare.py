#!/usr/bin/env python2

import sys
import getopt
import heapq

class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg

def parseFile(in_fileName):
    out_result = {'answer': 0, 'objectArray': []}
    fileId = open(in_fileName, 'r')
    line = fileId.readline()
    out_result['answer'] = int(line)
    line = fileId.readline()
    nObjects = int(line)
    for line in fileId:
        wordList = line.split()
        nWords = len(wordList)
        if (nWords != 3) and (nWords != 7):
            raise Usage('Incorrect line formatting: ' + line)

        obj = {'type': [], 'center': []}
        obj['type'] = wordList[0];
        obj['center'] = [float(wordList[1]), float(wordList[2])]

        if (obj['type'] == 'Gear') and (nWords != 7):
            raise Usage('Not enough parameters of the gear')
        if (obj['type'] == 'Axis'):
            continue
        obj['maxR'] = float(wordList[3])
        obj['minR'] = float(wordList[4])
        obj['isBroken'] = wordList[5].lower() == "TRUE".lower()
        obj['nGears'] = int(wordList[6])
        out_result['objectArray'].append(obj)
    fileId.close();
    return out_result

def checkObjects(in_resultArray, in_gtArray):
    distArray = computeDistanceMatrix(in_resultArray, in_gtArray)
    nObjects = len(in_resultArray)
    resultIdxArray = range(nObjects)
    gtIdxArray = range(nObjects)
    isGood = True
    threshold = 9
    nCorrectGears = 0
    for idx in range(nObjects):
        while True:
            dist, iObj, iGt = heapq.heappop(distArray)
            if (iObj in resultIdxArray) and (iGt in gtIdxArray):
                resultIdxArray.remove(iObj)
                gtIdxArray.remove(iGt)
                break
        isGoodGear = dist <= threshold
        found = in_resultArray[iObj]
        gt = in_gtArray[iGt]
        isGoodGear = isGoodGear and (found['type'] == gt['type'])
        if (isGoodGear and (gt['type'] == 'Gear')):
            isGoodGear = isGoodGear and (abs(found['maxR'] - gt['maxR']) <= 2)
            isGoodGear = isGoodGear and (abs(found['minR'] - gt['minR']) <= 2)
            isGoodGear = isGoodGear and (found['isBroken'] == gt['isBroken'])
            nCorrectGears = nCorrectGears + (found['nGears'] == gt['nGears'])
            isGood = isGood and isGoodGear
    assert(not resultIdxArray)
    assert(not gtIdxArray)
    return (isGood,    nCorrectGears)

def computeDistanceMatrix(in_results, in_gt):
    nObjects = len(in_results)
    nGt = len(in_gt)
    distArray = []
    for iObject in range(nObjects):
        c1 = in_results[iObject]['center']
        for iGt in range(nGt):
            c2 = in_gt[iGt]['center']
            dist = c1[0] - c2[0]
            dist = dist * dist
            tmp = c1[1] - c2[1]
            dist = dist + tmp * tmp
            heapq.heappush(distArray, (dist, iObject, iGt))
    return distArray

def main(argv=None):
    if argv is None:
        argv = sys.argv
    try:
        if len(argv) != 3:
            raise Usage("Not enough input arguments")

        result = parseFile(argv[1])
        gt = parseFile(argv[2])

        isCorrect = result['answer'] == gt['answer']

        nGt = len(gt['objectArray'])
        isCorrect = isCorrect and len(result['objectArray']) == nGt
        if (isCorrect):
            isCorrect, nGears = checkObjects(result['objectArray'], gt['objectArray'])
        else:
            nGears = 0
        if (isCorrect):
            print("OK")
        else:
            print("Fail")
        print(float(nGears) / nGt)
    except Usage, err:
        print >>sys.stderr, err.msg
        print >>sys.stderr, "Usage <file with results> <file with groundtruth>"

if __name__ == "__main__":
    sys.exit(main())

