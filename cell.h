#ifndef H_CELL
#define H_CELL

#include <iostream>
#include <string>
#include "debug.h"

namespace Brainfuck {
	class Cell {
	public:
		constexpr Cell(int cell) noexcept : cell(cell) {}
		constexpr Cell() noexcept : cell(0) {}
		constexpr void down() noexcept {
			--cell;
		}
		constexpr void up() noexcept {
			++cell;
		}
		constexpr int getCell() const noexcept {
			return cell%256;
		}
		constexpr Cell& operator+=(int val) noexcept {
			  this->cell += val;
			  return *this;
		}
		constexpr Cell& operator-=(int val) noexcept {
			  this->cell -= val;
			  return *this;
		}
		friend std::ostream& operator<<(std::ostream& os, Cell const &cell) noexcept
		{
			os << cell.getCell();
			return os;
		}

	private:
		int cell;
	};
}
#endif