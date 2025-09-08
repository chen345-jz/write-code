//饿汉式

class Singleton
{
    private:
     static Singleton instance;
    public:

    static Singleton getInstance()
    {
        return instance;
    }
};

Singleton Singleton::instance;// 静态实例定义



//懒汉式
class Singleton {
private:
    static Singleton* instance;
    Singleton() {} // 构造函数私有化
public:
    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }
};

// 类外初始化静态成员
Singleton* Singleton::instance = nullptr;