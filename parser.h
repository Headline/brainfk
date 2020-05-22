#ifndef H_PARSER
#define H_PARSER

#include <string>
#include <cstdarg>
#include <vector>
#include <stack>

namespace Brainfuck {
	static constexpr char const * const errors[] = {
		"Invalid instruction '%c'.",
		"Unbalanced loop detected",
	};

	static constexpr bool IsValidInstruction(char c) noexcept {
		return c == '+' || c == '-' || c == '>' || c == '<' || c == '.' || c == ',' || c == '[' || c == ']';
	}

	using ErrorCallback = void(std::string_view, bool);

	struct Instruction {
		char a;
		uint8_t repeat;
		size_t val;
	};

	class Parser {
	public:
		Parser() = default;
		Parser(std::string_view str) : str(str) {
#ifdef DEBUG
		std::cout << "Parsing: " << std::string(str) << std::endl;
#endif
		}

		void setString(std::string_view str) {
			this->str = std::string(str);
		}
		std::vector<Instruction> &getInstructions() {
			return instructions;
		}

	public:
		void parse(ErrorCallback cb) {
			std::stack<size_t> loopstack;
			for (auto it = str.begin(); it != str.end(); ++it)
			{
				const char c = *it;
				if (c == ' ') {
					instructions.push_back({c, 0, 0});
					continue;
				}
				if (!IsValidInstruction(c)) {
					std::string err = buildError(0, c);
					cb(err, true);
				}

				if (c == '[') {
					loopstack.push(instructions.size());
					instructions.push_back({c, 0, 0});
				}
				else if(c == ']') {
					size_t tmp = loopstack.top();
					loopstack.pop();
					instructions.push_back({c, 0, tmp});
				} else {
					uint8_t repeat = 0;
					if (c == '+' || c == '-' || c == '>' || c == '<') {
						do {
							++repeat;
							++it;
						} while (it != str.end() && *it == c);
						--it;
					}
					instructions.push_back({c, repeat, 0});
				}
			}
			
			if (loopstack.size() != 0) {
				std::string err = buildError(1);
				cb(err, true);
			}
			
			for (size_t i = 0; i < instructions.size(); ++i) {
				auto &inst = instructions[i];
				if (inst.a == '[') {
					for (size_t j = i; j < instructions.size(); ++j) {
						auto &inst2 = instructions[j];
						if (inst2.a == ']' && inst2.val == i) {
							inst.val = j;
						}
					}
				}
			}
#ifdef DEBUG
			for (auto inst : instructions) {
				printf("%c val: %d repeat %d\n", inst.a, inst.val, inst.repeat);
			}
#endif
		}
		std::string buildError(size_t err, ...) {
			va_list ap;
			va_start(ap, err);
			
			char errorstr[128];
			vsnprintf(errorstr, sizeof(errorstr), errors[err], ap);

			char errorstring[256];
			snprintf(errorstring, sizeof(errorstring), "[Error] %s", errorstr);
			va_end(ap);
			return std::string(errorstring);
		}

	private:
		std::string str{};
		std::vector<Instruction> instructions;
	};
}


#endif // H_PARSER
