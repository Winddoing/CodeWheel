#include <iostream>
#include <competitor.h>

using namespace std;

int main(int argc, char *argv[])
{
	Human h;
	Competitor c1(h);

	/* cout << human.birthday() << endl; */

	cout << c1 << endl;	//多态特性
	//ostream &operator<<(ostream &, const Human &);
	//在此处调用的就是上面的运算符重载。
	//因为c1是Competitor类的对象，而这个类是Human的子类
	//即Competitor类包含了Human类:c1中包含h

	int n = c1.showFist();
	
	cout << n << endl;

/*  date类时测试
	//构造对象是通过调用类的构造函数来完成的。

	Date *birthday = new Date();	//birthday指针在栈中，对象存在堆中

//	birthday->setYear(1998);
//	birthday->setMonth(8);
//	(*birthday).setDay(1);

//cout 数据类型   std::ostream
//cin  数据类型	  std::istream
	cout << birthday->year() << "/"
		 << birthday->month() << "/"
		 << birthday->day() << endl;
*/
	return 0;
}

