#ifndef H_OPTIMIZER
#define H_OPTIMIZER

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <optional>

#include "platform.h"

//#define BF_BENCHMARK

namespace Brainfuck {
	struct OptimizerEntry {
		OptimizerEntry(std::vector<unsigned char> find, std::vector<unsigned char> replace)
			: find(find), replace(replace) {}

		OptimizerEntry(std::vector<unsigned char> find, std::vector<unsigned char> replace, std::vector<bool> mask, bool mustEQ)
			: find(find), replace(replace), mask(mask), wildcardsMustEq(mustEQ) {}

		std::vector<unsigned char> find;
		std::vector<unsigned char> replace;
		std::vector<bool> mask;
		bool wildcardsMustEq;
	};

	OptimizerEntry REPLACEMENT_QUERIES[] = {
#ifdef PLATFORM_WINDOWS_64
		/**
		 * Removes unnecessary loop comparisons when resetting a cell to 0. i.e. [-]
		 *
		 *		cmp byte ptr [r11], 0
		 *		je <addr>
		 *		sub byte ptr[r11], amount
		 *		cmp byte ptr [r11], 0
		 *		jne <addr>
		 *
		 *	Will be converted to
		 *
		 *		mov byte ptr[r11], 0
		 *
		 */
		{
			{0x41, 0x80, 0x3B, 0x00, 0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,
			 0x41, 0x80, 0x2B, 0x2A, 0x41, 0x80, 0x3B, 0x00, 0x0F, 0x85,
			 0xDE, 0xAD, 0xBE, 0xEF},

			{0x41, 0xC6, 0x03, 0x00},

			{true, true, true, true, true, true, true, true, true, true,
			 true, true, true, false, true, true, true, true, true, true,
			 true, true, true, true},
			false
		},
		/**
		 * Optimizes routines with destructive move operations like the following: [->>>+<<<]
		 * We can wildcard the movement amount & move the value ourself.
		 *
		 *		cmp byte ptr [r11], 0
		 *		je <addr>
		 *		sub byte ptr[r11], 1
		 *		add r11, ???
		 *		add byte ptr[r11], 1
		 *		sub r11, ???
		 *		cmp byte ptr [r11], 0
		 *		jne <addr>
		 *
		 * Can be converted to the trivial operation shown below
		 *
		 *		movzx rcx, byte ptr[r11]
		 *		mov byte ptr[r11], 0
		 *		add r11, ???
		 *		mov byte ptr[r11], cl
		 *		sub r11, ???
		 *
		 */
		{
			{0x41, 0x80, 0x3B, 0x00, 0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, 0x41, 0x80, 0x2B, 0x01, 0x49,
			 0x83, 0xC3, 0x2A, 0x41, 0x80, 0x03, 0x01, 0x49, 0x83, 0xEB, 0x2A, 0x41, 0x80, 0x3B, 0x00,
			 0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF},

			{0x49, 0x0F, 0xB6, 0x0B, 0x41, 0xC6, 0x03, 0x00, 0x49, 0x83, 0xC3, 0x90, 0x41, 0x88, 
			 0x0B, 0x49, 0x83, 0xEB, 0x90},

			{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			 true, true, false, true, true, true, true, true, true, true, false, true, true, true, true,
			 true, true, true, true, true, true},
			true
		}
 #else
		/**
		 * Removes unnecessary loop comparisons when resetting a cell to 0. i.e. [-]
		 *
		 *		cmp byte ptr [ecx], 0
		 *		je <addr>
		 *		sub byte ptr[ecx], amount
		 *		cmp byte ptr [ecx], 0
		 *		jne <addr>
		 *
		 *	Will be converted to
		 *
		 *		mov byte ptr[ecx], 0
		 *
		 */
		{
			{0x80, 0x39, 0x00, 0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, 0x80, 0x29, 0x2A, 0x80, 0x39, 0x00,
			 0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF},

			{0xC6, 0x01, 0x00},

			{true, true, true, true, true, true, true, true, true, true, true, false, true, true, true,
			 true, true, true, true, true, true},
			false
		},
		/**
		 * Optimizes routines with destructive move operations like the following: [->>>+<<<]
		 * We can wildcard the movement amount & move the value ourself.
		 *
		 *		cmp byte ptr [ecx], 0
		 *		je <addr>
		 *		sub byte ptr[ecx], 1
		 *		add ecx, ???
		 *		add byte ptr[ecx], 1
		 *		sub ecx, ???
		 *		cmp byte ptr [ecx], 0
		 *		jne <addr>
		 *
		 * Can be converted to the trivial operation shown below
		 *
		 *		movzx rcx, byte ptr[r11]
		 *		mov byte ptr[r11], 0
		 *		add r11, ???
		 *		mov byte ptr[r11], cl
		 *		sub r11, ???
		 *
		 */
		{
			{0x80, 0x39, 0x00, 0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, 0x80, 0x29, 0x01, 0x83, 0xC1, 0x2A,
			 0x80, 0x01, 0x01, 0x83, 0xE9, 0x2A, 0x80, 0x39, 0x00, 0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF },

			{0x0F, 0xB6, 0x01, 0xC6, 0x01, 0x00, 0x83, 0xC1, 0x90, 0x88, 0x01, 0x83, 0xE9, 0x90},

			{true, true, true, true, true, true, true, true, true, true, true, true, true, true, false,
			 true, true, true, true, true, false, true, true, true, true, true, true, true, true, true },
			true
		}
 #endif
	};

