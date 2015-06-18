#include <human.h>

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
{
	return b;
}
void Human::setName(const string &nName)
{
	n = nName;	//调用复制构造函数    两对象之间使用等号运算符赋值实际上是调用复制构造函数
}
void Human::setBirthday(const Date &nBirthday)
{
	b = nBirthday;
}

