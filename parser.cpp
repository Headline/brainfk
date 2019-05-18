#include "parser.h"

#include <algorithm>

static constexpr inline bool IsValidInstruction(char c) noexcept {
	switch (c) {
		case '+':
		case '-':
		case '>':
		case '<':
		case '.':
		case ',':
		case '[':
		case ']':
			return true;
		default:
			return false;
	}
}

Brainfuck::Parser::Parser(std::string_view str) noexcept : str(str)
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

inline int findEndLoop(std::string_view str, size_t index) noexcept
{
	int loopscope = 0;
	while (index < str.size()) {
		switch (str[index]) {
			case '[':
				loopscope++;
				break;
			case ']':
				loopscope--;
				break;
		}
		
		if (!loopscope) {
			return index;
		}
		index++;
	}
	
	return -1;
}

void Brainfuck::Parser::parse(ErrorCallback cb) noexcept
{
	size_t index = 0;
	for (auto it = str.begin(); it != str.end(); index++, it++)
	{
		const char c = *it;
		if (!IsValidInstruction(c)) {
			std::string err = buildError(0, c);
			cb(err, true);
		}
		
		switch (c) {
			case '[':
				map.insert({findEndLoop(str, index), index});
			default:
				break;
		}		
	}
}