#ifndef __TREEADAPTER_H_
#define __TREEADAPTER_H_

#include "BinaryTree/BinaryTree.h"

template <class Key, class Value, class Tree = BinaryTree<Key, Value>>
class TreeAdapter {
public:
    TreeAdapter() {}
    ~TreeAdapter() {}

public:
    /* 将键为key，值为val的键值对存入表中 */
    void put(Key key, Value val) { tree.insert(key, val); }

    /* 获取键key对应的值 */
    const Value &get(Key key) const { return tree.get(key); }

    /* 从表中删除键key */
    void erase(Key key) { tree.erase(key); }

    /* 键key是否存在表中 */
    bool contains(Key key) { return tree.contains(key); }

    /* 表中键值对的数量 */
    int size() { return tree.size(); }

    /* 最小的键 */
    Key min() { return tree.min(); }

    /* 最大的键 */
    Key max() { return tree.max(); }

    /* 小于等于key的键 */
    Key floor(Key key) { return tree.floor(); }

    /* 大于等于key的键 */
    Key ceiling(Key key) { return tree.ceiling(); }

    /* 小于key键的数量 */
    int rank(Key key) { return tree.rank(key); }

    /* 排名为k的键 */
    Key select(int k) { return tree.select(k); }

    /* 删除最小的键 */
    void deleteMin() { tree.deleteMin(); }

    /* 删除最大的键 */
    void deleteMax() { tree.deleteMax(); }

    /* 累计[lo, ho]之间键的数量 */
    int size(Key lo, Key hi) { return tree.size(lo, hi); }

    /* [lo, ho]之间所有的键(已排序) */
    // vector<Key> Keys(Key lo, Key hi);

    /* 表中所有元素的集合 */
    // vector<Key> Keys();

private:
    Tree tree;
};

#endif // __TREEADAPTER_H_