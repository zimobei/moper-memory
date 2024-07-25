/*
 * MemoryPool
 * ���ڴ��ϵͳ�ĺ���class
 * 
 */

#pragma once
#include <cstdlib> // size_t��malloc()��free()

namespace Memory {

// MemoryPool�ı�������
enum MemoryError {
	CREATE_MEMORY_ERROR, // CreateMemoryBlock()����
	CREATE_FREE_SAPCE_HEADER_ERROR,  // CreateOneFreeSpaceHeader()����
};

// �ײ������
class MemoryPool {
private: // �ڲ����ݽṹ
	// ���д洢���
	struct FreeSpace {
		FreeSpace* next_free; // ָ����һ�����д洢���
	};
	// ���д洢ͷ���
	struct FreeSpaceHeader {
		std::size_t size; // ��ǰͷ����´洢���Ĵ�С
		FreeSpaceHeader* next_header; // ����ָ����һ��ͷ���
		FreeSpace* next_free; // ָ���ͷ����µĵ�һ�����д洢���
	};
	// Blockͷ���
	struct BlockHeader {
		BlockHeader* prev_block; // ָ��ǰBlock��ǰһ��Block
	};

private: // �ڲ���������
	using data_address = unsigned char*; // ���ڱ�ʾ��ַ������һ��Ҫ��ʹ��unsigned char��ֻҪ��СΪ1B�����ͼ��ɣ�
	const std::size_t INITIAL_SIZE = 8; // FreeSpace�е�һ��FreeSpaceHeader��size
	const std::size_t ALIGN_SIZE = 4; // FreeSpace������FreeSpceHeader֮��size�Ĳ�ֵ
private: // ��Ա���� 
	// Block
	BlockHeader* current_block; // ָ��ǰBlock�����ں�������������ʵ��
	// FreeSpace
	std::size_t max_size_of_free_space; // ��¼��ǰFreeSpace���������һ��ͷ����size������ǰFreeSapce���ܷ���������д洢�ռ�
	FreeSpaceHeader* free_space_head; // ָ��FreeSpace����ĵ�һ�����
	FreeSpaceHeader* free_space_rear; // ָ��FreeSpace��������һ����㡣���ں�����չFreeSpace
private: // �ڲ�����������ʵ�ֽӿڣ�

	/* CreateOneFreeSpaceHeader()
	 * 
	 * @brief ����һ��size = target_size��FreeSpaceHeader����
	 * @return ���ض����ָ��
	 */
	FreeSpaceHeader* CreateOneFreeSpaceHeader(std::size_t target_size);

	/* ExtendFreeSpaceHeaders()
	 * 
	 * @brief ��չFreeSpace��ֱ�������ܷ����������С����max_size_of_free_space�� = target_size
	 */
	void ExtendFreeSpaceHeaders(std::size_t target_size);

	/* FindFreeSpaceHeader()
	 * 
	 * @brief ����FreeSpace���ҵ���һ�����Ϸ����СҪ���FreeSpaceHeader
	 * @param target_size �������Ĵ�С
	 * @return ����ָ���FreeSpaceHeader��ָ��
	 */
	FreeSpaceHeader* FindFreeSpaceHeader(std::size_t target_size) const;

	/* CreateMemoryBlock()
	 * 
	 * @brief ��������size��С�ɷ���洢�ռ��Block��BlockHeader��������size�ڣ�
	 * @return ���ظ�Block�п�size��С����洢�ռ���׵�ַ��������BlockHeader��
	 */
	void* CreateMemoryBlock(std::size_t size);
	
	/* ExtendFreeSpace()
	 * 
	 * @brief ��չheader�µ�FreeSpace
	 */
	void ExtendFreeSpace(FreeSpaceHeader* header);
public: // �ӿ�
	MemoryPool() noexcept;
	~MemoryPool() noexcept;

	/* allocate()
	 * 
	 * @brief �����СΪ n ��δ��ʼ���洢�ռ�
	 * @return ���ظ�δ��ʼ���洢�ռ���׵�ַ
	 */
	void* allocate(std::size_t n);

	/* deallocate()
	 * 
	 * @brief ����� p ��ַ����СΪ n �Ĵ洢�ռ�
	 * @param p ����Ϊͨ�� allocate() ��õ�ָ��
	 * @param n ����Ϊͨ�� allocate() ����Ŀռ��С
	 * ע�⣺�����������߱��������Ƿ���ȷ����������˱�֤�ڴ氲ȫ���������û��Լ����
	 */
	void deallocate(void* p, std::size_t n);
};

/* memory_pool
 * 
 * @brief ��̬MemoryPoolʵ������
 * ����MemoryPool֧�������С�Ŀռ����룬
 * �����û�ʹͨ����ͨ��template <class T>����MemoryPool��
 * �����MemoryPool��Ϊ����������class�У�
 * �����������FreeSpaceHeader�޷��õ���Ч����
 * ������
 * ���ĳһ����T�Ĵ�СΪ24�����û�����Ŀռ�һ��Ϊ24��������
 * ����FreeSpace��FreeSpaceHeader�Ĵ�СΪ8��12��16��20��24��28......
 * ���д�СΪ8��12��16��20��28......��FreeSpaceHeader��Զ�޷��õ�ʹ��
 * 
 * ��ˣ��ڴ˶��徲̬MemoryPoolʵ������memory_pool
 * ������Ҫ����MemoryPool��class��ͨ��memory_pool����ʵ��
 * �����е���MemoryPool��class�����ڲ�ʹ�õ���ͬһ��MemoryPool����memory_pool
 * 
 * �����˷����Դ���ȱ�㣺
 * ��memory_pool������Ŀռ�����ڳ���������Զ��ͷ��⣬������Զ�õ��ͷ�
 */
static MemoryPool memory_pool;

}; /* Memory */