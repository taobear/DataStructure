#ifndef __RBTREE_H_
#define __RBTREE_H_

#include <cassert>
#include <iostream>

using namespace std;

enum Color {
    COLER_RED = 0,
    COLER_BLACK = 1,
    COLER_BOTTOM
};

template<class Key, class Value>
struct RbNode {
    Key key;
    Value val;
    RbNode *left;
    RbNode *right;
    RbNode *parent;
    Color color;

    RbNode(const Key &key, const Value &val, const Color color) {
        this->key = key;
        this->val = val;
        this->color = color;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
};

template <class Key, class Value>
class RbTree {
public:
    using MyRbNode = RbNode<Key, Value>;

    RbTree();
    ~RbTree();

public:
    void insert(const Key &key, const Value &val);
    void erase(const Key &key);

    int size() { return count_; }
    bool isEmpty() { return count_ == 0; }

    Key minimum() {
        assert(count_ != 0);
        const MyRbNode *minNode = minimum(root_);
        return minNode->key;
    }
    Key maximum() {
        assert(count_ != 0);
        const MyRbNode *maxNode = maximum(root_);
        return maxNode->key;
    }

    void preOrder() { preOrder(root_); }
    void inOrder() { inOrder(root_); }
    void postOrder() { postOrder(root_); }

private:
    void insert(MyRbNode *x, const Key &key, const Value &val);
    void erase(MyRbNode *x, const Key &key);

    MyRbNode *minimum(MyRbNode *x);
    MyRbNode *maximum(MyRbNode *x);

    MyRbNode *deleteMin(MyRbNode *x);
    MyRbNode *deleteMax(MyRbNode *x);

    void preOrder(MyRbNode *node);
    void inOrder(MyRbNode *node);
    void postOrder(MyRbNode *node);

private:
    void eraseFixUp(MyRbNode *x);
    void insertFixUp(MyRbNode *x);

    MyRbNode *eraseFixUpLeftNode(MyRbNode *x);
    MyRbNode *eraseFixUpRightNode(MyRbNode *x);

    void leftRotateWithParent(MyRbNode *x);
    void rightRotateWithParent(MyRbNode *x);

    void transplant(MyRbNode *oldNode, MyRbNode *newNode);

    MyRbNode *leftRotate(MyRbNode *x);
    MyRbNode *rightRotate(MyRbNode *x);

    void flipColor(MyRbNode *x);

    void destroy(MyRbNode *x);

