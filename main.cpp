#include "TXCachePolicy.hpp"
#include "TXLruCache.hpp"


int main() {
    TXCache::TXLruCache<int, int> cache(2);

    return 0;
}