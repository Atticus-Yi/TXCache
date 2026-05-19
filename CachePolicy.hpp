#pragma once

namespace TXCache
{

template <typename Key, typename Value>
class CachePolicy
{
public:
    virtual ~CachePolicy(){};

    virtual void put(const Key& key, Value value) = 0;
    virtual bool get(const Key& key, Value& value) = 0;
    virtual Value get(const Key& key) = 0;
};

            
}