#ifndef COMPETITOR_H
#define COMPETITOR_H

#include <human.h>

class Competitor : public Human
{
public:
	enum FistType{None = 0, Stone, Sicssors, Cloth};
	Competitor(const Human &human);
	FistType showFist()const;
protected:
	FistType thinkFist()const;
};

#endif // COMPETITOR_H

