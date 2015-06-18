#include <machcompetitor.h>
#include <stdlib.h>
#include <time.h>

MachCompetitor::MachCompetitor(const Competitor &competitor)
	:Competitor(competitor)
{
	srand(time(0));//以当前的时间(unix时间戳)为随机种子
}
Competitor::FistType MachCompetitor::thinkFist()const
{
	//rand()取得随机数
	return (FistType)(rand() % 3 + 1);
}

