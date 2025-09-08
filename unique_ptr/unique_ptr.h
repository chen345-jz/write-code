#include<atomic>


template<typename T>
class unique_ptr
{
private:
    T* ptr;

public:
    //显示构造函数
    explicit  unique_ptr(T* p=nullptr):ptr(p){}

    //析构函数
    ~unique_ptr(){delete ptr;}

    //禁用拷贝构造
    unique_ptr(const unique_ptr<T>& other) = delete;
    unique_ptr& operator=(const unique_ptr<T>& other) = delete;


    //移动构造函数
    unique_ptr(unique_ptr<T>&& other) noexcept :ptr(other.ptr)
    {
        other.ptr=nullptr;
    }

    unique_ptr& operator=(unique_ptr<T>&& other)noexcept
    {
        if(this!=&other)
        {
            delete ptr;
            ptr=other.ptr;
            other.ptr=nullptr;
        }
        return *this;
    }

    T&  operator* ( ) const
    {
        return *ptr; //为什么不先判断if（ptr）?  保持和原生指针的行为一致
    }

    T& operator->() const
    {
        return ptr;
    }

    explicit operator bool() const //explicit:防止智能指针被隐式转换为 bool 类型
    {
        return ptr;
    }

    T* release()
    {
        T* old = ptr;
        ptr = nullptr;
        return old;
    }

    void reset(T* p=nullptr)
    {
        delete ptr;
        ptr=p;
    }

    void swap(unique_ptr<T>& other) noexcept //noexcept表示不会抛出异常，让容器如vector在扩容或元素搬移的时候优先选择移动而非拷贝
    {
        std::swap(ptr,other.ptr);
    }

    void swap(unique_ptr<T>& lhs,unique_ptr<T>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

    bool operator==(const unique_ptr<T>* lhs,const unique_ptr<T>* rhs)
    {
        return lhs.get()==rhs.get();
    }

    bool operator!=(const unique_ptr<T>* lhs,const unique_ptr<T>* rhs)
    {
        return !(lhs.get()==rhs.get());
    }
};