#include <date.h>

ostream & operator << (ostream &os, const Date &date)
{
	os << date.y << "/"
	   << date.m << "/"
	   << date.d;

	return os;
}
Date::Date(int nYear, int nMonth, int nDay)
		:y(nYear),m(nMonth),d(nDay)	//初始化列表
{

}
void Date::setYear(int nYear)
{
	y = nYear;
}
void Date::setMonth(int nMonth)
{
	m = nMonth;
}
void Date::setDay(int nDay)
{
	d = nDay;
}
int Date::year() const
{
	return y;
}
int Date::month() const
{
	return m;
}
int Date::day() const
{
	return d;
}
