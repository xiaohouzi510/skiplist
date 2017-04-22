#ifndef _RANK_H_
#define _RANK_H_

#include <map>
#include <vector>
#include <string>

using namespace std;

struct zskiplist;

struct RankData
{
	RankData() : m_playerID(0),m_vip(0),m_timeStamp(0),m_score(0){}
	string m_name;
	int m_playerID;
	int m_vip;
	int m_timeStamp;
	double m_score;
};

int scoreCompare(const RankData *left,const RankData *right);

class Rank
{
public:
	Rank();

	~Rank();

public:
	RankData* insertData(int playerID,RankData *data);

	void redirectData();

	unsigned long getRank(int playerID);

	bool deleteData(int playerID);

	bool alterData(int playerID,const RankData &data);

	void release();

	void getPage(int page,vector<RankData*> &result);

	zskiplist* getSkipList();

	const RankData* getRankData(int playerID);

private:
	void init();

private:
	map<int,RankData*> m_datas;
	zskiplist *m_skipList;
};


#endif