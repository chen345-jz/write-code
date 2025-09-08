#include<atomic>
template<typename T>
class shared_ptr
{
private:
    T* ptr;
    std::atomic<std::size_t>* ref_count;
    void release()
    {
        if(ref_count&&ref_count->fetch_sub(1)==1)
        delete p;
        delete ref_count;
    }

    public:
    //默认构造
    shared_ptr():ptr(nullptr),ref_count(nullptr){}
    //显示构造函数
    shared_ptr(T* p):ptr(p),ref_count(p?std::atomic<size_t>(1):nullptr){}

    //拷贝构造函数
    shared_ptr(const shared_ptr<T>& other):ptr(other.ptr),ref_count(other.ref_count)
    {
        if(ref_count)
        {
            ref_count->fetch_add(1);
        }
    }

    //移动构造函数
    shared_ptr(shared_ptr<T>&& other) noexcept:ptr(other.ptr),ref_count(other.ref_count)
    {
        //如果移动构造/赋值后没有置空源对象，就可能导致其析构时再次释放已经被转移的资源,//从而发生严重错误。正确做法是在资源转移后清空原对象指针，防止二次释放。
        other.ptr=nullptr;
        other.ref_count=nullptr;

    }

    //拷贝赋值
    shared_ptr<T> operator=(const shared_ptr<T>& other)
    {
        if(this!=&ohter)
        {
            release();
        }
    }
};