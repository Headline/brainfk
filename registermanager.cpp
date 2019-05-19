#include "registermanager.h"

#include <string>

Brainfuck::RegisterManager::RegisterManager(std::ostream &ostream, std::istream &istream) noexcept :
	reg(0), ostream(ostream), istream(istream), 
#ifndef BRAINFUCK_ALLOCATION_SIZE
	positive(100), negative(100)
#else
	positive(BRAINFUCK_ALLOCATION_SIZE), negative(BRAINFUCK_ALLOCATION_SIZE)
#endif

{
	positive.push_back(0);
}

void Brainfuck::RegisterManager::doRead() noexcept
{
	getVectorCell() = istream.get();
}

void Brainfuck::RegisterManager::doLeft() noexcept {
	--reg;
#ifndef BRAINFUCK_MAX_SPEED
	ensureSize();
#endif
}

void Brainfuck::RegisterManager::doRight() noexcept {
	++reg;
#ifndef BRAINFUCK_MAX_SPEED
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