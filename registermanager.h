#ifndef H_REGISTERMANAGER
#define H_REGISTERMANAGER

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

//#define BRAINFUCK_MAX_SPEED // skips bounds checking
//#define BRAINFUCK_ALLOCATION_SIZE 10000 // manually sets array size

namespace Brainfuck {
	using bigint = unsigned int;
	template <typename T>
	class RegisterManager {
	public:
		RegisterManager(std::ostream &ostream, std::istream &istream)  :
		reg(0), ostream(ostream), istream(istream), 
#ifndef BRAINFUCK_ALLOCATION_SIZE
	positive(1000), negative(1000)
#else
	positive(BRAINFUCK_ALLOCATION_SIZE), negative(BRAINFUCK_ALLOCATION_SIZE)
#endif
		{
			positive.push_back(0);
		}

		void doLeft(int times) {
			reg -= times;
#ifndef BRAINFUCK_MAX_SPEED
			ensureSize();
#endif
		}
		
		void doRight(int times) {
			reg += times;
#ifndef BRAINFUCK_MAX_SPEED
			ensureSize();
#endif
		}
		void doUp(int times) {
			getVectorCell() += times;
		}

		void doDown(int times) {
			getVectorCell() -= times;
		}
		
		void doRead() {
			getVectorCell() = istream.get();
		}

	public:
		Brainfuck::bigint getCurrentRegister() const{
			return reg;
		}
		size_t size() const {
			return positive.size() + negative.size();
		}
		std::vector<T> const &getPositive() const {
			return positive;
		}
		std::vector<T> const &getNegative() const {
			return negative;
		}

		void doAction(char a, int times) {
			if (a == '+')
				doUp(times);
			else if (a == '-')
				doDown(times);
			else if (a == '>')
				doRight(times);
			else if (a == '<')
				doLeft(times);
			else if (a == '.')
				ostream << (unsigned char)getVectorCell();
			else if (a == ',')
				doRead();
		}
		inline T& getVectorCell() {
			return (reg >= 0) ? positive[reg] : negative[(reg*-1)-1];
		}

		friend std::ostream& operator<<(std::ostream& os, RegisterManager const &reg) {
			auto &negative = reg.getNegative();
			auto &positive = reg.getPositive();

			std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
			os<<"[..] [";
			std::reverse_copy(negative.begin(), negative.end(), std::ostream_iterator<T>(os, "] ["));
			std::copy(positive.begin(), positive.end(), std::ostream_iterator<T>(os, "] ["));
			os<<"..]";
			return os;
		}
	private:
		void ensureSize() {
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

	private:
		bigint reg;
		std::ostream &ostream;
		std::istream &istream;
		std::vector<T> positive;
		std::vector<T> negative;
	};
}
#endif
