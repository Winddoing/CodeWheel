/*#############################################################
 *     File Name	: string.cpp
 *     Author		: wqshao
 *     Created Time	: 2018年09月26日 星期三 15时14分46秒
 *     Description	:
 *############################################################*/

#include <iostream>

#include <string>               // make string class available
//#include <cstring>              // C-style string library

using namespace std;

int main()
{
	using namespace std;

	//char数组的用法
	char charr1[20];
	//int num1 = strlen(charr1);//没有初始化的char数组，长度是未知的，原因是需要遇到‘\0’才结束

	char charr2[] = "lilei,nihao";//编译器会自动计算长度
	char charr3[100] = "lilei,nihao";
	int goodFrameCount = 100;

	sprintf_s(charr3, "chess%d.jpg", goodFrameCount);//这种用法重点学习
	int num2 = strlen(charr3);//num2=12，strlen函数只计算可见的字符
	int num3 = sizeof(charr3); //num3=100，sizeof计算整个数组的长度

	char charr4[20];
	//cin >> charr4;//注意两点：1、遇到空格，认为结束；
	//cout << charr4 << endl; //2、当输入的字符串长度超过charr4的长度时，也是可以显示出来的，但不推荐使用

	cin.getline(charr4, 20);     // indicate maximum length,此种方法就可以输入包含空格的字符串了
	cout << charr4 << endl;

	char charr5[20];
	char charr6[20] = "jaguar";
	strcpy_s(charr5, charr6);//赋值操作， charr6内容赋值给charr5
	strcat_s(charr5, " juice");//添加到末尾




	//string类的用法
	//string类，感觉本质上讲，就是一个vector<char>
	string str;
	int number1 = str.size();//计算个数，未初始化时，个数为0，区别于char数组

	//cin >> str;
	//cout << str << endl;//同样存在的问题，字符串之间不能够有空格，空格就意味着‘\0’结束的标志

	getline(cin, str);
	cout << str << endl;//就可以输入含空格的字符串了

	string s1 = "penguin";
	string s2, s3;
	s2 = s1;//string的赋值简单多了
	s2 = "buzzard";
	s3 = s1 + s2;//字符串合并
	s1 += s2;//将字符串s2添加到s1的末尾，s1的内容发生了改变
	s2 += " for a day";//同样是加到s2的末尾

	return 0;
}
