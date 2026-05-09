template <typename Key, typename Value>
class BasePolicy {
public:
    virtual ~BasePolicy() = default;

    // 核心增删改查接口
    virtual void put(Key key, Value value) = 0;
    virtual bool get(Key key, Value& value) = 0;
    virtual Value get(Key key) = 0;
    virtual void remove(Key key) = 0;

};