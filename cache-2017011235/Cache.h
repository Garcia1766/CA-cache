//
// Created by garcia on 4/5/20.
//

#ifndef CACHE_2017011235_CACHE_H
#define CACHE_2017011235_CACHE_H

#include "CacheGroup.h"

class Cache {
public:
    CacheGroup * cacheGroup;

    Cache(int log_ways = 3, int log_offset = 3, int rs = 0);
    ~Cache();
    void SetShape(int log_ways, int log_offset, int rs = 0);
};


#endif //CACHE_2017011235_CACHE_H
