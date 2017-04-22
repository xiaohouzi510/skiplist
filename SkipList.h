#ifndef _SKIPLIST_H_
#define _SKIPLIST_H_

struct RankData;

typedef int (*skipCompare)(const RankData* left,const RankData *right);

struct zskiplistNode;

struct zskiplistLevel
{
    zskiplistNode *forward;
    unsigned int span;
};

/* ZSETs use a specialized version of Skiplists */
struct zskiplistNode
{
    RankData *obj;
    zskiplistNode *backward;
    zskiplistLevel level[1];
};

struct zskiplist
{
    zskiplistNode *header, *tail;
    unsigned long length;
    int level;
    skipCompare compareFun;
};

zskiplist *zslCreate(skipCompare compareFun);
zskiplistNode *zslInsert(zskiplist *zsl,RankData *obj);
int zslDelete(zskiplist *zsl,RankData *obj);
unsigned long zslGetRank(zskiplist *zsl,RankData *obj);
void zslFree(zskiplist *zsl);

#endif