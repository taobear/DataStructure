#ifndef __AVLTREE_H_
#define __AVLTREE_H_
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace std;

/**
 * AVL树的基本性质
 * 1. 满足二分搜索树的所有性质
 * 2. 带有平衡条件，每个结点的左右子树的高度之差的绝对值最多为1
 * 3. 平衡因子 = |左子树长度 - 右子树长度|
 */
template <class Key, class Value>
struct AVLTreeNode {
    Key key;
    Value value;
    AVLTreeNode *left;
    AVLTreeNode *right;
    int height;

    AVLTreeNode(const Key &key, const Value &value) {
        this->key = key;
        this->value = value;
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template <class Key, class Value>
class AVLTree {
public:
    using MyAVLTreeNode = AVLTreeNode<Key, Value>;
    
    AVLTree();
    ~AVLTree();

public:
    int size() { return count_; }
    int height() { return root_->height; }
    bool isEmpty() { return count_ == 0; }
    bool contain(const Key &key) { return get(key) != nullptr; }

    Value *get(const Key &key) { return get(root_, key); }
    void put(const Key &key, const Value &val) { root_ = put(root_, key, val); }

    Key minimum() {
        assert(count_ != 0);
        const MyAVLTreeNode *minNode = minimum(root_);
        return minNode->key;
    }
    Key maximum() {
        assert(count_ != 0);
        const MyAVLTreeNode *maxNode = maximum(root_);
        return maxNode->key;
    }

    void deleteMin() {
        if (root_ != nullptr)
            root_ = deleteMin(root_);
    }
    void deleteMax() {
        if (root_ != nullptr)
            root_ = deleteMax(root_);
    }

    void deleteKey(const Key &key) { root_ = deleteKey(root_, key); }

    void preOrder() { preOrder(root_); }
    void inOrder() { inOrder(root_); }
    void postOrder() { postOrder(root_); }

private:
    MyAVLTreeNode *put(MyAVLTreeNode *root, const Key &key, const Value &val);
    Value *get(MyAVLTreeNode *root, const Key &key);

    MyAVLTreeNode *deleteMin(MyAVLTreeNode *x);
    MyAVLTreeNode *deleteMax(MyAVLTreeNode *x);

    MyAVLTreeNode *deleteKey(MyAVLTreeNode *x, const Key &key);

    const MyAVLTreeNode *minimum(const MyAVLTreeNode *x);
    const MyAVLTreeNode *maximum(const MyAVLTreeNode *x);

    void destroy(MyAVLTreeNode *node);

    void preOrder(MyAVLTreeNode *node);
    void inOrder(MyAVLTreeNode *node);
    void postOrder(MyAVLTreeNode *node);

private:
    MyAVLTreeNode *rebalance(MyAVLTreeNode *x);                                                                                                                                                                                                    getNodeHeight(MyAVLTreeNode *x) { return x != nullptr ? x->height : 0; }

    MyAVLTreeNode *LL(MyAVLTreeNode *root);
    MyAVLTreeNode *LR(MyAVLTreeNode *root);
    MyAVLTreeNode *RL(MyAVLTreeNode *root);
    MyAVLTreeNode *RR(MyAVLTreeNode *root);

    MyAVLTreeNode *leftRotate(MyAVLTreeNode *root);
    MyAVLTreeNode *rightRotate(MyAVLTreeNode *root);

private:
    MyAVLTreeNode *root_;
    int count_;
};

template <class Key, class Value>
AVLTree<Key, Value>::AVLTree()
{
    root_ = nullptr;
    count_ = 0;
}

template <class Key, class Value>
AVLTree<Key, Value>::~AVLTree()
{
    destroy(root_);
}

template <class Key, class Value>
Value *AVLTree<Key, Value>::get(MyAVLTreeNode *x, const Key &key)
{
    if (x == nullptr) {
        return nullptr;
    }

    if (x->key == key) {
        return x->value;
    }

    if (key < x->key) {
        return get(x->left, key);
    }
    else {
        return get(x->right, key);
    }
}

template <class Key, class Value>
const typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::minimum(const MyAVLTreeNode *x)
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
const typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::maximum(const MyAVLTreeNode *x)
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
void AVLTree<Key, Value>::destroy(MyAVLTreeNode *x)
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
void AVLTree<Key, Value>::preOrder(MyAVLTreeNode *x)
{
    if (x == nullptr) {
        return;
    }

    cout << "(" << x->key << ", " << x->value << ")" << endl;
    preOrder(x->left);
    preOrder(x->right);
}

template <class Key, class Value>
void AVLTree<Key, Value>::inOrder(MyAVLTreeNode *x)
{
    if (x == nullptr) {
        return;
    }

    inOrder(x->left);
    cout << "(" << x->key << ", " << x->value << ")" << endl;
    inOrder(x->right);
}

template <class Key, class Value>
void AVLTree<Key, Value>::postOrder(MyAVLTreeNode *x)
{
    if (x == nullptr) {
        return;
    }

    postOrder(x->left);
    postOrder(x->right);
    cout << "(" << x->key << ", " << x->value << ")" << endl;
}

/**
 * 新加入的结点在左子树的左子树上，导致左子树偏高
 *               O(root, x+3, diff=2)
 *       O(x+2)        O(x)
 *   O(x+1)   O(x)
 */
template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode*
AVLTree<Key, Value>::LL(MyAVLTreeNode *root)
{
    return rightRotate(root);;
}

/**
 * 新加入的结点在左子树的右子树上，导致左子树偏高
 *              O(root, x+3, diff = 2)
 *        O(x+2)          O(x)
 *   O(x)     O(x+1)
 */
template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::LR(MyAVLTreeNode *root)
{
    root->left = leftRotate(root->left);
    return LL(root);
}

/**
 * 新加入的结点在右子树的右子树上，导致右子树偏高
 *               O(root, x+3, diff=2)
 *       O(x)          O(x+2)
 *                 O(x)   O(x+1)
 */
template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::RR(MyAVLTreeNode *root)
{
    return leftRotate(root);
}

/**
 * 新加入的结点在右子树的左子树上，导致右子树偏高
 *               O(root, x+3, diff=2)
 *       O(x)          O(x+2)
 *                 O(x+1)   O(x)
 */
template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::RL(MyAVLTreeNode *root)
{
    root->right = rightRotate(root->right);
    return RR(root);
}

/**
 * 按照root为根结点，进行左旋
 *   O(root)                     O(newRoot)
 *       O           ==>  O(oldRoot)     O
 *           O
 */
template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::leftRotate(MyAVLTreeNode *root)
{
    if (root == nullptr || root->right == nullptr) {
        return root;
    }

    MyAVLTreeNode *oldRoot = root;
    MyAVLTreeNode *newRoot = root->right;

    oldRoot->right = newRoot->left;
    newRoot->left = oldRoot;

    oldRoot->height = max(getNodeHeight(oldRoot->left), getNodeHeight(oldRoot->right)) + 1;
    newRoot->height = max(getNodeHeight(newRoot->left), getNodeHeight(newRoot->right)) + 1;

    return newRoot;
}

/**
 * 按照root为根结点，进行左旋
 *              O(root)             O(newRoot)
 *         O            ==>    O         O(oldRoot)
 *    O
 */
template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::rightRotate(MyAVLTreeNode *root)
{
    if (root == nullptr || root->left == nullptr) {
        return root;
    }

    MyAVLTreeNode *oldRoot = root;
    MyAVLTreeNode *newRoot = root->left;

    oldRoot->left = newRoot->right;
    newRoot->right = oldRoot;

    oldRoot->height = max(getNodeHeight(oldRoot->left), getNodeHeight(oldRoot->right)) + 1;
    newRoot->height = max(getNodeHeight(newRoot->left), getNodeHeight(newRoot->right)) + 1;

    return newRoot;
}

template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::rebalance(MyAVLTreeNode *x)
{
    if (x == nullptr) {
        return x;
    }

    MyAVLTreeNode *leftNode = x->left;
    MyAVLTreeNode *rightNode = x->right;
    MyAVLTreeNode *newRoot = x; // default

    int leftHight = getNodeHeight(leftNode);
    int rightHight = getNodeHeight(rightNode);

    if (leftHight - rightHight > 1) {
        if (getNodeHeight(leftNode->left) > getNodeHeight(leftNode->right)) {
            newRoot = LL(x);
        } else {
            newRoot = LR(x);
        }
    } else if (leftHight - rightHight < -1) {
        if (getNodeHeight(rightNode->right) > getNodeHeight(rightNode->left)) {
            newRoot = RR(x);
        } else {
            newRoot = RL(x);
        }
    } else {
        newRoot->height = max(getNodeHeight(newRoot->left), getNodeHeight(newRoot->right)) + 1;        
    }

    return newRoot;
}

template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::put(MyAVLTreeNode *x, const Key &key, const Value &val)
{
    if (x == nullptr) {
        count_++;
        return new MyAVLTreeNode(key, val);
    }

    MyAVLTreeNode *newRoot = nullptr;
    if (key < x->key) {
        x->left = put(x->left, key, val); // x的左子树必然有结点
        newRoot = rebalance(x);
    } else if (key > x->key) {
        x->right = put(x->right, key, val);
        newRoot = rebalance(x);
    } else {
        newRoot = x;
        newRoot->value = val;
    }

    return newRoot;
}

template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::deleteMin(MyAVLTreeNode *x)
{
    if (x == nullptr) {
        return x;
    }

    if (x->left == nullptr) {
        MyAVLTreeNode *rightNode = x->right;
        delete x;
        count_ --;
        return rightNode;
    }

    x->left = deleteMin(x->left);
    return rebalance(x);
}

template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::deleteMax(MyAVLTreeNode *x)
{
    if (x == nullptr) {
        return x;
    }

    if (x->right == nullptr) {
        MyAVLTreeNode *leftNode = x->left;
        delete x;
        count_ --;
        return leftNode;
    }

    x->right = deleteMax(x->right);
    return rebalance(x);
}

template <class Key, class Value>
typename AVLTree<Key, Value>::MyAVLTreeNode *
AVLTree<Key, Value>::deleteKey(MyAVLTreeNode *x, const Key &key)
{
    if (x == nullptr) {
        return x;
    }

    MyAVLTreeNode *newX = nullptr;
    if (key < x->key) {
        x->left = deleteKey(x->left, key);
        newX = rebalance(x);
    } else if (key > x->key) {
        x->right = deleteKey(x->right, key);
        newX = rebalance(x);
    } else {
        if (x->left == nullptr) {
            newX = x->right;
            delete x;
            count_ --;
        } else if (x->right == nullptr) {
            newX = x->left;
            delete x;
            count_ --;
        } else {
            MyAVLTreeNode *successor = new MyAVLTreeNode(*minimum(x->right));

            successor->right = deleteMin(x->right);
            successor->left = x->left;
            successor->height = max(getNodeHeight(successor->left), getNodeHeight(successor->right)) + 1;

            newX = rebalance(successor);
            delete x;
        }
    }

    return newX;
}

#endif