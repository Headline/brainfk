#include "registermanager.h"

#include <string>

Brainfuck::RegisterManager::RegisterManager(std::ostream &ostream, std::istream &istream) noexcept :
	reg(0), ostream(ostream), istream(istream), positive(100), negative(100)
{
	positive.push_back(0);
}

void Brainfuck::RegisterManager::doRead() noexcept
{
	getVectorCell() = istream.get();
}

void Brainfuck::RegisterManager::doLeft() noexcept {
	--reg;
#ifdef MAX_SPEED
	ensureSize();
#endif
}

void Brainfuck::RegisterManager::doRight() noexcept {
	++reg;
#ifdef MAX_SPEED
	ensureSize();
#endif
}

inline void Brainfuck::RegisterManager::ensureSize() noexcept
{
	if (reg >= 0) {
		if (positive.size() <= reg) {
			positive.push_back(0);
		}
	} else {
		if (negative.size() <= (reg*-1)-1) {
			negative.push_back(0);
		}
	}
}