	template <typename T>
	class Optimizer {
	public:
		using Iterator = typename std::vector<T>::iterator;
		Optimizer() {};
		Optimizer(std::vector<T> &code) : code(code) {};

		Iterator find(OptimizerEntry entry) {
			// general case: test to see if we can find a replacement
			return std::search(code.begin(), code.end(), std::boyer_moore_searcher(entry.find.begin(), entry.find.end()));
		}

		void replace(Iterator it, OptimizerEntry entry, std::optional<T> wildcardByte) {
			if (it != code.end()) {
				// if we're dealing with a masked replacement, then we need to handle
				// the wildcard byte
				if (wildcardByte) {
					std::vector<unsigned char> replacement(entry.replace);
					std::replace(replacement.begin(), replacement.end(), (T)0x90, *wildcardByte);

					auto end = it;
					std::advance(end, entry.find.size());

					auto last = code.erase(it, end);
					code.insert(last, replacement.begin(), replacement.end());
				}
				else {
					auto last = code.erase(it, it + entry.find.size());
					// our convention here is that if the replacement begins with a NOP, then there's nothing to replace
					// and we should just move on
					if (entry.replace[0] != 0x90) {
						//--last; // go back to where we erased, since we're going to need to insert
						code.insert(last, entry.replace.begin(), entry.replace.end());
					}
				}
			}
		}

		void optimize() {
			for (auto entry : REPLACEMENT_QUERIES) {
				if (entry.find.size() == 0)
					continue;

				// if there's a mask, we need to handle replacement ourselves since the
				// replacement may require the masked value
				if (entry.mask.size() > 0) {

					Iterator result = code.end();
					T wildcardByte{};
					while ((result = manualFind(entry, wildcardByte)) != code.end()) {
						replace(result, entry, wildcardByte);
					}
				}
				// no mask, static replacement
				else {
					Iterator result = code.end();
					while ((result = find(entry)) != code.end()) {
						replace(result, entry, std::nullopt);
					}
				}
			}
		}

		Iterator manualFind(OptimizerEntry entry, T& wildcardByte) {
			// if there's a mask, we need to handle replacement ourselves
			for (size_t i = 0; i < code.size(); i++) {
				for (size_t j = 0; j < entry.find.size(); j++) {
					// if we're currently at a masked value
					if (!entry.mask[j]) {
						if (wildcardByte) {
							// if we need wildcard bytes to equal eachother, lets ensure
							// that they do.
							if (wildcardByte != code[i + j] && entry.wildcardsMustEq) {
								break;
							}
						}
						else {
							// lets store whatever's there to compare against later
							wildcardByte = code[i + j];
						}
					}
					if (code[i + j] != entry.find[j] && entry.mask[j]) {
						break;
					}
					if (j == entry.find.size() - 1) {
						return code.begin() + i;
					}
				}
			}
			return code.end();
		}

	private:
		std::vector<T> &code;
	};
}
#endif H_OPTIMIZER

