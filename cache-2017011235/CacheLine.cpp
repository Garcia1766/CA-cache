//
// Created by garcia on 4/5/20.
//

#include <cstdio>
#include "CacheLine.h"

CacheLine::CacheLine(int log_ways) {
    if (log_ways <= 7) {
        data = new uint8_t[7];
    } else {
        data = new uint8_t[8];
    }
}

CacheLine::~CacheLine() {
    delete [] data;
}

void CacheLine::SetDataSize(int log_ways) {
    if (log_ways <= 7) {
        data = new uint8_t[7];
    } else {
        data = new uint8_t[8];
    }
}

bool CacheLine::IsValid() {
    return (data[0] & 0x80) != 0;
}

bool CacheLine::IsDirty() {
    return (data[0] & 0x40) != 0;
}

uint64_t CacheLine::GetTag(int log_ways) {
    int tag_len = 47 + log_ways;
    uint64_t tag = 0;
    if (tag_len <= 48) {
        for (int i = 6; i >= 1; i--) {
            tag += ((uint64_t) data[i]) << (8 * (6 - i));
        }
    } else if (tag_len <= 56) {
        for (int i = 6; i >= 1; i--) {
            tag += ((uint64_t) data[i]) << (8 * (6 - i));
        }
        tag += ((uint64_t) (data[0] & 0x3f)) << (8 * (6 - 0));
    } else {
        for (int i = 7; i >= 1; i--) {
            tag += ((uint64_t) data[i]) << (8 * (7 - i));
        }
        tag += ((uint64_t) (data[0] & 0x3f)) << (8 * (7 - 0));
    }
    return tag;
}

void CacheLine::SetValid() {
    data[0] = (data[0] | 0x80);
    //printf("set %lx\n", data[0]);
}

void CacheLine::RemoveValid() {
    data[0] = data[0] & 0x7f;
}

void CacheLine::SetDirty() {
    data[0] = data[0] | 0x40;
}

void CacheLine::RemoveDirty() {
    data[0] = data[0] & 0xbf;
}

void CacheLine::SetTag(int log_ways, uint64_t tag) {
    int tag_len = 47 + log_ways;
    if (tag_len <= 48) {
        for (int i = 6; i >= 1; i--) {
            data[i] = tag & 0x00ff;
            tag = tag >> 8;
        }
    } else if (tag_len <= 56) {
        for (int i = 6; i >= 1; i--) {
            data[i] = tag & 0x00ff;
            tag = tag >> 8;
        }
        data[0] = ((data[0] & 0xc0) | (tag & 0x00ff));
    } else {
        for (int i = 7; i >= 1; i--) {
            data[i] = tag & 0x00ff;
            tag = tag >> 8;
        }
        data[0] = ((data[0] & 0xc0) | (tag & 0x00ff));
    }
}


