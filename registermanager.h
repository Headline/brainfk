#ifndef H_REGISTERMANAGER
#define H_REGISTERMANAGER

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

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

	public:
		unsigned char getCurrentValue() noexcept{
			return getVectorCell();
		}

		Brainfuck::bigint getCurrentRegister() const noexcept{
			return reg;
		}
		size_t size() const noexcept {
			return positive.size() + negative.size();
		}
		std::vector<unsigned char> const &getPositive() const noexcept {
			return positive;
		}
		std::vector<unsigned char> const &getNegative() const noexcept {
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
				ostream << getVectorCell();
			else if (a == ',')
				doRead();

#ifdef DEBUG
			std::cout << *this << std::endl;
#endif
		}
		inline unsigned char& getVectorCell() noexcept {
			return (reg >= 0) ? positive[reg] : negative[(reg*-1)-1];
		}
		friend std::ostream& operator<<(std::ostream& os, RegisterManager const &reg) {
			auto &negative = reg.getNegative();
			auto &positive = reg.getPositive();
			os<<"[..] [";
			std::reverse_copy(negative.begin(), negative.end(), std::ostream_iterator<unsigned char>(os, "] ["));
			std::copy(positive.begin(), positive.end(), std::ostream_iterator<unsigned char>(os, "] ["));
			os<<"..]";
			return os;
		}
	private:
		void ensureSize() noexcept;

	private:
		bigint reg;
		std::ostream &ostream;
		std::istream &istream;
		std::vector<unsigned char> positive;
		std::vector<unsigned char> negative;
	};
}
#endif H_REGISTERMANAGER