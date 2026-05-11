#pragma once

namespace TXCache
{

template <typename Key, typename Value>
class TXCachePolicy
{
public:
    virtual ~TXCachePolicy(){};

    virtual void put(Key key, Value value) = 0;
    virtual bool get(Key key, Value& value) = 0;
    virtual Value get(Key key) = 0;
};

            
}