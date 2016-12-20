
# -*-coding:UTF-8-*-

from types import MethodType

class Std(object):
    __slots__ = ('name', 'age')  #����Std�ܶ�̬��ӵ�����
    pass

def set_age(self, age):
    self.age = age

if __name__ == "__main__":
    s1 = Std()

    s1.name = "12"
    s1.age = 10
    # s1.number = 1 #error
    # s1.set_age = MethodType(set_age, s1, Std)
    Std.set_age = MethodType(set_age, None, Std)

    s1.set_age(222)

    print ("name: %s, age: %d", s1.name, s1.age)

    s2 = Std()

    s2.set_age(23)
    print ("age: %d",s2.age)




__author__ = 'WangQuan'
