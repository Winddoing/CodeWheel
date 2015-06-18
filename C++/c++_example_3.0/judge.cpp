#include <judge.h>
#include <competitor.h>

Judge::Judge(const Human &human)
	:Human(human), c1(0), c2(0)
{
	fistNames.push_back("stone");
   	fistNames.push_back("sicssors");
	fistNames.push_back("cloth");
}
void Judge::setCompetitors(const Competitor *p1, 
					   const Competitor *p2)
{
	c1 = p1;
	c2 = p2;
}
void Judge::start()
{
	int f1, f2;	//c1和c2的showFist值
	int res;	//本轮比分比较结果 0:平  1:c1胜 2:c2胜

	if(0 == c1 || 0 == c2)	//检查两位选手是否已经设置
	{
		cout << "please set competitors!" << endl;
		return;
	}
	score1 = score2 = 0;	//实始化两人比分
	while(score1 < round && score2 < round)	
	{
		cout << "wait " << c1->name() << " show fist :" << endl;
		f1 = c1->showFist();
		cout << c1->name() << " fist is: " << fistNames[f1 - 1] << endl;
		cout << "wait " << c2->name() << " show fist :" << endl;
		f2 = c2->showFist();
		cout << c2->name() << " fist is: " << fistNames[f2 - 1] << endl;
		res = judgeFist(f1, f2);
		if(1 == res)
		{
			score1++;
		}else if(2 == res)
		{
			score2++;
		}
		cout << "current scores is " << score1 << " : " << score2 << endl;
		cout << endl;
	}
}
void Judge::showResult()const
{
	if(0 == c1 || 0 == c2)
	{
		cout << "please set competitors!" << endl;
		return;
	}
	cout << ((score1 > score2) ? c1->name() : 
			c2->name()) << " is victory !" << endl;
}
int Judge::judgeFist(int fist1, int fist2)const
{
	return (fist2 + 3 - fist1) % 3;
}
void Judge::setRound(int nRound)
{
	round = nRound;
}

