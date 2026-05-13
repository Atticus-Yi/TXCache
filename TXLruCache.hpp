#pragma once

#include <unordered_map>

#include "TXCachePolicy.hpp"

namespace TXCache
{
    template <typename Key, typename Value>
    class LruNode
    {
    public:
        Key key_;
        Value value_;
        LruNode* prev_;
        LruNode* next_;

        LruNode(Key key, Value value) : key_(key), value_(value), prev_(nullptr), next_(nullptr) {}
    };

    template <typename Key, typename Value>
    class TXLruCache : public TXCachePolicy<Key, Value>
    {
    public:
        TXLruCache()
            :
        {
            LruNode* head_;
            LruNode* tail_;
            head_->next = tail_;
            tail_->prev = head_;
        }
        

    private:
        void addtoHead(LruNode* node)
        {
            node->next_ = head_;
            node->prev_ = head->next_;
            head->next_->prev_ = node;
            head->next_ = node;
        };

        void removeNode(LruNode* node)
        {
            node->prev_->next_ = node->next_;
            node->next_->prev_ = node->prev_;
        };

        void moveToHead(LruNode* node)
        {
            removeNode(node);
            addtoHead(node);
        };

        LruNode* removeTail()
        {
            LruNode* node = tail_->prev_;
            removeNode(node);
            return node;
        };

    private:
        LruNode* head_;
        LruNode* tail_;

        srd::unordered<Key, LruNode*> nodemap_;
    };
} 