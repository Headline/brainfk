#include "registermanager.h"

Brainfuck::RegisterManager::RegisterManager() noexcept
{
	reg = 0;
	positive.push_back(0);
}

void Brainfuck::RegisterManager::doAction(Action const a) noexcept
{
	switch (a)
	{
		case Up:
			doUp();
			break;
		case Down:
			doDown();
			break;
		case Right:
			doRight();
			break;
		case Left:
			doLeft();
			break;
	}
}

void Brainfuck::RegisterManager::doUp() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	auto &cell = vec.at(index);
	cell.up();
}

void Brainfuck::RegisterManager::doDown() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	auto &cell = vec.at(index);
	cell.down();
}

void Brainfuck::RegisterManager::doLeft() noexcept {
	reg--;
	ensureSize();
}

void Brainfuck::RegisterManager::doRight() noexcept {
	reg++;
	ensureSize();
}

void Brainfuck::RegisterManager::ensureSize() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	if (vec.size() <= index)
		vec.emplace_back(0);
}

bigint Brainfuck::RegisterManager::getCurrentValue() noexcept
{
	size_t index = 0;
	auto &vec = getVector(index);
	return vec[index].getCell();
}

std::vector<Brainfuck::Cell>& Brainfuck::RegisterManager::getVector(size_t &out) noexcept {
	if (this->reg >= 0) {
		out = (size_t)this->reg;
		return positive;
	}
	else {
		out = (size_t)std::abs(this->reg)-1;
		return negative;
	}
}