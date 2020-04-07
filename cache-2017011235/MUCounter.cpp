//
// Created by garcia on 4/7/20.
//

#include "MUCounter.h"
#include "util.h"

MUCounter::MUCounter(int log_ways) {
    if (log_ways <= 1) {
        counter = new uint8_t[1];
    } else {
        counter = new uint8_t[log2val(log_ways) >> 1];
    }
}

MUCounter::~MUCounter() {
    delete [] counter;
}

int MUCounter::GetCount(int num_ind) {
    int num_chars = num_ind >> 1;
    int ans = 0;
    if ((num_chars << 1) == num_ind) {  // left 4 bits
        for (int i = 0; i < 4; ++i) {
            if(counter[num_chars] & Pick1[3 - i]) {
                ans = ans + (1 << i);
            }
        }
    } else {  // right 4 bits
        for (int i = 0; i < 4; ++i) {
            if(counter[num_chars] & Pick1[7 - i]) {
                ans = ans + (1 << i);
            }
        }
    }
    return ans;
}

void MUCounter::SetCount(int num_ind, int ans) {
    int num_chars = num_ind >> 1;
    if ((num_chars << 1) == num_ind) {
        for (int i = 0; i < 4; ++i) {
            if (((ans >> 1) << 1) == ans) {  // ans % 2 == 0
                counter[num_chars] = counter[num_chars] & Pick0[3 - i];
            } else {  // ans % 2 == 1
                counter[num_chars] = counter[num_chars] | Pick1[3 - i];
            }
            ans = ans >> 1;
        }
    } else {
        for (int i = 0; i < 4; ++i) {
            if (((ans >> 1) << 1) == ans) {  // ans % 2 == 0
                counter[num_chars] = counter[num_chars] & Pick0[7 - i];
            } else {  // ans % 2 == 1
                counter[num_chars] = counter[num_chars] | Pick1[7 - i];
            }
            ans = ans >> 1;
        }
    }
}

void MUCounter::IncCount(int num_ind) {
    int ans = GetCount(num_ind) + 1;
    if (ans >= 16) ans = 8;
    SetCount(num_ind, ans);
}

bool MUCounter::IsProtected(int log_ways, int num_ind) {
    int protected_size = log2val(log_ways) >> 1;
    int tar_cnt = GetCount(num_ind);
    int geq = 0;
    for (int i = 0; i < log2val(log_ways); ++i) {
        if (GetCount(i) >= tar_cnt) {
            geq++;
        }
    }
    return geq <= protected_size;
}
