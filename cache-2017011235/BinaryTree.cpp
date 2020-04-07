//
// Created by garcia on 4/6/20.
//

#include <cstdio>
#include "BinaryTree.h"
#include "util.h"

BinaryTree::BinaryTree(int log_ways) {
    if (log_ways <= 3) {
        bitree = new uint8_t[1];
    } else {
        bitree = new uint8_t[log2val(log_ways - 3)];
    }
}

BinaryTree::~BinaryTree() {
    delete [] bitree;
}

int BinaryTree::LeafParent(int log_ways, int leaf) {
    int ways = log2val(log_ways);
    return (leaf >> 1) + ((ways >> 1) - 1);
}

int BinaryTree::LeafLChild(int log_ways, int parent) {
    int ways = log2val(log_ways);
    return (parent - ((ways >> 1) - 1)) << 1;
}

int BinaryTree::LeafRChild(int log_ways, int parent) {
    int ways = log2val(log_ways);
    return ((parent - ((ways >> 1) - 1)) << 1) + 1;
}

bool BinaryTree::At(int ind) {
    int num_chars = ind / 8;
    int in_char = ind % 8;
    return bitree[num_chars] & Pick1[in_char];
}

void BinaryTree::ChangeTo(int ind, int val) {
    int num_chars = ind / 8;
    int in_char = ind % 8;
    if (val == 0) {
        //printf("num_chars = %d, in_char = %d\n", num_chars, in_char);
        bitree[num_chars] = (bitree[num_chars] & Pick0[in_char]);
    } else {
        bitree[num_chars] = (bitree[num_chars] | Pick1[in_char]);
    }
    //printf("ChangeTo return\n");
}

int BinaryTree::GetWhichReplace(int log_ways) {
    int cur_ind = 0;
    for (int i = 0; i < log_ways - 1; ++i) {
        if (!this->At(cur_ind)) {  // 0, go left
            cur_ind = LChild(cur_ind);
        } else {                   // 1, go right
            cur_ind = RChild(cur_ind);
        }
    }
    if (!this->At(cur_ind)) {
        cur_ind = LeafLChild(log_ways, cur_ind);
    } else {
        cur_ind = LeafRChild(log_ways, cur_ind);
    }
    return cur_ind;
}

void BinaryTree::Update(int log_ways, int leaf) {
    int cur_ind = LeafParent(log_ways, leaf);
    //printf("cur_ind = %d\n", cur_ind);
    if (leaf % 2 == 0) {
        ChangeTo(cur_ind, 1);
    } else {
        ChangeTo(cur_ind, 0);
    }
    for (int i = 0; i < log_ways - 1; ++i) {
        if (cur_ind % 2) {
            ChangeTo(Parent(cur_ind), 1);
        } else {
            ChangeTo(Parent(cur_ind), 0);
        }
        cur_ind = Parent(cur_ind);
    }
    //printf("Update return\n");
}
