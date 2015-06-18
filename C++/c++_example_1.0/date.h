#ifndef DATE_H
#define DATE_H

#include <iostream>

using namespace std;

class Date;

ostream & operator << (ostream &os, const Date &date);

class Date
{
public:
	//友元函数允中允许通过对象直接访问对向中私有成员
	friend ostream & operator << (ostream &os, const Date &date);
	//构造函数有两种：一般构造函数和复制构造函数
	//1. 如果当前类中没有定义复制构造函数，系统会为
	//   当前类添加一个默认复制构造函数：如：
	//   Date(const Date &date);  参数为当前类的常引用
	//   引用实际上就是一个别名
	//   如：int a = 9;
	//       int &b = a; 声明了一个引用，它引用了a,实际上此条语句
	//                   没有分配任何内存，只是给a取了一个别名叫b;
	//                   切记:声明引用时，一定要进行直接初始化。
	//                       如：int &b;    b = a;     X
	//       b = 4;
	//       a = ?		//4
	//   意味着在Date复制构造函数调用时，不会改变date所引用对象的值
	//   只能调用Date类中的末尾带const的成员函数
	//2.  使用引用有什么好处,
	//   	如：void aaa(Date date1);	  void bbb(Date &date2);比较
	//   		aaa(a);					  bbb(b);
	//   	函数调用时，是用实参为形参赋值
	//   	Date date1 = a;				  Date &date2 = b;
	//		date1调用Date类的复制构造函数实例化了一个Date对象
	//		date2是一个引用，它不调用任何函数
	//		date2作为b对象的引用传入到bbb函数中，即意味着bbb函数可以修改b对象
	//		基于上两方面的原因，所以函数参数是引用时，多数情况下都写为常引用
	//		如: void bbb(const Date &date2);
	//3. 一般情况下都在哪些位置使用引用：
	//		函数参数、返回值。
	//一般情况下如果自定义类中没有定义任何形的构造函数时，系统会为当前类
	//添加一个无参构造函数，如:Date();
	//4. C＋＋中函数的形参允许有默认值，右对齐,只能在类定义中写默认值，
	//   在函数定义时不可以写默认值,函数调用时，有默认值的形参，可以不传递实参
	Date(int nYear = 1991, int nMonth = 8, int nDay = 1);
	//重载：函数名称相同，但参数的数数量或类型不同构成重载
	//      注意：二义性,如函数调用时无法区分到底调用哪个函数时即二义性
	// Date(int nYear, int nMonth = 8, int nDay = 1);
	// Date(int nYear);
	//
	//  Date date(1988);
	//
	void setYear(int nYear);
	void setMonth(int nMonth);
	void setDay(int nDay);
	int year() const;	//取得年份
	int month() const;
	int day() const;
private:
	int y, m, d;
};

#endif // DATE_H