    bool isRed(MyRbNode *x) { return !(x == nullptr || x->color != COLER_RED); }

private:
    MyRbNode *root_;
    int count_;
};

template <class Key, class Value>
RbTree<Key, Value>::RbTree()
{
    root_ = nullptr;
    count_ = 0;
}

template <class Key, class Value>
RbTree<Key, Value>::~RbTree()
{
    destroy(root_);
}

template <class Key, class Value>
void RbTree<Key, Value>::destroy(MyRbNode *x)
{
    if (x == nullptr)
    {
        return;
    }

    if (x->left != nullptr)
    {
        destroy(x->left);
    }

    if (x->right != nullptr)
    {
        destroy(x->right);
    }

    count_--;
    delete x;
}

template <class Key, class Value>
typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::minimum(MyRbNode *x)
{
    if (x == nullptr)
    {
        return x;
    }

    if (x->left == nullptr)
    {
        return x;
    }

    return minimum(x->left);
}

template <class Key, class Value>
typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::maximum(MyRbNode *x)
{
    if (x == nullptr)
    {
        return x;
    }

    if (x->right == nullptr)
    {
        return x;
    }

    return maximum(x->right);
}

template <class Key, class Value>
void RbTree<Key, Value>::preOrder(MyRbNode *x)
{
    if (x == nullptr)
    {
        return;
    }

    cout << "(" << x->key << ", " << x->value << ")" << endl;
    preOrder(x->left);
    preOrder(x->right);
}

template <class Key, class Value>
void RbTree<Key, Value>::inOrder(MyRbNode *x)
{
    if (x == nullptr)
    {
        return;
    }

    inOrder(x->left);
    cout << "(" << x->key << ", " << x->val << ")" << endl;
    inOrder(x->right);
}

template <class Key, class Value>
void RbTree<Key, Value>::postOrder(MyRbNode *x)
{
    if (x == nullptr)
    {
        return;
    }

    postOrder(x->left);
    postOrder(x->right);
    cout << "(" << x->key << ", " << x->val << ")" << endl;
}


/**
 * @brief transplant因为只在删除中有调用，该节点只有右节点没有左节点
 */
template <class Key, class Value>
void RbTree<Key, Value>::transplant(MyRbNode *oldNode, MyRbNode *newNode)
{
    if (oldNode->parent == nullptr) {
        root_ = newNode;
    } else {
        if (oldNode->parent->left == oldNode) {
            oldNode->parent->left = newNode;
        } else {
            oldNode->parent->right = newNode;
        }

        if (newNode != nullptr && newNode->parent->right != nullptr) {
            newNode->parent->right = newNode->right;
        }
    }

    if (newNode != nullptr) {
        newNode->parent = oldNode->parent;
        newNode->left = oldNode->left;
        newNode->right = oldNode->right;
    }
}

template <class Key, class Value>
typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::leftRotate(MyRbNode *x)
{
    assert(x != nullptr || x->right != nullptr);

    MyRbNode *oldRoot = x;
    MyRbNode *newRoot = x->right;

    oldRoot->right = newRoot->left;
    newRoot->left = oldRoot;

    /* 旋转并改变链接的颜色 */
    Color oldRootColor = oldRoot->color;
    oldRoot->color = newRoot->color;
    newRoot->color = oldRootColor;

    newRoot->parent = oldRoot->parent;
    oldRoot->parent = newRoot;

    return newRoot;
}

template <class Key, class Value>
typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::rightRotate(MyRbNode *x)
{
    assert(x != nullptr || x->left != nullptr);
    
    MyRbNode *oldRoot = x;
    MyRbNode *newRoot = x->left;

    oldRoot->left = newRoot->right;
    newRoot->right = oldRoot;

    /* 旋转并改变链接的颜色 */
    Color oldRootColor = oldRoot->color;
    oldRoot->color = newRoot->color;
    newRoot->color = oldRootColor;

    newRoot->parent = oldRoot->parent;
    oldRoot->parent = newRoot;

    return newRoot;
}

template <class Key, class Value>
void RbTree<Key, Value>::leftRotateWithParent(MyRbNode *x)
{
    if (x == nullptr) {
        return;
    }

    MyRbNode *p = x->parent;
    if (p == nullptr) {
        root_ = leftRotate(x);
    } else {
        if (p->left == x) {
            p->left = leftRotate(x);
        } else {
            p->right = leftRotate(x);
        }
    }
}

template <class Key, class Value>
void RbTree<Key, Value>::rightRotateWithParent(MyRbNode *x)
{
    if (x == nullptr) {
        return;
    }

    MyRbNode *p = x->parent;
    if (p == nullptr) {
        root_ = rightRotate(x);
    } else {
        if (p->left == x) {
            p->left = rightRotate(x);
        } else {
            p->right = rightRotate(x);
        }
    }
}

template <class Key, class Value>
void RbTree<Key, Value>::flipColor(MyRbNode *x)
{
    assert(x != nullptr);
    assert(x->left != nullptr && x->right != nullptr);

    x->color = COLER_RED;
    x->left->color = COLER_BLACK;
    x->right->color = COLER_BLACK;
}

template <class Key, class Value>
void RbTree<Key, Value>::insertFixUp(MyRbNode *x)
{
    while (isRed(x->parent)) {
        if (x->parent == x->parent->parent->left) {
            MyRbNode *uncle = x->parent->parent->right;
            if (isRed(uncle)) {
                flipColor(x->parent->parent);
                x = x->parent->parent;
            } else {
                if (x->parent->right == x) {
                    leftRotateWithParent(x->parent);
                    x = x->left;
                }
                rightRotateWithParent(x->parent->parent);
            }
        } else {
            MyRbNode *uncle = x->parent->parent->right;
            if (isRed(uncle)) {
                flipColor(x->parent->parent);
                x = x->parent->parent;
            } else {
                if (x->parent->left == x) {
                    rightRotateWithParent(x->parent);
                    x = x->parent;
                }
                leftRotateWithParent(x->parent->parent);
            }
        }
    }

    root_->color = COLER_BLACK;
}

template <class Key, class Value>
void RbTree<Key, Value>::insert(MyRbNode *x, const Key &key, const Value &val)
{
    MyRbNode *par = nullptr;
    MyRbNode *cur = root_;

    while (cur != nullptr) {
        par = cur;
        if (key > cur->key) {
            cur = cur->right;
        } else if (key < cur->key) {
            cur = cur->left;
        } else {
            cur->val = val;
            return;
        }
    }
    
    MyRbNode *newNode = new MyRbNode(key, val, COLER_RED);
    count_++;

    newNode->parent = par;
    if (par == nullptr) {
        root_ = newNode;
    } else if (key < par->key) {
        par->left = newNode;
    } else {
        par->right = newNode;
    }

    insertFixUp(newNode);
}

template <class Key, class Value>
void RbTree<Key, Value>::insert(const Key &key, const Value &val)
{
    insert(root_, key, val);
}

template <class Key, class Value>
void RbTree<Key, Value>::erase(const Key &key)
{
    erase(root_, key);
}

template <class Key, class Value>
void RbTree<Key, Value>::erase(MyRbNode *x, const Key &key)
{
    if (x == nullptr) {
        return;
    }

    if (key < x->key) {
        erase(x->left, key);
    } else if (key > x->key) {
        erase(x->right, key);
    } else {
        MyRbNode *successor;
        if (x->left == nullptr) {
            successor = x->right;
            eraseFixUp(x);
        } else if (x->right == nullptr) {
            successor = x->left;
            eraseFixUp(x);
        } else {
            successor = minimum(x->right);
            eraseFixUp(successor);

            // 只有后继节点不是x的右孩子,才分为两步
            if (successor != x->right) {
                transplant(successor, successor->right);
            }

            successor->color = x->color; // 有两个孩子的节点删除不改变节点的颜色
        }

        transplant(x, successor);

        delete(x);
        count_--;
    }
}

/**
 * @brief 删除节点为左子节点上的调整操作
 */
template <class Key, class Value>
typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::eraseFixUpLeftNode(MyRbNode *x)
{
    if (x == nullptr) {
        return x;
    }

    MyRbNode *p = x->parent; // x的父节点
    MyRbNode *s = p->right; // x的兄弟节点

    if (s->color == COLER_RED) {
        leftRotateWithParent(p); // 将p作为支节点，进行左旋
        
        // p->color = COLER_RED;
        // s->color = COLER_BLACK;

        s = s->parent->right;
    }

    MyRbNode *sl = s->left;
    MyRbNode *sr = s->right;
    if (!isRed(sl) && !isRed(sr)) {
        s->color = COLER_RED;
        x = p;
    } else {
        if (!isRed(sr)) {
            rightRotateWithParent(sl);

            // s->color = COLER_RED;
            // sl->color = COLER_BLACK;
        }

        leftRotateWithParent(p);

        // s->color = p->color;
        sr->color = COLER_BLACK;
        // p->color = COLER_BLACK;

        x = root_; // 已经调整完成，直接将下次调整的节点置为根节点
    }

    return x;
}

/**
 * @brief 删除节点为右子节点上的调整操作
 */
template <class Key, class Value>
typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::eraseFixUpRightNode(MyRbNode *x)
{
    if (x == nullptr) {
        return x;
    }

    MyRbNode *p = x->parent;
    MyRbNode *s = p->left;

    if (s->color = COLER_RED) {
        rightRotateWithParent(p);

        // p->color = COLER_RED;
        // s->color = COLER_BLACK;

        s = s->parent->left;
    }

    MyRbNode *sl = s->left;
    MyRbNode *sr = s->right;
    if (!isRed(sl) && !isRed(sr)) {
        s->color = COLER_RED;
        x = p;
    } else {
        if (!isRed(sl)) {
            leftRotateWithParent(sr);

            // s->color = COLER_RED;
            // sr->color = COLER_BLACK;
        }

        rightRotateWithParent(p);

        // s->color = p->color;
        sl->color = COLER_BLACK;
        // p->color = COLER_BLACK;

        x = root_;
    }

    return x;
}

/**
 * @brief 删除黑色节点的调整操作
 */
template <class Key, class Value>
void RbTree<Key, Value>::eraseFixUp(MyRbNode *x)
{
    if (x == nullptr || x->color != COLER_BLACK) {
        return;
    }

    // 保证不是从根节点开始
    while (x != root_ && x->color == COLER_BLACK) {
        if (x == x->parent->left) { // 当前节点是父节点的左子节点
            x = eraseFixUpLeftNode(x);
        } else { // 当前节点是父节点的右子节点
            x = eraseFixUpRightNode(x);
        }
    }
    x->color = COLER_BLACK;
}

#endif