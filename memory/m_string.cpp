#include "m_string.h"

// ��MStringʵ�ֹ����е���Allocator.allocate()������ڴ�ռ�ʼ��������洢���ַ���str�ĳ��ȱ���һ��
// ��ʹ�ñ�MString�ڵ��� operator+=()ʱ������Ҫ���·����ڴ�ռ�
// �����Ҫ�������
// ������MString����ʱ��ǰ����һ���㹻����ڴ�ռ�

MString::MString(const char* str) : allocator(){
	this->length = strlen(str);
	this->start = this->allocator.allocate(this->length + 1); 
	memcpy(this->start, str, this->length);
	this->start[this->length] = '\0';
}

MString::~MString() { 
	this->allocator.deallocate(this->start, this->length + 1); 
}

char* MString::data() const {
	return this->start;
}

std::size_t MString::size() const {
	return this->length;
}

MString& MString::operator=(const char* str) {
	this->allocator.deallocate(this->start, this->length + 1);
	this->length = strlen(str);
	this->start = this->allocator.allocate(this->length + 1);
	memcpy(this->start, str, this->length);
	this->start[this->length] = '\0';
	return *this;
}

MString& MString::operator=(const MString& str) {
	this->allocator.deallocate(this->start, this->length + 1);
	this->length = str.size();
	this->start = this->allocator.allocate(this->length + 1);
	memcpy(this->start, str.data(), this->length);
	this->start[this->length] = '\0';
	return *this;
}

MString& MString::operator+=(const char* str) {
	std::size_t add = strlen(str);
	char* new_start = this->allocator.allocate(this->length + 1 + add);
	memcpy(new_start, this->start, this->length);
	this->allocator.deallocate(this->start, this->length + 1);
	memcpy(new_start + this->length, str, add);
	this->length += add;
	this->start = new_start;
	this->start[this->length] = '\0';
	return *this;
}

MString& MString::operator+= (const MString& str) {
	char* new_start = this->allocator.allocate(this->length + 1 + str.size());
	memcpy(new_start, this->start, this->length);
	this->allocator.deallocate(this->start, this->length + 1);
	memcpy(new_start + this->length, str.data(), str.size());
	this->length += str.size();
	this->start = new_start;
	this->start[this->length] = '\0';
	return *this;
}

std::ostream& operator<<(std::ostream& os, const MString& str) {
	os << str.data();
	os.flush();
	return os;
}