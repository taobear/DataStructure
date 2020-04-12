#ifndef __RBTREE_H_
#define __RBTREE_H_

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

private:
    MyRbNode *insert(MyRbNode *x, const Key &key, const Value &val);
    MyRbNode *erase(MyRbNode *x, const Key &key);

    const MyRbNode *minimum(const MyRbNode *x);
    const MyRbNode *maximum(const MyRbNode *x);

private:
    void eraseFixUp(MyRbNode *root, MyRbNode *x);
    void insertFixUp(MyRbNode *root, MyRbNode *x);

    MyRbNode *eraseFixUpLeftNode(MyRbNode *x);
    MyRbNode *eraseFixUpRightNode(MyRbNode *x);

    void leftRotateWithParent(MyRbNode *x);
    void rightRotateWithParent(MyRbNode *x);

    void transplant(MyRbNode *oldNode, MyRbNode *newNode);

    MyRbNode *leftRotate(MyRbNode *x);
    MyRbNode *rightRotate(MyRbNode *x);

    MyRbNode *flipColor(MyRbNode *x);

    bool isRed(MyRbNode *x) { return !(x == nullptr || x->color != COLER_RED); }

private:
    MyRbNode *root_;
    int count_;
};

template <class Key, class Value>
const typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::minimum(const MyRbNode *x)
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
const typename RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::maximum(const MyRbNode *x)
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
    }
    newNode->parent = oldNode->parent;
}

template <class Key, class Value>
RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::leftRotate(MyRbNode *x)
{
    assert(x != nullptr);
    assert(x->right != nullptr && x->right->color == COLER_RED);

    MyRbNode *oldRoot = x;
    MyRbNode *newRoot = x->right;

    oldRoot->right = newRoot->left;
    newRoot->left = oldRoot;

    newRoot->color = oldRoot->color;
    oldRoot->color = COLER_RED;

    newRoot->parent = oldRoot->parent;
    oldRoot->parent = newRoot;

    return newRoot;
}

template <class Key, class Value>
RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::rightRotate(MyRbNode *x)
{
    assert(x != nullptr);
    assert(x->left != nullptr && x->left->color == COLER_RED);
    
    MyRbNode *oldRoot = x;
    MyRbNode *newRoot = x->left;

    oldRoot->left = newRoot->right;
    newRoot->right = oldRoot;

    newRoot->color = oldRoot->color;
    oldRoot->color = COLER_RED;

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
RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::flipColor(MyRbNode *x)
{
    assert(x != nullptr);
    assert(x->left != nullptr && x->right != nullptr);

    x->color = COLER_RED;
    x->left = COLER_BLACK;
    x->right = COLER_BLACK;
}

template <class Key, class Value>
RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::insert(MyRbNode *x, const Key &key, const Value &val)
{
    if (x == nullptr) {
        return new MyRbNode(key, val, COLER_RED);
    }

    if (key < x->key) {
        insert(x->left, key, val);
    } else if (key > x->key) {
        insert(x->right, key, val);
    } else {
        x->val = val;
    }

    if (isRed(x->right) && !isRed(x->left)) x = leftRotate(x);
    if (isRed(x->left) && isRed(x->left->left)) x = rightRotate(x);
    if (isRed(x->left) && isRed(x->right)) x = flipColor(x);

    return x;
}

template <class Key, class Value>
void RbTree<Key, Value>::insert(const Key &key, const Value &val)
{
    insert(root_, key, val);
    root_->color = COLER_BLACK;
}

template <class Key, class Value>
RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::erase(MyRbNode *x, const Key &key)
{
    if (x == nullptr) {
        return x;
    }

    if (key < x->key) {
        erase(x->left, key);
    } else if (key > x->key) {
        erase(x->right, key);
    } else {
        MyRbNode *successor;
        Color color = x->color;

        if (x->left == nullptr) {
            successor = x->right;
            transplant(x, successor);
        } else if (x->right == nullptr) {
            successor = x->left;
            transplant(x, successor);
        } else {
            successor = minimum(x->right);

            transplant(successor, successor->right);
            if (successor == x->right) {
                x->right = successor->right;
                successor->right->parent = x;
            } else {
                successor->right->parent = successor->parent;
                successor->parent->left = successor->right;
            }

            transplant(x, successor);
        }
    }

    if (x->color == COLER_BLACK) {
        eraseFixUp(root_, x);
    }
}

/**
 * @brief 删除节点为左子节点上的调整操作
 */
template <class Key, class Value>
RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::eraseFixUpLeftNode(MyRbNode *x)
{
    if (x == nullptr) {
        return;
    }

    MyRbNode *p = x->parent; // x的父节点
    MyRbNode *s = p->right; // x的兄弟节点

    if (s->color == COLER_RED) {
        leftRotateWithParent(p); // 将p作为支节点，进行左旋
        
        p->color = COLER_RED;
        s->color = COLER_BLACK;

        s = s->parent->right;
    }

    if (!isRed(s->left) && !isRed(s->right)) {
        s->color = COLER_RED;
        x = p;
    } else {
        MyRbNode *sl = s->left;
        MyRbNode *sr = s->right;
        if (!isRed(sr)) {
            rightRotateWithParent(sl);

            s->color = COLER_RED;
            sl->color = COLER_BLACK;
        }

        leftRotateWithParent(p);

        s->color = p->color;
        s->right->color = COLER_BLACK;
        p->color = COLER_BLACK;

        x = root_; // 已经调整完成，直接将下次调整的节点置为根节点
    }

    return x;
}

/**
 * @brief 删除节点为右子节点上的调整操作
 */
template <class Key, class Value>
RbTree<Key, Value>::MyRbNode *
RbTree<Key, Value>::eraseFixUpRightNode(MyRbNode *x)
{
    if (x == nullptr) {
        return;
    }

    MyRbNode *p = x->parent;
    MyRbNode *s = p->left;

    if (s->color = COLER_RED) {
        rightRotateWithParent(p);

        p->color = COLER_RED;
        s->color = COLER_BLACK;

        s = s->parent->left;
    }

    if (!isRed(s->left) && !isRed(s->right)) {
        s->color = COLER_RED;
        x = p;
    } else {
        MyRbNode *sl = s->left;
        MyRbNode *sr = s->right;
        if (!isRed(sl)) {
            leftRotateWithParent(sr);

            s->color = COLER_RED;
            sr->color = COLER_BLACK;
        }

        rightRotateWithParent(p);

        s->color = p->color;
        s->left->color = COLER_BLACK;
        p->color = COLER_BLACK;

        x = root_;
    }

    return x;
}

/**
 * @brief 删除黑色节点的调整操作
 */
template <class Key, class Value>
void RbTree<Key, Value>::eraseFixUp(MyRbNode *root, MyRbNode *x)
{
    assert(x != nullptr || x->color != COLER_BLACK);

    // 保证不是从根节点开始
    while (x != root && x->color == COLER_BLACK) {
        if (x == x->parent->left) { // 当前节点是父节点的左子节点
            x = eraseFixUpLeftNode(x);
        } else { // 当前节点是父节点的右子节点
            x = eraseFixUpRightNode(x);
        }
    }
}

#endif