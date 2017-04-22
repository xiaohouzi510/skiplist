#include "Rank.h"
#include "SkipList.h"

#define PAGE_COUNT 50

int scoreCompare(const RankData *left,const RankData *right)
{
	if(left == right)
	{
		return 0;
	}

	if(left->m_score < right->m_score)
	{
		return -1;
	}
	else if(left->m_score == right->m_score)
	{
		if(left->m_vip < right->m_vip)
		{
			return -1;
		}
		else if(left->m_vip == right->m_vip)
		{
			return left->m_timeStamp < right->m_timeStamp ? -1 : 1;
		}
	}

	return 1;
}

Rank::Rank() :
m_skipList(NULL)
{
	init();
}

Rank::~Rank()
{
	release();
}

RankData* Rank::insertData(int playerID,RankData *data)
{
	m_datas[playerID] = data;
	return data;
}

void Rank::redirectData()
{
	map<int,RankData*>::iterator it;
	for(it = m_datas.begin();it != m_datas.end();++it)
	{
		zslInsert(m_skipList,it->second);
	}
}

unsigned long Rank::getRank(int playerID)
{
	map<int,RankData*>::iterator it = m_datas.find(playerID);
	if(it == m_datas.end())
	{
		return 0;
	}

	return zslGetRank(m_skipList,it->second);
}

bool Rank::deleteData(int playerID)
{
	map<int,RankData*>::iterator it = m_datas.find(playerID);
	if(it == m_datas.end())
	{
		return false;
	}

	zslDelete(m_skipList,it->second);
	delete it->second;
	m_datas.erase(it);

	return true;
}

bool Rank::alterData(int playerID,const RankData &data)
{
	if(!deleteData(playerID))
	{
		return false;
	}

	RankData *newData = new RankData();
	*newData = data;
	insertData(playerID,newData);

	return true;
}

void Rank::release()
{
	zslFree(m_skipList);
	map<int,RankData*>::iterator it;
	for(it = m_datas.begin();it != m_datas.end();++it)
	{
		delete it->second;
	}

	m_datas.clear();
}

void Rank::getPage(int page,vector<RankData*> &result)
{
	int endIndex   = page * PAGE_COUNT;
	int startIndex = endIndex - PAGE_COUNT + 1;
	int index = 1;
	zskiplistNode *node = m_skipList->header->level[0].forward;
	while(node != NULL)
	{
		if(index >= startIndex)
		{
			result.push_back(node->obj);
		}
		++index;
		node = node->level[0].forward;
		if(index > endIndex)
		{
			break;
		}
	}
}

zskiplist* Rank::getSkipList()
{
	return m_skipList;
}

const RankData* Rank::getRankData(int playerID)
{
	map<int,RankData*>::iterator it = m_datas.find(playerID);
	if(it == m_datas.end())
	{
		return NULL;
	}

	return it->second;
}

void Rank::init()
{
	m_skipList = zslCreate(scoreCompare);
}