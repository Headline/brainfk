#ifndef H_BRAINFUCKVM
#define H_BRAINFUCKVM

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <string>


#include "registermanager.h"

namespace Brainfuck {
	class BrainfuckVM {
	public:
		BrainfuckVM() noexcept {}

		void doAction(Action a) {
			regs.doAction(a);
		}
		
		RegisterManager getRegisterManager() const {
			return regs;
		}
		friend std::ostream& operator<<(std::ostream &os, BrainfuckVM const &vm) {
			RegisterManager regs = vm.getRegisterManager();
			os << regs;
			os << std::endl;
			os << "Register: " << regs.getCurrentRegister() << std::endl;
			os << "Value: " << regs.getCurrentValue();
			return os;
		}
	private:
		RegisterManager regs;
	};
}
#endif