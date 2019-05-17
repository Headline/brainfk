#ifndef H_CELL
#define H_CELL

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <string>

namespace Brainfuck {
	enum Action {
		Left = 0,
		Right,
		Up,
		Down
	};

	class Cell {
	public:
		Cell(Cell const &cell) noexcept : cell(cell.cell) {}
		Cell(int const cell) noexcept : cell(cell) {}
		Cell() noexcept : cell(0) {}
		void down() noexcept {
			cell--;
			if (cell < 0) {
				cell = 255;
			}
		}
		void up() noexcept {
			cell++;
			if (cell > 255) {
				cell = 0;
			}
		}
		int getCell() const noexcept {
			return cell;
		}
		operator int() { return cell; }
		operator int const() { return cell; }
		Cell& operator= (const Cell &cell) {
			this->cell = cell.getCell();
		}
		Cell& operator= (const int cell) {
			this->cell = cell;
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