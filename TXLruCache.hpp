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
        TXLruCache(int capacity)
            : capacity_(capacity)
        {
            LruNode* head_;
            LruNode* tail_;
            head_->next = tail_;
            tail_->prev = head_;
        }

        ~TXLruCache() override = default;

        bool get(Key key, Value& value) override
        {
            auto it = nodemap_.find(key);

            if(it != nodemap_.end()){
                moveToHead(it->second);
                value = it->second->value_;
                return true;
            }

            return false;
        }

        Value get(Key key) override
        {
            Value value{};
            get(key, value);
            return value;
        }

        void put(Key key, Value value) override
        {
            auto it = nodemap_.find(key);

            if(it != nodemap_.end()){
                it->second->value_ = value;
                moveToHead(it->second);
                return;
            }

            LruNode* newnode = new LruNode(key, value);
            nodemap_[key] = newnode;
            addToHead(newnode);

            if(nodemap_.size() > capacity_){
                LruNode* tailnode = popTail();
                nodemap_.erase(tailnode->key_);
                delete tailnode;
            }
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

        LruNode* popTail()
        {
            LruNode* node = tail_->prev_;
            removeNode(node);
            return node;
        };

    private:
        int capacity_;
        LruNode* head_;
        LruNode* tail_;
        std::unordered<Key, LruNode*> nodemap_;
    };
} 