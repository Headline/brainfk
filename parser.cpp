#include "parser.h"

#include <algorithm>
#include <stack>

static constexpr inline bool IsValidInstruction(char c) noexcept {
    return c == '+' || c == '-' || c == '>' || c == '<' || c == '.' || c == ',' || c == '[' || c == ']';
}

Brainfuck::Parser::Parser(std::string_view str) noexcept : str(str)
{
#ifdef DEBUG
	std::cout << "Parsing: " << std::string(str) << std::endl;
#endif
}

std::string Brainfuck::Parser::buildError(size_t err, ...) noexcept
{
	va_list ap;
	va_start(ap, err);
	
	char errorstr[128];
	vsnprintf(errorstr, sizeof(errorstr), errors[err], ap);

	char errorstring[256];
	snprintf(errorstring, sizeof(errorstring), "[Error] %s", errorstr);
	va_end(ap);
	return std::string(errorstring);
}

void Brainfuck::Parser::parse(ErrorCallback cb) noexcept
{
	std::stack<size_t> loopstack;
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		const char c = *it;
		if (!IsValidInstruction(c)) {
			std::string err = buildError(0, c);
			cb(err, true);
		}

		if (c == '[') {
			loopstack.push(instructions.size()-1);
		}
		else if(c == ']') {
			size_t tmp = loopstack.top();
			loopstack.pop();
			instructions.push_back({c, 0, tmp});
		} else {
			int repeat = 0;
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
#ifdef DEBUG
	for (auto inst : instructions) {
		printf("%c val: %d repeat %d\n", inst.a, inst.val, inst.repeat);
	}
#endif
}