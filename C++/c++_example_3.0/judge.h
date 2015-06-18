#ifndef JUDGE_H
#define JUDGE_H

#include <human.h>
#include <vector>

//当前类定义中只使用了Competitor的指针形式，
//所以此处只需要做类的说明，而不需要类的定义(包含头文件)
//但需要在.cpp文件中进行头文件包含
//优点：当Competitor作任何修改时，当前文件都不需要重新编译
//		提高整个项目的编译速度。
class Competitor;

class Judge : public Human
{
public:
	Judge(const Human &human);
	void setCompetitors(const Competitor *p1, 
					   const Competitor *p2);
	void start();
	void showResult()const;
	void setRound(int nRound);
private:
	int judgeFist(int fist1, int fist2)const;
private:
	const Competitor *c1;
	const Competitor *c2;
	int score1, score2;
	int round;
	vector<string> fistNames;	
	//容器类	中文名		存储形式				用途
	//vector	向量		数组					顺序存储，随机或顺序读写
	//list  	列表		链表					随机存储，顺序读写
	//map		映射		指针数组＋动态分配内存	以上两者折中
	
	// 向量与列表声明：
	//vector<T> 对象名(n);	声明一个存储n个T类型数据的对象
	// vector<int> vint(8);	分配8个int存储间
	// vector<Date> vDate;	只实例化容器对象，不分配存储内存

	//映射声明:
	// map<K,T> 对象名(n);	//K表示键，唯一的,相当于数组下标；
	// 						//T代表值类型，相当于数组元素类型
	// map<string, Date> m;	

	//列表的使用：
	//	list<int> vlist;
	//	vlist.push_back(2);
	//	vlist.push_back(3);
	//	vlist.push_back(5);	//向列表中添加数据
	//	list<int>::iterator it;	//声明一个迭代器
	//	it = vlist.begin();		//将迭代器指向容器首元素
	//	while(it != vlist.end())
	//	{
	//		cout << *it;	//*it取迭代器所指向的元素值
	//		++it;			//将迭代器指针下移
	//	}
	
	//向量也可以使用列表的访问方法，同时也可以以下面的方式访问:
	// vector<int> vvector(8);	//8用于指定向量的长度
	// vvector[0] = 8;	//如果在实列化向量时，没有指定向量长度
	// vvector[2] = 6;	//那么不可以使用左侧的这种方式为向量存入数据，
	// vvector[1] = 9;	//只能使用push_back或push_front
	// for(int i = 0; i < 8; i++)
	// {
	//		cout << vvector[i];
	// }
};

#endif // JUDGE_H

