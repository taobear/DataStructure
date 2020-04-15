#include "RbTree.h"

#include <iostream>
using namespace std;

void initAVLTree(RbTree<int, int> &rbt)
{
    rbt.insert(1, 2);
    rbt.insert(0, 1);
    rbt.insert(5, 2);
    rbt.insert(3, 1);
    rbt.insert(2, 1);
    rbt.insert(7, 1);
}

int main(int argc, char **argv)
{

    RbTree<int, int> rbt;

    initAVLTree(rbt);

    return 0;
}