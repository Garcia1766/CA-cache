//
// Created by garcia on 4/5/20.
//

#include "Cache.h"

Cache::Cache(int log_ways, int log_offset, int rs) {
    int log_index = 17 - log_ways - log_offset;
    int index = 1;
    for (int i = 0; i < log_index; ++i) {
        index = index << 1;
    }
    cacheGroup = new CacheGroup[index];
    for (int i = 0; i < index; ++i) {
        cacheGroup[i].SetWays(log_ways, rs);
    }
}

Cache::~Cache() {
    delete [] cacheGroup;
}

void Cache::SetShape(int log_ways, int log_offset, int rs) {
    int log_index = 17 - log_ways - log_offset;
    int index = 1;
    for (int i = 0; i < log_index; ++i) {
        index = index << 1;
    }
    cacheGroup = new CacheGroup[index];
    for (int i = 0; i < index; ++i) {
        cacheGroup[i].SetWays(log_ways, rs);
    }
}
