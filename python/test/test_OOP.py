
#-*-coding:UTF-8-*-

class Std(object):
    def __init__(self, name, score) :
        self.__name = name
        self.__score = score

    def print_score(self) :
        print "%s,%s" %(self.__name, self.__score)

    def __set_name(self, name) :
        self.__name = name

    def set_score(self, score):
        self.__score = score

    def print_info(self):
        print "I'm a Student"

class Task(Std):
    pass

class bag(Std):
    pass


if __name__ == "__main__" :

    a = Std("aaa", 12)

    task1 = Task("sss", 23)

    task1.print_info()
    print a

    # print Std, a.name
    print "111:", a._Std__name

    # a.name = "qwwqwq"
    # print a.name

    # a.__set_name("1111111111")
    a._Std__set_name("1111111111")
    a.print_score()

    a.set_score(333)

    a.print_score()


    L = []
    L = ()

    print "L is list: ", isinstance(L, list)
    print "a is Std type: ", isinstance(a, Std)
    print "task1 is Task type: ", isinstance(task1, Task)
    print "task1 is Task type: ", isinstance(task1, Std)


    a = 12
    b = '123'

    print ("a Type: ", type(a))
    print "b Type: ", type(b)
    print "task1 Type: ", type(task1)

    print dir()

    print dir("abc")

    print dir(Std)

    print __doc__

    print __file__, __package__


__author__ = 'WangQuan'
