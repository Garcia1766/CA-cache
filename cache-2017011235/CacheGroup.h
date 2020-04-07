//
// Created by garcia on 4/5/20.
//

#ifndef CACHE_2017011235_CACHEGROUP_H
#define CACHE_2017011235_CACHEGROUP_H


#include "CacheLine.h"
#include "BinaryTree.h"
#include "LRUStack.h"

class CacheGroup {
public:
    CacheLine *cacheLine;
    BinaryTree *binaryTree;
    LRUStack *lruStack;

    CacheGroup(int log_ways = 3, int rs = 0);
    ~CacheGroup();
    void SetWays(int log_ways, int rs = 0);

    bool Read(int log_ways, uint64_t tag, int rs = 0);  // 1->hit, 0->miss
    bool Write(int log_ways, uint64_t tag, int wh = 0, int wm = 0, int rs = 0);
};


#endif //CACHE_2017011235_CACHEGROUP_H
