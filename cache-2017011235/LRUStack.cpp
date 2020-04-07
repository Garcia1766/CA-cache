//
// Created by garcia on 4/6/20.
//

#include "LRUStack.h"
#include "util.h"

LRUStack::LRUStack(int log_ways) {
    if (log_ways <= 2) {
        stack = new uint8_t[1];
    } else {
        stack = new uint8_t[log_ways * log2val(log_ways - 3)];
    }
    int ways = log2val(log_ways);
    for (int i = 0; i < ways; ++i) {
        int res = i;
        for (int j = (i + 1) * log_ways - 1; j >= i * log_ways; --j) {
            if (((res >> 1) << 1) == res) {
                ChangeTo(j, 0);
            } else {
                ChangeTo(j, 1);
            }
            res = res >> 1;
        }
    }
}

LRUStack::~LRUStack() {
    delete [] stack;
}

int LRUStack::At(int ind) {
    int num_chars = ind >> 3;
    int in_char = ind  - ((ind >> 3) << 3);
    return (stack[num_chars] & Pick1[in_char]) ? 1 : 0;
}

void LRUStack::ChangeTo(int ind, int val) {
    int num_chars = ind >> 3;
    int in_char = ind  - ((ind >> 3) << 3);
    if (val == 0) {
        stack[num_chars] = (stack[num_chars] & Pick0[in_char]);
    } else {
        stack[num_chars] = (stack[num_chars] | Pick1[in_char]);
    }
}

int LRUStack::GetNum(int log_ways, int num_ind) {  // range(num_ind) = [0, ways)
    int base = 1, ans = 0;
    for (int i = (num_ind + 1) * log_ways - 1; i >= num_ind * log_ways; --i) {
        if (At(i)) {
            ans += base;
        }
        base = (base << 1);
    }
    return ans;
}

int LRUStack::GetWhichReplace(int log_ways) {
    int ways = log2val(log_ways);
    return GetNum(log_ways, ways - 1);
}

void LRUStack::Update(int log_ways, int num) {
    int ways = log2val(log_ways);
    for (int i = 0; i < ways; ++i) {
        if (GetNum(log_ways, i) == num) {
            for (int j = i * log_ways - 1; j >= 0; --j) {
                ChangeTo(j + log_ways, At(j));
            }
            int res = num;
            for (int j = log_ways - 1; j >= 0; --j) {
                if (((res >> 1) << 1) == res) {
                    ChangeTo(j, 0);
                } else {
                    ChangeTo(j, 1);
                }
                res = res >> 1;
            }
            break;
        }
    }
}
