// author : zhoukang
// date   : 2022-06-01 16:38:33

#include "LRUCache.h"

int main()
{
    LRUCache<int, int> cache(5);
    cache.Put(1, 1); // 缓存是 {1=1}
    cache.Traverse(PrintLRUNode);
    cache.Put(2, 2); // 缓存是 {1=1, 2=2}
    cache.Traverse(PrintLRUNode);
    cache.Get(1);    // 返回 1
    cache.Traverse(PrintLRUNode);
    cache.Put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    cache.Traverse(PrintLRUNode);
    cache.Get(2);    // 返回 -1 (未找到)
    cache.Traverse(PrintLRUNode);
    cache.Put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    cache.Traverse(PrintLRUNode);
    cache.Get(1);    // 返回 -1 (未找到)
    cache.Traverse(PrintLRUNode);
    cache.Put(7, 5); // 缓存是 {1=1}
    cache.Traverse(PrintLRUNode);
    cache.Put(9, 5); // 缓存是 {1=1}
    cache.Traverse(PrintLRUNode);
    return 0;
}
