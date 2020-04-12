#ifndef __TREE_COMM_API_H_
#define __TREE_COMM_API_H_


template <class Key, class Value>
class tree_comm_api {
public:
    tree_comm_api();
    virtual ~tree_comm_api();

public:
    /* 将键为key，值为val的键值对存入表中 */
    void put(Key key, Value val);

    /* 获取键key对应的值 */
    Value get(Key key);

    /* 从表中删除键key */
    void del(Key key);

    /* 键key是否存在表中 */
    bool contains(Key key);

    /* 表中键值对的数量 */
    int size();

    /* 最小的键 */
    Key min();

    /* 最大的键 */
    Key max();

    /* 小于等于key的键 */
    Key floor(Key key);

    /* 大于等于key的键 */
    Key ceiling(Key key);

    /* 小于key键的数量 */
    int rank(Key key);

    /* 排名为k的键 */
    Key select(int k);

    /* 删除最小的键 */
    void deleteMin();

    /* 删除最大的键 */
    void deleteMax();

    /* 累计[lo, ho]之间键的数量 */
    int size(Key lo, Key hi);

    /* [lo, ho]之间所有的键(已排序) */
    // vector<Key> Keys(Key lo, Key hi);

    /* 表中所有元素的集合 */
    // vector<Key> Keys();
};


#endif