
import os
def data_type() :
    print 'sssssssssss'
    print 'I\'m wqshao'
    print "I'm wqshao"
    print """
            I'm wqshao
            I like she
            I like free
          """
    print 'true: ' + str(True)
    print 'flase: ' + str(False)
    print 'true: ', True
    print 'flase: ', False

    print '2<3', 2 > 3

    a = 'ABC'
    b = a
    a = 'XYZ'
    print b

    print 'operate: ', 10 / 3, ':', 10.0 / 3,":", 10 % 3

def string_coding():
    print 'string_coding function'

    print "A:", ord('A')
    print "65:", chr(65)

    s1 = "u'ABC'"
    s2 = "'ABC'"
    print 'Unicode to utf-8:', s1.encode('utf-8')
    print 'utf-8 to unicode:', s2.decode('utf-8')

    print "a=%d, b=%d" %(1, 11)

def dict() :
    d = {'a' : 10, 'b' : 11, 'c' : 12 }

    print "dict: ", d['a'], d['b']

    d['a'] = 1231
    print "dict: ", d['a']
    print "if wqshao:", 'wqshao' in d
    print "if c:", 'c' in d
    print "get a:", d.get('a')
    print "get wqshao:", d.get('wqshao', -1)
    print "get a:", d.get('a', -1)
    d.pop('a')
    print "get a:", d.get('a', -1)

def list() :
    L = []

    for l in range(0, 10) :
        L.append(l)

    print L

    print [x for x in range(0, 10)]

    dir = [a for a in os.listdir('.')]
    print dir

def generator() :
    g = (x for x in range(0, 10))

    print g.next()

    for s in g :
        print s

def add(x, y, f) :
    return f(x) + f(y)

def func() :
    print '-10 abs:', abs(-10)
    a = abs
    print '-1 abs:', a(-10)
    # print "xxx", a

    # abs = 10
    # print '-10 abs:', abs(-10)

    a, b = 1, 2
    z = add(a, b,abs)
    print "1+2=", z

def my_add(x, y) :
    return x + y

def fn(x, y) :
    return x*10 + y

def my_map():
    print "string:", map(str, [1, 2, 3, 4, 5])

    #reduce(f, [x1, x2, x3, x4]) = f(f(f(x1, x2), x3), x4)
    print 'Add:', reduce(my_add, [1, 2, 3])
    print 'sum:', sum([1, 2 ,3])

    print "int:", reduce(fn, [1, 2, 3])

def char2num(s) :
    print s
    return {'0': 0, '1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9}[s]

def str2int() :
    print "int: %d" %reduce(fn, map(char2num, "21321431"))

    print "int1: %d" %reduce(lambda x,y: x*10+y, map(char2num, "1212121")) #DOTO: lambda

def name_chage(s) :
    return s.capitalize()

def name_test() :
    print "aaaa", name_chage("sdaasdAASD")
    L = ['adam', 'LISA', 'barT']
    print L
    print "1. ", map(name_chage, L)

def is_odd(i) :
    return i % 2 == 1

def not_empty(s) :
    return s and s.strip()

def my_filter() :
    print "a: ", filter(is_odd, [1, 2, 3, 4, 5])
    print "s: ", filter(not_empty, ["AAA", "", "s", '', None])

def is_prime(n) :
    if n <= 1 :
        return False
    i = 2
    while i*i <= n :
        if n % i == 0 :
            return False
        i += 1
    return True

def print_prime() :
    L = [n for n in range(0, 100)]
    print L
    print "11 is prime :", is_prime(11)
    print "Prime:", filter(is_prime, L)

def count() :
    fs = []
    for i in range(1, 4):
         def f(j):
             def g():
                 return j*j
             return g
         fs.append(f(i))
    return fs

def close_p():
    print "Count:", count()

    f = count()
    print "Count 1:", f[0]()
    print "Count 2:", f[1]()
    print "Count 3:", f[2]()


def lambda_func() :
    f = lambda x: x + 2
    print "func: ", f
    print "3+2=", f(3)

if __name__ == "__main__" :

    # data_type()
    # string_coding()
    # dict()
    # list()
    # generator()
    # func()
    # my_map()
    # str2int()
    # name_test()
    # my_filter()
    # print_prime()
    # close_p()
    lambda_func()
__author__ = 'WangQuan'
