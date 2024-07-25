/*
 * Allocator<T> 
 * ͨ��MemoryPool�ľ�̬����Memory::memory_poolʵ��
 * ����Allocator<T>�Ķ����ڲ�������ͬһ��MemoryPool����Memory::memory_pool
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
	 * @brief ���� n * sizeof(T) ��С��δ��ʼ���洢�ռ�
	 * @return ����ָ�� T ���͵� n �������������Ԫ�ص�ָ�룬����Ԫ����δ����
	 */
	[[nodiscard]] constexpr T* allocate(std::size_t n) {
		if (n == 0) return nullptr;
		return static_cast<T*>(memory_pool.allocate(n * sizeof(T)));
	}

	/* deallocate()
	 * 
	 * @brief ��ָ�� p �����õĴ洢�����
	 * @param ptr ������ͨ����ǰ�� allocate() ��õ�ָ��
	 * @param n ������ڶ�ԭ������ p �� allocate() ���õ��ײ���
	 */
	constexpr void deallocate(T* ptr, std::size_t n) {
		if (n == 0) return;
		memory_pool.deallocate(static_cast<void*>(ptr), sizeof(T) * n);
	}

	/* construct()
	 * 
	 * @brief �� p ��ָ��δ��ʼ���洢�й������
	 * @param ptr ָ��δ��ʼ���洢��ָ��
	 * @param ...args ���õĹ��캯������
	 */
	template <class U, class... Args>
	void construct(U* ptr, Args&&... args) {
		new (ptr) U(std::forward<Args>(args)...);
	}

	/* destroy()
	 * 
	 * @brief ���� p ��ָ�Ķ������������
	 * @param ptr ָ��Ҫ�����ٵĶ����ָ��
	 */
	template <class U>
	void destroy(U* ptr) {
		ptr->~U();
	}
};

}; /* Memory */