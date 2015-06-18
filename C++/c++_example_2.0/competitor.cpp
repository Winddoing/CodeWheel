#include <competitor.h>
#include <iostream>

using namespace std;

Competitor::Competitor(const Human &human)
	:Human(human)	//在子类实例化时，先要构造父类对象
{
}
Competitor::FistType Competitor::showFist()const
{
	return thinkFist();
}
Competitor::FistType Competitor::thinkFist()const
{
	int n;

	while(1)
	{
		cout << "Input fist value(1.stone 2.sicssors 3.cloth)";
		cin >> n;
		if(n >= Stone && n <= Cloth) break;
		cout << "Invalid fist value ! try again !" << endl;
	}
	//注意enum与int类型之间在赋值操作时的转换,enum可以自动转换为int,但int必须
	//强制转换为enum;但是转换前一定要注意int的值是否在enum范围内
	return (FistType)n;
}

