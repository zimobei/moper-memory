#include "memory_pool.h"

namespace Memory {

MemoryPool::MemoryPool() noexcept : current_block(nullptr), max_size_of_free_space(INITIAL_SIZE) {
	FreeSpaceHeader* header = CreateOneFreeSpaceHeader(INITIAL_SIZE);

	free_space_head = header;
	free_space_rear = free_space_head;
}

MemoryPool::~MemoryPool() noexcept {
	FreeSpaceHeader* header_iterator = free_space_head;
	while (header_iterator != nullptr) {
		FreeSpaceHeader* next_header = header_iterator->next_header;
		std::free(header_iterator);
		header_iterator = next_header;
	}

	BlockHeader* block_iterator = current_block;
	while (block_iterator != nullptr) {
		BlockHeader* prev_block = block_iterator->prev_block;
		std::free(block_iterator);
		block_iterator = prev_block;
	}
}

void* MemoryPool::allocate(std::size_t n) {
	ExtendFreeSpaceHeaders(n);

	FreeSpaceHeader* header = FindFreeSpaceHeader(n);

	if (header->next_free == nullptr) ExtendFreeSpace(header);

	FreeSpace* result = header->next_free;
	header->next_free = header->next_free->next_free;

	return result;
}

void MemoryPool::deallocate(void* p, std::size_t n) {
	FreeSpaceHeader* header = free_space_head;
	while (header->size < n) header = header->next_header;

	reinterpret_cast<FreeSpace*>(p)->next_free = header->next_free;
	header->next_free = reinterpret_cast<FreeSpace*>(p);
}

MemoryPool::FreeSpaceHeader* MemoryPool::CreateOneFreeSpaceHeader(std::size_t target_size) {
	MemoryPool::FreeSpaceHeader* header = reinterpret_cast<MemoryPool::FreeSpaceHeader*>(std::malloc(sizeof(FreeSpaceHeader)));
	if (header == NULL) throw MemoryError::CREATE_FREE_SAPCE_HEADER_ERROR;

	header->next_free = nullptr;
	header->next_header = nullptr;
	header->size = target_size;

	return header;
}

void MemoryPool::ExtendFreeSpaceHeaders(std::size_t target_size) {
	while (max_size_of_free_space < target_size) {
		MemoryPool::FreeSpaceHeader* header = CreateOneFreeSpaceHeader(max_size_of_free_space + MemoryPool::ALIGN_SIZE);

		free_space_rear->next_header = header;
		free_space_rear = header;

		max_size_of_free_space = free_space_rear->size;
	}
}

MemoryPool::FreeSpaceHeader* MemoryPool::FindFreeSpaceHeader(std::size_t target_size) const {
	MemoryPool::FreeSpaceHeader* header = free_space_head;
	while (header != nullptr && header->size < target_size) header = header->next_header;

	return header;
}

void* MemoryPool::CreateMemoryBlock(std::size_t size) {
	MemoryPool::BlockHeader* block = reinterpret_cast<MemoryPool::BlockHeader*>(std::malloc(sizeof(MemoryPool::BlockHeader) + size));
	if (block == NULL) throw MemoryError::CREATE_MEMORY_ERROR;

	block->prev_block = current_block;
	current_block = block;

	return block + 1;
}

void MemoryPool::ExtendFreeSpace(FreeSpaceHeader* header) {
	std::size_t num = header->size > 128 ? 1 : 20;

	MemoryPool::data_address address = reinterpret_cast<MemoryPool::data_address>(MemoryPool::CreateMemoryBlock(num * header->size));

	for (std::size_t i = 0; i < num; ++i) {
		MemoryPool::FreeSpace* free = reinterpret_cast<MemoryPool::FreeSpace*>(address + i * header->size);
		free->next_free = header->next_free;
		header->next_free = free;
	}
}

}; /* Memory */