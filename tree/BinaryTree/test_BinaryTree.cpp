#include "BinaryTree.h"

void initBinaryTree(BinaryTree<int, int> &bt)
{
    bt.put(1, 2);
    bt.put(0, 1);
    bt.put(5, 2);
    bt.put(3, 1);
    bt.put(2, 1);
    bt.put(7, 1);
}

int main(int argc, char **argv)
{

    BinaryTree<int, int> bt;

    initBinaryTree(bt);
    bt.inOrder();
    cout << endl;
    
    bt.deleteMax();
    bt.inOrder();
    cout << endl;

    bt.deleteKey(3);
    bt.inOrder();

    cout << "MAX: " << bt.maximum() << endl;
    cout << "MIN: " << bt.minimum() << endl;

    return 0;
}