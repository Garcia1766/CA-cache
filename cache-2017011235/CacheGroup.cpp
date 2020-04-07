//
// Created by garcia on 4/5/20.
//

#include <cstdlib>
#include <cstdio>
#include "CacheGroup.h"
#include "util.h"

CacheGroup::CacheGroup(int log_ways, int rs) {
    int ways = log2val(log_ways);
    cacheLine = new CacheLine[ways];
    if (log_ways > 7) {
        for (int i = 0; i < ways; ++i) {
            cacheLine[i].SetDataSize(log_ways);
        }
    }
    if (log_ways > 0) {
        if (rs == 2) {
            binaryTree = new BinaryTree(log_ways);
        } else if (rs == 1) {
            lruStack = new LRUStack(log_ways);
        }
    }
}

CacheGroup::~CacheGroup() {
    delete [] cacheLine;
}

void CacheGroup::SetWays(int log_ways, int rs) {
    int ways = log2val(log_ways);
    cacheLine = new CacheLine[ways];
    if (log_ways > 7) {
        for (int i = 0; i < ways; ++i) {
            cacheLine[i].SetDataSize(log_ways);
        }
    }
    if (log_ways > 0) {
        if (rs == 2) {
            binaryTree = new BinaryTree(log_ways);
        } else if (rs == 1) {
            lruStack = new LRUStack(log_ways);
        }
    }
}

bool CacheGroup::Read(int log_ways, uint64_t tag, int rs) {
    int ways = log2val(log_ways);
    int last_invalid = -1;
    for (int i = 0; i < ways; ++i) {
        if (!cacheLine[i].IsValid()) {
            last_invalid = i;
        } else if (cacheLine[i].GetTag(log_ways) == tag) {
            if (log_ways > 0) {
                if (rs == 2) {
                    binaryTree->Update(log_ways, i);
                } else if (rs == 1) {
                    lruStack->Update(log_ways, i);
                }
            }
            return true;
        }
    }
    if (last_invalid >= 0) {
        cacheLine[last_invalid].SetValid();
        cacheLine[last_invalid].RemoveDirty();
        cacheLine[last_invalid].SetTag(log_ways, tag);
        if (log_ways > 0) {
            if (rs == 2) {
                binaryTree->Update(log_ways, last_invalid);
            } else if (rs == 1) {
                lruStack->Update(log_ways, last_invalid);
            }
        }
    } else {
        int which_replace = random() % ways;
        if (log_ways > 0) {
            if (rs == 2) {
                which_replace = binaryTree->GetWhichReplace(log_ways);
                binaryTree->Update(log_ways, which_replace);
            } else if (rs == 1) {
                which_replace = lruStack->GetWhichReplace(log_ways);
                lruStack->Update(log_ways, which_replace);
            }
        }
        //printf("which_replace = %d\n", which_replace);
        cacheLine[which_replace].RemoveDirty();
        cacheLine[which_replace].SetTag(log_ways, tag);
    }
    return false;
}

bool CacheGroup::Write(int log_ways, uint64_t tag, int wh, int wm, int rs) {
    int ways = log2val(log_ways);
    int last_invalid = -1;
    for (int i = 0; i < ways; ++i) {
        if (!cacheLine[i].IsValid()) {
            last_invalid = i;
        } else if (cacheLine[i].GetTag(log_ways) == tag) {
            if (wh == 0) {
                cacheLine[i].SetDirty();
            }
            if (log_ways > 0) {
                if (rs == 2) {
                    binaryTree->Update(log_ways, i);
                } else if (rs == 1) {
                    lruStack->Update(log_ways, i);
                }
            }
            return true;
        }
    }
    if (last_invalid >= 0) {
        if (wm == 0) {
            cacheLine[last_invalid].SetValid();
            cacheLine[last_invalid].SetDirty();
            cacheLine[last_invalid].SetTag(log_ways, tag);
        }
        if (log_ways > 0) {
            if (rs == 2) {
                binaryTree->Update(log_ways, last_invalid);
            } else if (rs == 1) {
                lruStack->Update(log_ways, last_invalid);
            }
        }
    } else {
        if (wm == 0) {
            int which_replace = random() % ways;
            if (log_ways > 0) {
                if (rs == 2) {
                    which_replace = binaryTree->GetWhichReplace(log_ways);
                    binaryTree->Update(log_ways, which_replace);
                } else if (rs == 1) {
                    which_replace = lruStack->GetWhichReplace(log_ways);
                    lruStack->Update(log_ways, which_replace);
                }
            }
            //printf("which_replace = %d\n", which_replace);
            cacheLine[which_replace].SetDirty();
            cacheLine[which_replace].SetTag(log_ways, tag);
        }
    }
    return false;
}
