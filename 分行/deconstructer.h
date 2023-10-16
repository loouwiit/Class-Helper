#pragma once
#include <functional>
class Deconstructer
{
public:
	Deconstructer(const std::function<void()> function) { this->function = function; }
	Deconstructer() = delete;
	Deconstructer(Deconstructer&) = delete;
	~Deconstructer() { function(); }

private:
	std::function<void()> function;
};
