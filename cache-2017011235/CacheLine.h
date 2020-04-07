//
// Created by garcia on 4/5/20.
//

#ifndef CACHE_2017011235_CACHELINE_H
#define CACHE_2017011235_CACHELINE_H


#include <cstdint>

class CacheLine {
public:
    uint8_t *data;

    CacheLine(int log_ways = 3);
    ~CacheLine();
    void SetDataSize(int log_ways);

    bool IsValid();
    bool IsDirty();
    uint64_t GetTag(int log_ways);

    void SetValid();
    void RemoveValid();
    void SetDirty();
    void RemoveDirty();
    void SetTag(int log_ways, uint64_t tag);
};


#endif //CACHE_2017011235_CACHELINE_H
