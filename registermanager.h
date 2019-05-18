#ifndef H_REGISTERMANAGER
#define H_REGISTERMANAGER

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "cell.h"
#include "debug.h"

namespace Brainfuck {
	using bigint = unsigned int;
	class RegisterManager {
	public:
		RegisterManager(std::ostream &ostream, std::istream &istream) noexcept;
		void doUp() noexcept;
		void doDown() noexcept;
		void doLeft() noexcept;
		void doRight() noexcept;
		void doPrint() noexcept;
		void doRead() noexcept;

		Brainfuck::bigint getCurrentValue() noexcept{
			return getVectorCell().getCell();
		}

		Brainfuck::bigint getCurrentRegister() const noexcept{
			return reg;
		}
		size_t size() const noexcept {
			return positive.size() + negative.size();
		}
		std::vector<Cell> const &getPositive() const noexcept {
			return positive;
		}
		std::vector<Cell> const &getNegative() const noexcept {
			return negative;
		}
		void doAction(char a, int times) noexcept {
			if (a == '+') 
				getVectorCell() += times;
			else if (a == '-')
				getVectorCell() -= times;
			else if (a == '>')
				reg += times;
			else if (a == '<')
				reg -= times;
			else if (a == '.')
				ostream << static_cast<char>(getVectorCell().getCell());
			else if (a == ',')
				doRead();
			
#ifdef DEBUG
			std::cout << *this << std::endl;
#endif
		}
		inline Cell& getVectorCell() noexcept {
			return (reg >= 0) ? positive[reg] : negative[(reg*-1)-1];
		}
		friend std::ostream& operator<<(std::ostream& os, RegisterManager const &reg) {
			auto &negative = reg.getNegative();
			auto &positive = reg.getPositive();
			os<<"[..] [";
			std::reverse_copy(negative.begin(), negative.end(), std::ostream_iterator<Cell>(os, "] ["));
			std::copy(positive.begin(), positive.end(), std::ostream_iterator<Cell>(os, "] ["));
			os<<"..]";
			return os;
		}
	private:
		void ensureSize() noexcept;

		bigint reg;
		std::ostream &ostream;
		std::istream &istream;
		std::vector<Cell> positive;
		std::vector<Cell> negative;
	};
}
#endif H_REGISTERMANAGER