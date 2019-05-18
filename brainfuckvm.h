#ifndef H_BRAINFUCKVM
#define H_BRAINFUCKVM

#include <iostream>
#include <cassert>

#include "registermanager.h"
#include "parser.h"

namespace Brainfuck {
	class BrainfuckVM {
	public:
		BrainfuckVM(std::ostream &ostream, std::istream &istream) noexcept : regs(ostream, istream) {}

		void doAction(char a) noexcept {
#ifdef DEBUG
			std::cout << "Doing action " << a << std::endl;
#endif
			regs.doAction(a);
		}
		
		void run(std::string_view str, ErrorCallback cb) noexcept {
			parser.setString(str);
			parser.parse(cb);
			auto start = std::chrono::steady_clock::now();
			run(parser.getInstructions());
			auto end = std::chrono::steady_clock::now();
	
			std::cout << "Elapsed time in seconds : " 
				<< std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
				<< " seconds" << std::endl;
		}

		void run(std::vector<Instruction> const &inst) noexcept {
			size_t size{inst.size()};
			for (size_t i = 0; i < size; ++i) {
				char c = inst[i].a;
				if (c == ']' && regs.getCurrentValue() != 0) {
					i = inst[i].val;
					continue;
				}
				doAction(c);
			}
		}

		RegisterManager getRegisterManager() const noexcept {
			return regs;
		}
		friend std::ostream& operator<<(std::ostream &os, BrainfuckVM const &vm) noexcept {
			RegisterManager regs = vm.getRegisterManager();
			os << regs;
			return os;
		}
	private:
		Parser parser;
		RegisterManager regs;
	};
}
#endif