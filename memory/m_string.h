/*
 * MString
 * 自定义String类型，用于测试Allocator<T>
 * 
 */

#pragma once
#include "allocator.h"
#include <ostream>

class MString {
private:
	char* start;
	std::size_t length;
	Memory::Allocator<char> allocator;
public:
	MString(const char* str = "");
	~MString();

	char* data() const;
	std::size_t size() const;

	MString& operator=(const char* str);
	MString& operator=(const MString& str);
	MString& operator+=(const char* str);
	MString& operator+= (const MString& str);
	friend std::ostream& operator<<(std::ostream& os, const MString& str);
};
