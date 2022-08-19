// author : zhoukang
// date   : 2022-06-01 16:36:45

#ifndef EPOLL_DEMO_LRU_H
#define EPOLL_DEMO_LRU_H

#include <unordered_map>
#include <numeric>
#include "iostream"

template<typename K, typename V>
struct Node {
    K key = K();
    V value = V();
    Node* next = nullptr;
    Node* prev = nullptr;

    Node(K k, V v) : key(k), value(v), next(nullptr), prev(nullptr)
    {};

    Node() = default;
};

#define LRU_DEFAULT_CAPACITY 100

template<typename K, typename V>
class LRUCache {
    using InnerNode = Node<K, V>;
    using InnerHash = std::unordered_map<K, InnerNode*>;

private:
    int m_size = 0;
    int m_capacity = LRU_DEFAULT_CAPACITY;
    InnerNode* m_head = nullptr; // 用于在开头位置插入元素
    InnerNode* m_tail = nullptr; // 用于在结尾位置删除元素
    InnerHash m_nodeHash; // 用于存放节点信息

public:
    LRUCache(int capacity)
    {
        m_capacity = (capacity <= 0) ? LRU_DEFAULT_CAPACITY : capacity;
        m_size = 0;

        m_head = new InnerNode;
        m_tail = new InnerNode;

        m_head->next = m_tail;
        m_tail->prev = m_head;
    }

    ~LRUCache()
    {
        InnerNode* node = m_head;
        InnerNode* next = nullptr;

        while (node != nullptr) {
            next = node->next;
            delete(node);
            node = nullptr;
            node = next;
        }
    }

    int Get(K key)
    {
        auto iter = m_nodeHash.find(key);
        if (iter == m_nodeHash.end()) {
            return INTMAX_MIN;
        }

        InnerNode* node = iter->second;

        // 移动node到list最前面
        MoveNodeToFront(node);

        return node->value;
    }

    void Put(K key, V value)
    {
        InnerNode* node = nullptr;
        auto iter = m_nodeHash.find(key);
        if (iter != m_nodeHash.end()) {
            node = iter->second;
            // 更新值
            node->value = value;

            // 移动node到list最前面
            MoveNodeToFront(node);
        } else {
            if (IsFull() == true) {
                // list 删除最后一个元素，hash删除对应节点, 更新size
                PopBack();
            }

            node = new InnerNode(key, value);

            // 插入新的元素，hash添加新的节点， 更新size
            PushFront(node);
        }
    }

    void Traverse(void(* callback)(InnerNode*))
    {
        std::cout << "-------------" << std::endl;
        InnerNode* node = m_head->next;
        while (node != m_tail) {
            callback(node);
            node = node->next;
        }
    }

private:
    bool IsFull()
    {
        return m_size >= m_capacity;
    }

    void MoveNodeToFront(InnerNode* node)
    {
        if (node == m_head->next) { // 本身就是第一个节点，不需要移动
            return;
        }

        // 将node从list链表中指定位置去除
        node->prev->next = node->next;
        node->next->prev = node->prev;

        // 将node放入首位
        m_head->next->prev = node;
        node->next = m_head->next;

        m_head->next = node;
        node->prev = m_head;
    }

    void PopBack()
    {
        // 找到最后节点
        InnerNode* lastNode = m_tail->prev;
        K key = lastNode->key;

        // list删除最后节点
        lastNode->prev->next = m_tail;
        m_tail->prev = lastNode->prev;

        delete lastNode;
        lastNode = nullptr;

        // hash删除该节点
        m_nodeHash.erase(key);

        // 更新元素个数
        --m_size;
    }

    void PushFront(InnerNode* node)
    {
        // 将node放入首位
        m_head->next->prev = node;
        node->next = m_head->next;

        m_head->next = node;
        node->prev = m_head;

        // hash增加该节点
        m_nodeHash[node->key] = node;

        ++m_size;
    }
};

void PrintLRUNode(Node<int, int>* node)
{
    std::cout << node->key << " : " << node->value << std::endl;
}

#endif //EPOLL_DEMO_LRU_H


