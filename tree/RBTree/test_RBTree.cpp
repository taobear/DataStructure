#include "RbTree.h"

#include <iostream>
using namespace std;

void initRbTree(RbTree<int, int> &rbt)
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

    initRbTree(rbt);

    rbt.inOrder();
    cout << endl;

    rbt.erase(1);
    rbt.inOrder();
    cout << endl;

    rbt.erase(2);
    rbt.inOrder();
    cout << endl;

    cout << "MAX:" << rbt.maximum() << endl;
    cout << "MIN:" << rbt.minimum() << endl;

    return 0;
}