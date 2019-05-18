#include "parser.h"

#include <algorithm>

static constexpr inline bool IsValidInstruction(char c) noexcept {
    return c == '+' || c == '-' || c == '>' || c == '<' || c == '.' || c == ',' || c == '[' || c == ']';
}

Brainfuck::Parser::Parser(std::string_view str) noexcept : str(str), instructions(str.size()+1)
{
	std::cout << "Parsing: " << std::string(str) << std::endl;
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

inline size_t findStartLoop(std::string_view str, size_t index) noexcept
{
	int loopscope = 0;
	while (index > 0) {
		char c = str[index];
		if (c == ']')
			--loopscope;
		else if (c == '[')
			++loopscope;
		
		if (!loopscope) {
			return index;
		}
		--index;
	}
	
	return -1;
}

void Brainfuck::Parser::parse(ErrorCallback cb) noexcept
{
	size_t index = 0;
	for (auto it = str.begin(); it != str.end(); ++index, ++it)
	{
		const char c = *it;
		if (!IsValidInstruction(c)) {
			std::string err = buildError(0, c);
			cb(err, true);
		}
		if(c == ']') {
			instructions.push_back({c, findStartLoop(str, index)});
		} else {
			instructions.push_back({c, 0});
		}
		Instruction i = instructions[index];
	}
}