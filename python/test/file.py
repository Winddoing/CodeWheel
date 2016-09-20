
import linecache

def read():
    fs = open("record.txt")
    buf = fs.read()
    print buf
    fs.close()

def printf():
    buf = linecache.getlines("record.txt")
    score_less_60 = [ ]
    name_in_L = [ ]
    total = 0

    for i in buf:
        str1 = i.split(',')
        if str1[0][0] != '#':
            if int(str1[2]) < 60:
                score_less_60.append(str1[0])
            if str1[0][0] == 'L':
                name_in_L.append(str1[0])
            total = total + int(str1[2])
    print "score < 60 :", score_less_60
    print "name contains 'L':", name_in_L
    print "Total socre:", total

def check():
    buf = linecache.getlines("record.txt")
    name_in_lower = [ ]

    for i in buf:
        str1 = i.split(',')
        if str1[0][0] != '#':
            if str1[0][0].islower():
                name_in_lower.append(str1[0])
                i.replace(str1[0][0], str1[0][0].upper())
                print str1[0][0].upper()
    print "name contains loewr:", name_in_lower
    read()

def esc():
    global flag
    flag = False

def switch(s):
    dict_switch.get(s)()

if __name__ ==  "__main__":
    flag = True
    dict_switch = {"1":read,
                   "2":printf,
                   "3":check,
                   "4":esc,
                  }
    while 1:
        if flag:
            s = raw_input("Please choose:")
            switch(s)
        else:
            break


