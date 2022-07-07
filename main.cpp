// author : zhoukang
// date   : 2022-06-01 16:38:33

#include "LRUCache.h"

int main() {
    LRUCache cache(2);
    cache.put(1, 1); // 缓存是 {1=1}
    cache.put(2, 2); // 缓存是 {1=1, 2=2}
    cache.get(1);    // 返回 1
    cache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    cache.get(2);    // 返回 -1 (未找到)
    cache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    cache.get(1);    // 返回 -1 (未找到)
    cache.get(3);    // 返回 3
    cache.get(4);    // 返回 4

    return 0;
}
