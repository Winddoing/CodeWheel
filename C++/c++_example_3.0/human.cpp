#include <human.h>
#include <iostream>

using namespace std;

ostream & operator <<(ostream &os, const Human &human)
{
	os << human.name() << "  " << human.birthday();

	return os;
}
Human::Human()
{
	Human("zhang", Date());
}
Human::Human(const string &nName, const Date &nBirthday)
	:n(nName), b(nBirthday)
{
}
//const string &Human::name()const
//与 const string &Human::name()  是一对重载函数
const string &Human::name()const
{
	return n;
}
const Date &Human::birthday()const
//成员函数后加const代表此函数不修改类内的成员值，即可以通过常引用来调用此函数,如果不写const的同名同参函数，与这个带const的函数形成重载，通过常用或常指针调用的birthday函数是带const的，而非常引用或常指针调用的birthday函数是不带const的。
//此函数的返回值类型是什么？其const限定与函数无关，而与函数的返回值有关
//一般情况下在书写程序时，const一般只与指针和引用还有函数限定这三个部分有关
{
	return b;
}
/*
//类定义中
string *n;
//构造函数中
n = 0;
// funa()
if(0 == n)
	n = new string("zhang");
//funb()或析构函数
if(n != 0)
{
	delete n;
	n = 0;
}

void Human::setName(const string *nName)
{
	//....
}
*/
void Human::setName(const string &nName)
{
	n = nName;	
}
void Human::setBirthday(const Date &nBirthday)
{
	b = nBirthday;
}

