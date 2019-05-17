#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <string>

#include "brainfuckvm.h"

using namespace Brainfuck;

int main(int argc, char *argv[]) noexcept { 
	BrainfuckVM vm{};
	char c;
	do {
		system("CLS");
		std::cout << vm << std::endl;
	
		std::string in{};
		std::getline(std::cin, in);
		c = in[0];
		
		Action a;
		switch (c) {
			case 'w':
				a = Up;
				break;
			case 'a':
				a = Left;
				break;
			case 's':
				a = Down;
				break;
			case 'd':
				a = Right;
				break;
		}
		
		vm.doAction(a);
	} while (c != 'q');
}