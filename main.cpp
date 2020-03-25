#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include "Rank.h"
#include "SkipList.h"

using namespace std;

uint64_t gettime()
{
	uint64_t t;
	timeval tv;
	gettimeofday(&tv, NULL);
	t = (uint64_t)tv.tv_sec * 1000;
	t += tv.tv_usec / 1000;

	return t;
}

char g_names[26];

void init_name()
{
	int index = 0;
	for(char i = 97;i <= 122;++i)
	{
		g_names[index++] = i;
	}
}

void make_name(string &result)
{
	for(int i = 0;i < 5;++i)
	{
		int index = rand() % 26;
		result += g_names[index];
	}
}

int make_vip()
{
	return rand() % 10 + 1;
}

int make_score()
{
	return rand() % 100000000 + 1;
}

RankData* make_data(int playerID)
{
	RankData *result = new RankData();

	make_name(result->m_name);
	result->m_playerID  = playerID;
	result->m_vip       = make_vip();
	result->m_timeStamp = time(NULL);
	result->m_score     = make_score();

	return result;
}

void display(vector<RankData*> &datas)
{
	for(int i = 0;i < datas.size();++i)
	{
		printf("名字：%s 玩家ID：%d vip等级：%d 时间戳：%d 分数：%.0lf\n",datas[i]->m_name.data(),datas[i]->m_playerID,datas[i]->m_vip,datas[i]->m_timeStamp,datas[i]->m_score);
	}
}

void displayTime(uint64_t diffTime,const string &str)
{
	float t = (float)diffTime / 1000.0f;
	printf("%s %.4f 秒\n",str.data(),t);
}

int main(int argc,char *argv[])
{
	srand(time(NULL));
	init_name();
	int all = 1000000;
	Rank rank;
	int findID = rand() % all + 1;
	int deleteID = rand() % all + 1;

	//排行榜中增加all个玩家信息
	uint64_t makeStartTime = gettime();
	for(int i = 1;i <= all;++i)
	{
		rank.insertData(i,make_data(i));
	}
	uint64_t makeEndTime = gettime() - makeStartTime;

	const RankData *findData   = rank.getRankData(findID);
	const RankData *deleteData = rank.getRankData(deleteID);

	//将排行榜中玩家数据放入跳表
	uint64_t insertStartTime = gettime();
	rank.redirectData();
	uint64_t insertEndTime = gettime() - insertStartTime;

	//删除需要的时间
	uint64_t deleteStartTime = gettime();
	// rank.deleteData(deleteID);
	uint64_t deleteEndTime = gettime() - deleteStartTime;

	//查找需要时间
	uint64_t findStartTime = gettime();
	int rankIndex = rank.getRank(findID);
	uint64_t findEndTime = gettime() - findStartTime;

	//获取页数据
	vector<RankData*> datas;
	int page = 100;
	rank.getPage(page,datas);
	display(datas);
	printf("以上为第 %d 页玩家数据\n",page);

	zskiplist* curSkipList = rank.getSkipList();

	displayTime(makeEndTime,"生成排行榜用时:");
	displayTime(insertEndTime,"排行榜写入跳表:");
	displayTime(deleteEndTime,"删除一个数据用时:");
	displayTime(findEndTime,"查找一个数据用时:");

	printf("玩家 %d,名字 %s,排名 %d,跳表中总个数 %d\n",findData->m_playerID,findData->m_name.data(),rankIndex,curSkipList->length);

	return 0;
}