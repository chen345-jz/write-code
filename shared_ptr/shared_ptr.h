/*实现功能：构造函数
析构函数
拷贝构造函数
拷贝赋值运算符
移动构造函数
移动赋值运算符
解引用、箭头运算符
引用计数、原始指针、重置指针
*/

#include<atomic>  //引入原子操作
template <typename T>
class shared_ptr{
private:
    T* ptr;  //指向管理的对象
    std::atomic<std::size_t>* ref_count;
    void release()
    {
        if(ref_count&&ref_count->fetch_sub(1)==1) //如果减一之前是1，那么释放
        {
            delete ptr;
            delete ref_count;
        }
    }


public:
    //默认构造
    shared_ptr():ptr(nullptr),ref_count(nullptr){}

    //显示构造
    explicit shared_ptr(T* p):ptr(p),ref_count(p?new std::atomic<size_t>(1):nullptr)
    {
    }

    
    // 析构函数
    ~shared_ptr() {
        release();
    }

    //拷贝构造
    shared_ptr(const shared_ptr<T>& other):ptr(other.ptr),ref_count(other.ref_count)
    {
        if(ref_count)
        {
            ref_count->fetch_add(1);
        }
    }

    //移动构造，不再由other指向对象 而由新的shared指针指向 -- ++正好抵消。
    shared_ptr(shared_ptr<T>&& other) noexcept:ptr(other.ptr),ref_count(other.ref_count)
    {
        //如果移动构造/赋值后没有置空源对象，就可能导致其析构时再次释放已经被转移的资源,//从而发生严重错误。正确做法是在资源转移后清空原对象指针，防止二次释放。
        other.ptr=nullptr;
        other.ref_count=nullptr;

    }

    //拷贝赋值
    shared_ptr<T>& operator=(const shared_ptr<T>& other)
    {
        if(this!=&other)
        {
            release();
            ptr=other.ptr;
            ref_count=other.ref_count;
            if(ref_count)
            ref_count->fetch_add(1);
        }
        return *this;
    }

    //移动赋值
    shared_ptr<T>& operator=(shared_ptr<T>&& other)
    {
        if(this!=&other)
        {
            release();
            ptr=other.ptr;
            ref_count=other.ref_count;
            other.ptr=nullptr;
            other.ref_count=nullptr;
        }
        return *this;
    }

    //解引用运算符
    T& operator*() const
    {
        return *ptr;
    }

    //箭头运算符
    T* operator-> () const
    {
        return ptr;
    }

    //获取引用计数
    std::size_t use_count() const
    {
        return ref_count?ref_count->load():0;
    }
    
    // 获取原始指针
    T* get() const {
        return ptr;
    }

    //重置指针
    void reset(T* p=nullptr)
    {
        release();
        ptr=p;
        ref_count = p ? new std::atomic<std::size_t>(1) : nullptr;
    }
};