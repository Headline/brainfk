#ifndef H_PARSER
#define H_PARSER

#include <iostream>
#include <string>
#include <cstdarg>
#include <vector>

static constexpr char *errors[] = {
	"Invalid instruction '%c'.",
	"Unbalanced loop detected",
};

namespace Brainfuck {
	using ErrorCallback = void(std::string_view, bool);

	struct Instruction {
		char a;
		int repeat;
		size_t val;
	};

	class Parser {
	public:
		Parser() = default;
		Parser(std::string_view str) noexcept;

		void setString(std::string_view str) noexcept {
			this->str = std::string(str);
		}
		std::vector<Instruction> &getInstructions() noexcept {
			return instructions;
		}

	public:
		void parse(ErrorCallback cb) noexcept;
		std::string buildError(size_t err, ...) noexcept;

	private:
		std::string str{};
		std::vector<Instruction> instructions;
	};
}


#endif // H_PARSER