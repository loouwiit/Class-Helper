#pragma once

#include <fstream>

class File
{
public:
	void open(const char* path);
	void addBE();
	void close();

	wchar_t get();
	size_t getline(wchar_t buffer[], size_t bufferSize, wchar_t wchar = L'\n');
	void put(wchar_t wchar);
	void ignore(std::streamsize count = 1);

	File& operator<<(const unsigned char number);
	File& operator<<(const wchar_t wchar);
	File& operator<<(const std::wstring string);

	File& operator>>(std::wstring string);

private:
	std::wfstream file;
};
