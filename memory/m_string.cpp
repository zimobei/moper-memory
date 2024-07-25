#include "m_string.h"

// 本MString实现过程中调用Allocator.allocate()分配的内存空间始终与所需存储的字符串str的长度保持一致
// 这使得本MString在调用 operator+=()时总是需要重新分配内存空间
// 如果想要提高性能
// 可以在MString构造时提前分配一个足够大的内存空间

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