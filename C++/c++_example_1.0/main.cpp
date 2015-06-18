#include <iostream>
#include <human.h>

using namespace std;

int main(int argc, char *argv[])
{
	Human human;

	/* cout << human.birthday() << endl; */

	cout << human.birthday() << " "
		 << human.name() << endl;
	
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

