// 测试 shared_ptr
#include "shared_ptr.h"
#include <iostream>

struct Foo {
	int val;
	Foo(int v) : val(v) { std::cout << "Foo(" << val << ") 构造\n"; }
	~Foo() { std::cout << "Foo(" << val << ") 析构\n"; }
	void show() { std::cout << "Foo::val = " << val << std::endl; }
};

int main() {
	std::cout << "--- 默认构造 ---\n";
	shared_ptr<Foo> p1;
	std::cout << "p1.use_count(): " << p1.use_count() << std::endl;

	std::cout << "--- 显式构造 ---\n";
	shared_ptr<Foo> p2(new Foo(42));
	std::cout << "p2.use_count(): " << p2.use_count() << std::endl;
	p2->show();

	std::cout << "--- 拷贝构造 ---\n";
	shared_ptr<Foo> p3 = p2;
	std::cout << "p2.use_count(): " << p2.use_count() << ", p3.use_count(): " << p3.use_count() << std::endl;

	std::cout << "--- 移动构造 ---\n";
	shared_ptr<Foo> p4 = std::move(p3);
	std::cout << "p4.use_count(): " << p4.use_count() << ", p3.use_count(): " << p3.use_count() << std::endl;

	std::cout << "--- 拷贝赋值 ---\n";
	p1 = p2;
	std::cout << "p1.use_count(): " << p1.use_count() << ", p2.use_count(): " << p2.use_count() << std::endl;

	std::cout << "--- 移动赋值 ---\n";
	p1 = std::move(p4);
	std::cout << "p1.use_count(): " << p1.use_count() << ", p4.use_count(): " << p4.use_count() << std::endl;

	std::cout << "--- 重置指针 ---\n";
	p1.reset(new Foo(100));
	std::cout << "p1.use_count(): " << p1.use_count() << std::endl;
	p1->show();

	std::cout << "--- 作用域结束，自动析构 ---\n";
	return 0;
}
