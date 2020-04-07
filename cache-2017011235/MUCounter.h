//
// Created by garcia on 4/7/20.
//

#ifndef CACHE_2017011235_MUCOUNTER_H
#define CACHE_2017011235_MUCOUNTER_H


#include <cstdint>

class MUCounter {
public:
    uint8_t *counter;

    MUCounter(int log_ways = 3);
    ~MUCounter();

    int GetCount(int num_ind);
    void SetCount(int num_ind, int ans);
    void IncCount(int num_ind);

    bool IsProtected(int log_ways, int num_ind);
};


#endif //CACHE_2017011235_MUCOUNTER_H
