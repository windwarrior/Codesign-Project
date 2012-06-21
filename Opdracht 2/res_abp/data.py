import os
import glob
def main():
    for infile in glob.glob(os.path.join('','better*')):
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
                        "resend": 0,
                        "acked": False,
                        "radiofail":1 if linesplit[5].strip() == "failed" else 0,
                    }
                    ping_ack_array.append(dict)

                else:
                    if linesplit[5].strip() == "failed":
                        item["radiofail"] = item["radiofail"] + 1
                    item["resend"] = item["resend"] + 1
            if linesplit[0] == "Got":
                result = None
                for item in ping_ack_array:
                    if item["seq"] == int(linesplit[4]):
                        result = item
                if result is None:
                    print "Dit kan niet"
                else:
                    result["acked"] = True
        print filter(filterAcked, ping_ack_array)
        for filtered_item in filter(filterMoreThanOneTry, ping_ack_array):
            print "Sequence number: %d, Radio timeout: %d, Radio fail: %d" % (filtered_item["seq"], filtered_item["resend"] - filtered_item["radiofail"], filtered_item["radiofail"])
  
def filterMoreThanOneTry(pingPongDictionairy):
    return pingPongDictionairy["resend"] > 0


def filterAcked(pingDictionairy):
    return not pingDictionairy["acked"]  
main()
