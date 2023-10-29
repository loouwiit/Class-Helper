#include "file.h"

void File::open(const char* path)
{
	file.open(path, std::ios::in | std::ios::out | std::ios::binary);
}

void File::addBE()
{
	file.put(0xFE);
	file.put(0xFF);
}

void File::ignore(std::streamsize count)
{
	file.ignore(count);
}

void File::close()
{
	file.close();
}

wchar_t File::get()
{
	return (wchar_t)((file.get() << 8) + file.get());
}

size_t File::getline(wchar_t buffer[], size_t bufferSize, wchar_t wchar)
{
	size_t pos = 0;
	while (pos < bufferSize)
	{
		buffer[pos] = get();
		if (buffer[pos] == wchar)
		{
			buffer[pos] = L'\0';
			break;
		}
		pos++;
	}

	return pos;
}

void File::put(wchar_t wchar)
{
	file.put(wchar >> 8);
	file.put(wchar & 0xFF);
}

File& File::operator<<(const unsigned char number)
{
	wchar_t buffer[10];
	_itow_s(number, buffer, 10, 10);
	for (unsigned char i = 0; buffer[i] != L'\0' && i < 10; i++)
		put(buffer[i]);
	return *this;
}

File& File::operator<<(const wchar_t wchar)
{
	put(wchar);
	return *this;
}

File& File::operator<<(const std::wstring string)
{
	for (wchar_t i : string)
	{
		put(i);
	}
	return *this;
}

File& File::operator>>(std::wstring string)
{
	file >> string;
	return *this;
}
