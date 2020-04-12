#include "AVLTree.h"

#include <iostream>
using namespace std;

void initAVLTree(AVLTree<int, int> &avl)
{
    avl.put(1, 2);
    avl.put(0, 1);
    avl.put(5, 2);
    avl.put(3, 1);
    avl.put(2, 1);
    avl.put(7, 1);
}

int main(int argc, char **argv)
{

    AVLTree<int, int> avl;

    initAVLTree(avl);
    cout << "avl height: " << avl.height() << endl;

    avl.inOrder();
    cout << endl;

    avl.deleteMax();
    avl.inOrder();
    cout << endl;

    avl.deleteKey(3);
    avl.inOrder();

    cout << "MAX: " << avl.maximum() << endl;
    cout << "MIN: " << avl.minimum() << endl;

    return 0;
}