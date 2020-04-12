#ifndef __BINARYTREE_H_
#define __BINARYTREE_H_

#include <cassert>
#include <iostream>
using namespace std;

/*
 * 一颗二叉查找树(BST)是一颗二叉树，
 * 其中每个节点都含有一个Comparable的键且每个节点的键都大于其左子树的任意结点的键而小于右子树的任意结点的键
 */

template <class Key, class Value>
struct BtNode {
    Key key;
    Value value;
    BtNode *left;
    BtNode *right;

    BtNode(const Key &key, const Value &value) {
        this->key = key;
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template <class Key, class Value>
class BinaryTree 
{
public:
    using MyBtNode = BtNode<Key, Value>;

public:
    BinaryTree();
    ~BinaryTree();

    int size() { return count_; }
    bool isEmpty() { return count_ == 0; }
    bool contain(const Key &key) { return get(key) != nullptr; }

    Value *get(const Key &key) { return get(root_, key); }
    void put(const Key &key, const Value &val) { root_ = put(root_, key, val); }

    Key minimum() {
        assert(count_ != 0);
        const MyBtNode *minNode = minimum(root_);
        return minNode->key;
    }
    Key maximum() {
        assert(count_ != 0);
        const MyBtNode *maxNode = maximum(root_);
        return maxNode->key;
    }

    void deleteMin() { if (root_ != nullptr) root_ = deleteMin(root_); }
    void deleteMax() { if (root_ != nullptr) root_ = deleteMax(root_); }

    void deleteKey(const Key &key) { root_ = deleteKey(root_, key); }

    void preOrder() { preOrder(root_); }
    void inOrder() { inOrder(root_); }
    void postOrder() { postOrder(root_); }

private:
    Value *get(MyBtNode *x, const Key &key);
    MyBtNode* put(MyBtNode *x, const Key &key, const Value &val);

    const MyBtNode *minimum(const MyBtNode *x);
    const MyBtNode *maximum(const MyBtNode *x);

    MyBtNode *deleteMin(MyBtNode *x);
    MyBtNode *deleteMax(MyBtNode *x);

    MyBtNode *deleteKey(MyBtNode *x, const Key &key);

    void destroy(MyBtNode *node);

    void preOrder(MyBtNode *node);
    void inOrder(MyBtNode *node);
    void postOrder(MyBtNode *node);

private:
    MyBtNode *root_;
    int count_;
};

template <class Key, class Value>
BinaryTree<Key, Value>::BinaryTree()
{
    root_ = nullptr;
    count_ = 0;
}

template <class Key, class Value>
BinaryTree<Key, Value>::~BinaryTree()
{
    destroy(root_);
}

template <class Key, class Value>
Value *BinaryTree<Key, Value>::get(MyBtNode *x, const Key &key)
{
    if (x == nullptr) {
        return nullptr;
    }

    if (x->key == key) {
        return x->value;
    }

    if (key < x->key) {
        return get(x->left, key);
    } else {
        return get(x->right, key);
    }
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::MyBtNode*
BinaryTree<Key, Value>::put(MyBtNode *x, const Key &key, const Value &val)
{
    if (x == nullptr) {
        count_++;
        return new MyBtNode(key, val);
    }

    if (key < x->key) { // in left sub-tree
        x->left = put(x->left, key, val);
    } else if (key > x->key) {
        x->right = put(x->right, key, val);
    } else {
        x->value = val;
    }

    return x;
}

template <class Key, class Value>
const typename BinaryTree<Key, Value>::MyBtNode *
BinaryTree<Key, Value>::minimum(const MyBtNode *x)
{
    if (x == nullptr) {
        return x;
    }

    if (x->left == nullptr) {
        return x;
    }

    return minimum(x->left);
}

template <class Key, class Value>
const typename BinaryTree<Key, Value>::MyBtNode *
BinaryTree<Key, Value>::maximum(const MyBtNode *x)
{
    if (x == nullptr) {
        return x;
    }

    if (x->right == nullptr) {
        return x;
    }

    return maximum(x->right);
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::MyBtNode *
BinaryTree<Key, Value>::deleteMin(MyBtNode *x)
{
    if (x == nullptr) {
        return x;
    }

    if (x->left == nullptr) {
        MyBtNode *rightNode = x->right;
        delete x;
        count_--;
        return rightNode;
    }

    x->left = deleteMin(x->left);
    return x;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::MyBtNode *
BinaryTree<Key, Value>::deleteMax(MyBtNode *x)
{
    if (x == nullptr) {
        return x;
    }

    if (x->right == nullptr) {
        MyBtNode *leftNode = x->left;
        delete x;
        count_--;
        return leftNode;
    }

    x->right = deleteMax(x->right);
    return x;
}

/**
 * @brief 删除掉以node为根的二分搜索树中键值为key的结点，返回删除结点后新的二分搜索树的根
 */
template <class Key, class Value>
typename BinaryTree<Key, Value>::MyBtNode *
BinaryTree<Key, Value>::deleteKey(MyBtNode *x, const Key &key)
{
    if (x == nullptr) {
        return x;
    }

    if (key < x->key) {
        x->left = deleteKey(x->left, key);
    } else if (key > x->key) {
        x->right = deleteKey(x->right, key);
    } else {
        if (x->left == nullptr) {
            MyBtNode *rightNode = x->right;
            delete x;
            count_--;
            return rightNode; 
        }
        if (x->right == nullptr) {
            MyBtNode *leftNode = x->left;
            delete x;
            count_--;
            return leftNode; 
        }

        MyBtNode *successor = new MyBtNode(*minimum(x->right));
        
        successor->right = deleteMin(x->right);
        successor->left = x->left;

        delete x;
        return successor;
    }

    return x;
}

template <class Key, class Value>
void BinaryTree<Key, Value>::destroy(MyBtNode *x)
{
    if (x == nullptr) {
        return;
    }

    if (x->left != nullptr) {
        destroy(x->left);
    }

    if (x->right != nullptr) {
        destroy(x->right);
    }

    count_--;
    delete x;
}

template <class Key, class Value>
void BinaryTree<Key, Value>::preOrder(MyBtNode *x)
{
    if (x == nullptr) {
        return;
    }

    cout << "(" << x->key << ", " << x->value << ")" << endl;
    preOrder(x->left);
    preOrder(x->right);
}

template <class Key, class Value>
void BinaryTree<Key, Value>::inOrder(MyBtNode *x)
{
    if (x == nullptr) {
        return;
    }

    inOrder(x->left);
    cout << "(" << x->key << ", " << x->value << ")" << endl;
    inOrder(x->right);
}

template <class Key, class Value>
void BinaryTree<Key, Value>::postOrder(MyBtNode *x)
{
    if (x == nullptr) {
        return;
    }

    postOrder(x->left);
    postOrder(x->right);
    cout << "(" << x->key << ", " << x->value << ")" << endl;
}

#endif