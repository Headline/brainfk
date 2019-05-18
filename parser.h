#ifndef H_PARSER
#define H_PARSER

#include <iostream>
#include <string>
#include <cstdarg>

#include <vector>
#include <unordered_map>
#include <utility>
#include "cell.h"

static constexpr char *errors[] = {
	"Invalid instruction '%c'.",
};

namespace Brainfuck {
	using ErrorCallback = void(std::string_view, bool);

	class Parser {
	public:
		Parser() = default;
		Parser(std::string_view str) noexcept;

		void setString(std::string_view str) noexcept {
			this->str = std::string(str);
		}
		void parse(ErrorCallback cb) noexcept;
		std::string buildError(size_t err, ...) noexcept;
		std::unordered_map<size_t, size_t> &getMap() noexcept {
			return map;
		}
	private:
		std::string str{};
		std::unordered_map<size_t, size_t> map{};
	};
}


#endif // H_PARSER