#ifndef H_BRAINFUCKVM
#define H_BRAINFUCKVM

#include <iostream>

#include "registermanager.h"
#include "parser.h"
#include "jit.h"

namespace Brainfuck {
	template <typename T>
	class BrainfuckVM {
	public:
		constexpr BrainfuckVM(std::ostream &ostream, std::istream &istream, bool jit) noexcept : regs(ostream, istream), jit(jit) {}

		void doAction(char a, int times = 1) noexcept {
#ifdef DEBUG
			std::cout << "Doing action " << a << std::endl;
#endif
			regs.doAction(a, times);
		}

		constexpr void run(std::string_view str, ErrorCallback cb) noexcept {
			parser.setString(str);
			parser.parse(cb);
			auto& inst = parser.getInstructions();

			if (jit) {
				runJIT(inst);
			}
			else {
				runVM(inst);
			}
		}

		RegisterManager<T> getRegisterManager() const noexcept {
			return regs;
		}

		friend std::ostream& operator<<(std::ostream &os, BrainfuckVM const &vm) noexcept {
			auto regs = vm.getRegisterManager();
			os << regs;
			return os;
		}
private:
		constexpr void runJIT(std::vector<Instruction> const &inst) noexcept {
			Jit jit;
			jit.init();
			for (auto const &i : inst) {
				if (i.a == '>') {
					jit.shiftRight(i.repeat);
				}
				else if (i.a == '<') {
					jit.shiftLeft(i.repeat);
				}
				else if (i.a == '+') {
					jit.inc(i.repeat);
				}
				else if (i.a == '-') {
					jit.dec(i.repeat);
				}
				else if (i.a == '.') {
					jit.doPrint();
				}
				else if (i.a == ',') {
					jit.doRead();
				}
				else if (i.a == '[') {
					jit.loopStart();
				}
				else if (i.a == ']') {
					jit.loopEnd();
				}
			}
			jit.endFunction();
			jit.execute(true);
		}
		
		void runVM(std::vector<Instruction> const &inst) noexcept {
			size_t size{inst.size()};
			for (size_t i = 0; i < size; ++i) {
				char c = inst[i].a;
				if (c == '[' && regs.getVectorCell() == 0) {
					i = inst[i].val;
					continue;
				}
				else if (c == ']' && regs.getVectorCell() != 0) {
					i = inst[i].val;
					continue;
				}
				else {
					doAction(c, inst[i].repeat);
				}
			}
		}

		bool jit;
		Parser parser;
		RegisterManager<T> regs;
	};
}
#endif