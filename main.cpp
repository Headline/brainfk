#include <iostream>

#include <chrono>

#include "brainfuckvm.h"
#include "parser.h"

int main(int argc, char *argv[]) noexcept { 
	Brainfuck::BrainfuckVM vm{std::cout, std::cin};
		
	auto cb = [](std::string_view str, bool fatal) -> void {
		printf("\n%s | fatal: %d\n", std::string(str).c_str(), fatal);
	};
	vm.run(">++[<+++++++++++++>-]<[[>+>+<<-]>[<+>-]++++++++[>++++++++<-]>.[-]<<>++++++++++[>++++++++++[>++++++++++[>++++++++++[>++++++++++[>++++++++++[>++++++++++[-]<-]<-]<-]<-]<-]<-]<-]++++++++++.", cb);
}