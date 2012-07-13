import os
import glob
import sys
import re

def main():
    for infile in glob.glob(os.path.join('', 'res*')):
        resultArray = []
        currFile = open(infile)
        print "The current file is %s" % infile
        
        for line in currFile:
            if len(line.strip()) == 0:
                continue
            lineStrip = line.strip()
            print "'%s'" % lineStrip
            lineSplit = lineStrip.split(' ')
            currentLineDict = {
                "min": sys.maxint,
                "max": 0,
                "derv": 0,
                "avg": 0,
                "samples": 0,
                "total": 0,
                "distance": 0,
            }
            for item in lineSplit:
                if(re.match("\[.*\]", item)):
                    print "current distance is %s cm" % item.strip('[]')
                    currentLineDict["distance"] = item.strip('[]')
                    continue
                print "'%s'" % item
                value = int(item)
                currentLineDict["min"] = value if value < currentLineDict["min"] else currentLineDict["min"]
                currentLineDict["max"] = value if value > currentLineDict["max"] else currentLineDict["max"]
                currentLineDict["samples"] += 1
                currentLineDict["total"] += value

            currentLineDict["avg"] = currentLineDict["total"] / currentLineDict["samples"]
            currentLineDict["derv"] = currentLineDict["max"] - currentLineDict["min"]

            resultArray.append(currentLineDict)

        print resultArray

main()
