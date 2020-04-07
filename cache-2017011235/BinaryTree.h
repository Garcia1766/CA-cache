//
// Created by garcia on 4/6/20.
//

#ifndef CACHE_2017011235_BINARYTREE_H
#define CACHE_2017011235_BINARYTREE_H


#include <cstdint>

#define Parent(i) (((i) - 1) >> 1)
#define LChild(i) (1 + ((i) << 1))
#define RChild(i) ((1 + (i)) << 1)

class BinaryTree {
public:
    uint8_t *bitree;

    BinaryTree(int log_ways = 3);
    ~BinaryTree();

    int LeafParent(int log_ways, int leaf);
    int LeafLChild(int log_ways, int parent);
    int LeafRChild(int log_ways, int parent);
    bool At(int ind);
    void ChangeTo(int ind, int val);

    int GetWhichReplace(int log_ways);
    void Update(int log_ways, int leaf);
};


#endif //CACHE_2017011235_BINARYTREE_H
