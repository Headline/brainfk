#ifndef H_CELL
#define H_CELL

//#define DEBUG

#include <iostream>
#include <string>

namespace Brainfuck {
	class Cell {
	public:
		Cell(Cell const &cell) noexcept : cell(cell.cell) {}
		Cell(int const cell) noexcept : cell(cell) {}
		Cell() noexcept : cell(0) {}
		constexpr void down() noexcept {
			cell--;
			if (cell < 0) {
				cell = 255;
			}
		}
		constexpr void up() noexcept {
			cell++;
			if (cell > 255) {
				cell = 0;
			}
		}
		constexpr int getCell() const noexcept {
			return cell;
		}
		operator int() { return cell; }
		operator int const() { return cell; }
		Cell& operator= (const Cell &cell) noexcept {
			this->cell = cell.getCell();
			return *this;
		}
		Cell& operator= (const int cell) noexcept {
			this->cell = cell;
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