#ifndef H_REGISTERMANAGER
#define H_REGISTERMANAGER

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <string>

#include "cell.h"

using bigint = long long int;
namespace Brainfuck {
	class RegisterManager {
	public:
		RegisterManager() noexcept;

		void doAction(Action const a) noexcept;

		void doUp() noexcept;
		void doDown() noexcept;
		void doLeft() noexcept;
		void doRight() noexcept;

		bigint getCurrentValue() noexcept;

		std::vector<Cell>& getVector(size_t &out) noexcept;

		bigint getCurrentRegister() const noexcept{
			return reg;
		}
		size_t size() const noexcept {
			return positive.size() + negative.size();
		}
		std::vector<Cell> const &getPositive() const {
			return positive;
		}
		std::vector<Cell> const &getNegative() const {
			return negative;
		}
		friend std::ostream& operator<<(std::ostream& os, RegisterManager const &reg) {
			auto &negative = reg.getNegative();
			auto &positive = reg.getPositive();
			os<<"{ ";
			std::reverse_copy(negative.begin(), negative.end(), std::ostream_iterator<Cell>(os, " "));
			std::copy(positive.begin(), positive.end(), std::ostream_iterator<Cell>(os, " "));
			os<<"}";
			return os;
		}
	private:
		void ensureSize() noexcept;

		bigint reg;
		std::vector<Cell> positive;
		std::vector<Cell> negative;
	};
}
#endif H_REGISTERMANAGER