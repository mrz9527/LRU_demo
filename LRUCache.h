// author : zhoukang
// date   : 2022-06-01 16:36:45

#ifndef EPOLL_DEMO_LRU_H
#define EPOLL_DEMO_LRU_H

#include <unordered_map>

template<typename K, typename V>
struct Node {
    K key = K();
    V value = V();

    Node<K, V>* next = nullptr;
    Node<K, V>* pre = nullptr;

    Node(K k= K(), V v = V()): key(k), value(v) {};
};

class LRUCache {
    using IINode = Node<int, int>;
    using PIINode = IINode*;
    using PIINodeHashMap = std::unordered_map<int, PIINode>;
private:
    int capacity_;
    int size_;
    PIINode head_;
    PIINode tail_;
    PIINodeHashMap node_map_;
public:
    LRUCache(int capacity) : capacity_(capacity), size_(0), head_(new IINode),
                             tail_(new IINode)
    {
        head_->next = tail_;
        tail_->pre = head_;
    }

    ~LRUCache() {

    }

    int get(int key) {
        auto flag = hashmap_find(key);
        if(!flag) return -1;

        PIINode node = hashmap_get(key);
        list_move_to_front(node);
        return node->value;
    }

    void put(int key, int value) {
        auto flag = hashmap_find(key);
        if(!flag) {
            // 未找到
            if(is_full()) {
                list_popback();
                hashmap_del(key);
            }
            PIINode node = list_pushfront(key, value);
            hashmap_push(node);
        } else {
            // 找到
            PIINode node = hashmap_get(key);
            node->value = value;
            list_move_to_front(node);
        }
    }

private:
    int get_size() const {
        return size_;
    }

    bool is_full() {
        return size_ == capacity_;
    };

    bool hashmap_find(int key) {
        return node_map_.count(key) > 0;
    };

    // 哈希表和双链表都要删除
    void hashmap_del(int key) {
        node_map_.erase(key);
    }

    PIINode hashmap_get(int key) {
        return node_map_[key];
    }

    void hashmap_push(PIINode node) {
        node_map_.emplace(node->key, node);
    }

    // 删除链表尾元素
    void list_popback() {
        if(get_size() == 0) return;

        // 待删除节点
        PIINode node = tail_->pre;

        node->pre->next = tail_;
        tail_->pre = node->pre;

        delete node;

        --size_;
    };

    // 节点存在时，移动到头部
    void list_move_to_front(PIINode node) {
        if(node == nullptr) return;
        if(node == head_->next) return; // 第一个位置移动到第一个位置

        //从当前位置摘除
        PIINode pre = node->pre;
        PIINode next = node->next;

        pre->next = next;
        next->pre = pre;

        // 放入头部位置
        head_->next->pre = node;
        node->next = head_->next;

        head_->next = node;
        node->pre = head_;
    };

    // 有新的数据时，在头部插入
    PIINode list_pushfront(int key, int value) {
        PIINode node = new(std::nothrow) IINode(key, value);
        if(node) {
            head_->next->pre = node;
            node->next = head_->next;

            head_->next = node;
            node->pre = head_;

            ++size_;

            return node;
        }
        return nullptr;
    }
};

#endif //EPOLL_DEMO_LRU_H


