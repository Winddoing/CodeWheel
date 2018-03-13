#!/usr/bin/env python
import sys
import os
import struct
def main(fn):
    with open(fn,"rb") as fd:
        while True:
            stream = fd.read(32)
            if(len(stream) == 32):
                (l1,l2,l3,l4,l5,l6,l7,l8) = struct.unpack("8I",stream)
                print("%08x%08x%08x%08x%08x%08x%08x%08x" % (l8,l7,l6,l5,l4,l3,l2,l1))
            else:
                break
    if(len(stream) > 0):
        d = len(stream)
        fmt = str(d) + "B"
        b = struct.unpack(fmt,stream)
        s1=""
        for i in (range(0,32 - d)):
            s1 = s1 + "00"

        for i in range(0,len(b)):
            s1 = s1 + str.format("%02x" % b[i])
        print("%s" % (s1))


if __name__ == '__main__':
    if(len(sys.argv) == 2):
        main(sys.argv[1])
    else:
        print("%s [filename]" %sys.argv[0])
