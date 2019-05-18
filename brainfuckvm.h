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
			run(str, parser.getMap());
			auto end = std::chrono::steady_clock::now();
	
			std::cout << "Elapsed time in microseconds : " 
				<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
				<< " microseconds" << std::endl;
		}

		void run(std::string_view str, std::unordered_map<size_t, size_t> const &loopmap) noexcept {
			size_t size{str.size()};
			for (size_t i = 0; i < size; i++) {
				switch (str[i]) {
					case ']':
						if (regs.getCurrentValue() != 0) {
							i = loopmap.find(i)->second;
						}
						break;
					default:
						doAction(str[i]);
						break;
				}
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