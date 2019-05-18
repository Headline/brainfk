#include "registermanager.h"

#include <string>
Brainfuck::RegisterManager::RegisterManager(std::ostream &ostream, std::istream &istream) noexcept :
	reg(0), ostream(ostream), istream(istream)
{
	positive.push_back(0);
}

void Brainfuck::RegisterManager::doAction(char a) noexcept
{
	switch (a)
	{
		case '+':
			doUp();
			break;
		case '-':
			doDown();
			break;
		case '>':
			doRight();
			break;
		case '<':
			doLeft();
			break;
		case '.':
			doPrint();
			break;
		case ',':
			doRead();
			break;
	}
	
#ifdef DEBUG
	std::cout << *this << std::endl;
#endif
}

void Brainfuck::RegisterManager::doRead() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	vec.at(index) = istream.get();
}

inline void Brainfuck::RegisterManager::doPrint() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	ostream << (char)vec.at(index).getCell();
}

inline void Brainfuck::RegisterManager::doUp() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	vec.at(index).up();
}

inline void Brainfuck::RegisterManager::doDown() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	vec.at(index).down();
}

inline void Brainfuck::RegisterManager::doLeft() noexcept {
	reg--;
	ensureSize();
}

inline void Brainfuck::RegisterManager::doRight() noexcept {
	reg++;
	ensureSize();
}

inline void Brainfuck::RegisterManager::ensureSize() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	if (vec.size() <= index) {
		vec.push_back(0);
	}
}

inline std::vector<Brainfuck::Cell>& Brainfuck::RegisterManager::getVector(size_t &out) noexcept {
	if (this->reg >= 0) {
		out = (size_t)this->reg;
		return positive;
	}
	else {
		out = (size_t)std::abs(this->reg)-1;
		return negative;
	}
}