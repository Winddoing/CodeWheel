#ifndef HUMAN_H
#define HUMAN_H

#include <iostream>
#include <date.h>

using namespace std;

class Human{
public:
	Human(const string &nName = "zhangshan", const Date &nBirthday = Date());
	const string &name()const;
	const Date &birthday()const;
	// void setName(string &nName);
	// void setName1(const string &nName);
	//	setName("zhangshan");	X		这两个调用的实参均不是string对象，要在
	//  setName1("zhangshan");	V		调用时实例化出一个string对象再进行调用
	//                                  此时自动实例化的对象是一个常对象
	//  string n("zhangshan");
	//	setName(n);		V
	//	setName1(n);	V
	//
	//  void setName2(string *nName);		//对象的指针与常指针
	//  void setName3(const string *nName);
	//  setName2(new string("zhangshan"));	X
	//  setName3(new string("zhangshan"));	V
	//  string *n = new string("zhangshan");
	//	setName2(n);	V
	//	setName3(n);	V
	//	什么时候用引用，什么时候用指针：堆用指针，栈用引用
	void setName(const string &nName);
	void setBirthday(const Date &nBirthday);
private:
	string n;
	Date b;
};

#endif // HUMAN_H

