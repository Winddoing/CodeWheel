#include <iostream>
#include <machcompetitor.h>
#include <judge.h>

using namespace std;

int main(int argc, char *argv[])
{

	/*
	Competitor c1(Human("zhang", Date(1981, 1, 1)));
	MachCompetitor c2(Human("netebook", Date(2014, 1, 1)));
	Judge judge(Human("prog", Date(2014, 1, 1)));
	judge.setCompetitors(&c1, &c2);
	judge.setRound(3);
	judge.start();
	judge.showResult();
	*/

	/* cout << human.birthday() << endl; */

	//cout << c1 << endl;	//多态特性
	//ostream &operator<<(ostream &, const Human &);
	//在此处调用的就是上面的运算符重载。
	//因为c1是Competitor类的对象，而这个类是Human的子类
	//即Competitor类包含了Human类:c1中包含h

//	int n = c1.showFist();
	
//	cout << n << endl;

// date类时测试
	//构造对象是通过调用类的构造函数来完成的。

	Date *birthday = new Date();	//birthday指针在栈中，对象存在堆中
	Date bir;                       //bir对象在栈中

	birthday->setYear(1998);
	birthday->setMonth(8);
	(*birthday).setDay(1);
	cout << birthday->year() << "/"
		 << birthday->month() << "/"
		 << birthday->day() << endl;
	cout<<bir<<endl;               //运算符重载
	return 0;
}

