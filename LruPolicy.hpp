#pragma once

#include <unordered_map>
#include <utility>
#include <memory>

#include "TXCachePolicy.hpp"

namespace TXCache
{
    template<typename Key, typename Value> class LruPolicy;

    template <typename Key, typename Value>
    class LruNode
    {
    private:

        Key key_;
        Value value_;
        size_t accessCount_;
        std::weak_ptr<LruNode<Key, Value>> prev_;
        std::shared_ptr<LruNode<Key, Value>> next_;

        public:
        LruNode(Key key, Value value) 
        : key_(std::move(key))
        , value_(std::move(value)) 
        , accessCount_(1)
        {}

        const Key& getKey() const { return key_; }
        const Value& getValue() const { return value_; }
        size_t getAccessCount() const { return accessCount_; }
        void incrementAccessCount() { ++accessCount_; }
        void setValue(Value value) { value_ = std::move(value); }

        friend class LruPolicy<Key, Value>;
    };

    template <typename Key, typename Value>
    class LruPolicy : public CachePolicy<Key, Value>
    {
    public:
        
        using LruNodeType = LruNode<Key, Value>;
        using NodePtr = std::shared_ptr<LruNodeType>;
        using NodeMap = std::unordered_map<Key, NodePtr>;

        LruPolicy(int capacity)
            : capacity_(capacity)
        {
           initializeList();
        }

        ~LruPolicy() override = default;

        bool get(const Key& key, Value& value) override
        {
            auto it = nodemap_.find(key);

            if(it != nodemap_.end()){
                moveToHead(it->second);
                value = it->second->getValue();
                return true;
            }

            return false;
        }

        Value get(const Key& key) override
        {
            Value value{};
            get(key, value);
            return value;
        }

        void put(const Key& key, Value value) override
        {
            auto it = nodemap_.find(key);

            if(it != nodemap_.end()){
                updateExistingNode(it->second, std::move(value));
                return;
            }
            addNewNode(key, std::move(value));
        }

        void remove(const Key& key) 
        {   
        auto it = nodeMap_.find(key);

        if (it != nodeMap_.end())
        {
            removeNode(it->second);
            nodeMap_.erase(it);
        }

        }
        

    private:
        void initializeList()
        {
            dummyHead_ = std::make_shared<LruNodeType>(Key(), Value());
            dummyTail_ = std::make_shared<LruNodeType>(Key(), Value());
            dummyHead_->next_ = dummyTail_;
            dummyTail_->prev_ = dummyHead_;
        }

        void insertHead(NodePtr node)
        {
            node->next_ = dummyhead_->next_;
            node->prev_ = dummyhead_; //隐式转换，prev_仍然是弱指针
            dummyhead_->next_->prev_ = node; //隐式转换
            dummyhead_->next_ = node;
        }

        void removeNode(NodePtr node)
        {
            if(!node->prev_.expired() && node->next_) 
            {
                auto prev = node->prev_.lock(); 
                prev->next_ = node->next_;
                node->next_->prev_ = prev;
                node->next_ = nullptr; 
            }
        }

        void moveToHead(NodePtr node)
        {
            removeNode(node);
            insertHead(node);
        }

        void evictTail()
        {
            NodePtr tail = dummytail_->prev.lock();
            removeNode(tail);
            nodemap_erase.(tail->getKey());
        }

        void updateExistingNode(NodePtr node, const Value& value) 
        {
            node->setValue(value);
            moveToHead(node);
        }

        void addNewNode(const Key& key, const Value& value) 
        {
            if (nodeMap_.size() >= capacity_) 
        {
            evictTail();
        }

        NodePtr newNode = std::make_shared<LruNodeType>(key, std::move(value));
        insertHead(newNode);
        nodeMap_[key] = newNode;
        }

    private:
        int capacity_;
        NodePtr dummyhead_;
        NodePtr dummytail_;
        NodeMap nodemap_;
    };
}