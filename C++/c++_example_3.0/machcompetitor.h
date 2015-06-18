#ifndef MACHCOMPETITOR_H
#define MACHCOMPETITOR_H

#include <competitor.h>

class MachCompetitor : public Competitor
{
public:
	MachCompetitor(const Competitor &competitor);
	~MachCompetitor(){}
protected:
	FistType thinkFist()const;
};

#endif // MACHCOMPETITOR_H

