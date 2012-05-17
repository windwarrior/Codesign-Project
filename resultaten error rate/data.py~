import os
import glob
def main():
    for infile in glob.glob(os.path.join('', 'channel*')):
        pingPongDict = []
        file = open(infile)
        print "current file is: " + infile
        for line in file:
            if line is None:
                continue        
            linesplit = line.split(' ')
            if linesplit[0] == "ping":
                pingPongDict.append({
                    "id": int(linesplit[1]),
                    "ping": int(linesplit[2]),
                    "pong": 0,
                    "ponged": False,
                })
            elif linesplit[0] == "pong":
                if len(pingPongDict) == 0:
                    pass
                res = pingPongDict.pop()
                res['ponged'] = True
                pingPongDict.append(res)
                current = int(linesplit[1])
            elif linesplit[0] == "current":
                #print pingPongDict[current]
                curr = pingPongDict.pop()
                curr['pong'] = int(linesplit[2])

                pingPongDict.append(curr)
        print "ErrorRate: " + str(100 - len(filter(listFunction, pingPongDict))) + "%"
        #print map(calculateRTT, pingPongDict)
        avg = calculateAvgRTT(pingPongDict)
        if avg > 0:
            print "Average RTT: " + str(avg)
                
def listFunction(pingPongDict):
    return pingPongDict["ponged"]

def calculateRTT(pingPongDict):
    return 0 if pingPongDict['pong'] == 0 else pingPongDict['pong'] - pingPongDict['ping']

def calculateAvgRTT(pingPongList):
    return sum(map(calculateRTT, pingPongList)) / len(pingPongList)  
    	
main()
