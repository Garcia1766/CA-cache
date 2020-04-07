//
// Created by garcia on 4/6/20.
//

#ifndef CACHE_2017011235_LRUSTACK_H
#define CACHE_2017011235_LRUSTACK_H


#include <cstdint>

class LRUStack {
public:
    uint8_t *stack;

    LRUStack(int log_ways = 3);
    ~LRUStack();

    int At(int ind);
    void ChangeTo(int ind, int val);

    int GetNum(int log_ways, int num_ind);
    int GetWhichReplace(int log_ways);
    void Update(int log_ways, int num);
};


#endif //CACHE_2017011235_LRUSTACK_H
