/*
 * Allocator<T> 
 * 通过MemoryPool的静态对象Memory::memory_pool实现
 * 所有Allocator<T>的对象，内部均调用同一个MemoryPool，即Memory::memory_pool
 * 
 */

#pragma once

#include "memory_pool.h"
#include <type_traits>

namespace Memory {

template <typename T>
class Allocator {
public:
	using size_type = std::size_t;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using propagete_on_container_move_assignment = std::true_type;
	using is_always_equal = std::true_type;
public:
	Allocator() noexcept {}
	~Allocator() noexcept {}
public:

	/* allocate()
	 * 
	 * @brief 分配 n * sizeof(T) 大小的未初始化存储空间
	 * @return 返回指向 T 类型的 n 个对象的数组首元素的指针，数组元素尚未构造
	 */
	[[nodiscard]] constexpr T* allocate(std::size_t n) {
		if (n == 0) return nullptr;
		return static_cast<T*>(memory_pool.allocate(n * sizeof(T)));
	}

	/* deallocate()
	 * 
	 * @brief 从指针 p 所引用的存储解分配
	 * @param ptr 必须是通过先前对 allocate() 获得的指针
	 * @param n 必须等于对原先生成 p 的 allocate() 调用的首参数
	 */
	constexpr void deallocate(T* ptr, std::size_t n) {
		if (n == 0) return;
		memory_pool.deallocate(static_cast<void*>(ptr), sizeof(T) * n);
	}

	/* construct()
	 * 
	 * @brief 在 p 所指的未初始化存储中构造对象
	 * @param ptr 指向未初始化存储的指针
	 * @param ...args 所用的构造函数参数
	 */
	template <class U, class... Args>
	void construct(U* ptr, Args&&... args) {
		new (ptr) U(std::forward<Args>(args)...);
	}

	/* destroy()
	 * 
	 * @brief 调用 p 所指的对象的析构函数
	 * @param ptr 指向要被销毁的对象的指针
	 */
	template <class U>
	void destroy(U* ptr) {
		ptr->~U();
	}
};

}; /* Memory */