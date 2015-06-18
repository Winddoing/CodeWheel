#ifndef JUDGE_H
#define JUDGE_H

#include <human.h>

class Judge : public Human
{
public:
	Judge(const Human &human);
	void setCompetitor(const Competitor *p1, 
					   const Competitor *p2);
	void start();
	void showResult()const;
	int judgeFist(int fist1, int fist2)const;
	void setRound(int nRound);
private:
	const Competitor *c1;
	const Competitor *c2;
	int score1, score2;
	int round;
};

#endif // JUDGE_H

