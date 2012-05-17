import os
import glob
def main():
    for infile in glob.glob(os.path.join('','T*')):
        ping_ack_array = []
        file = open(infile)
        print "current file is: " + infile
        for line in file:
            if line is None:
                continue
            linesplit = line.split(' ')
            if linesplit[0] == "Sent":
                result = None
                for item in ping_ack_array:
                    if item["seq"] == int(linesplit[4]):
                        result = item

                if result is None:
                    dict = {
                        "seq": int(linesplit[4]),
                        "sent": 1,
                        "acked": False
                    }
                    ping_ack_array.append(dict)

                else:
                    print "else"
                    print item["seq"]
                    item["sent"] = item["sent"] + 1
                    print item["sent"]
            if linesplit[0] == "Got":
                result = None
                for item in ping_ack_array:
                    if item["seq"] == int(linesplit[4]):
                        result = item
                if result is None:
                    print "Dit kan niet"
                else:
                    result["acked"] = True
        print ping_ack_array    
main()
