#ifndef H_BRAINFUCKJIT
#define H_BRAINFUCKJIT

#include <windows.h>

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>


#include "optimizer.h"
#include "platform.h"

namespace Brainfuck {
	class Jit {
public:
		Jit() {
		}
		~Jit() {
		}

		inline void init() {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				// Windows x64 calling convention dictates the parameters are passed in the following order
				// RCX - RDX - R8
				// Therefore our cells ptr will exist in RCX, getchar in RDX, and putchar in R8
				0x49, 0x89, 0xCB,			// mov r11, rcx
#else // x86
				// Most x86 calling conventions require us to save the old call frame somewhere safe
				0x55,						// push ebp
				0x89, 0xE5,					// mov ebp, esp // init new call frame
				0x56,						// push esi
				0x57,						// push edi
				// Windows x86 calling convention states paramaters are stored in the stack, lets grab them
				0x8B, 0x4D, 0x08,			// mov ecx, [ebp+8] // cellsptr
				0x8B, 0x75, 0x0C,			// mov esi, [ebp+12] // getchar
				0x8B, 0x7D, 0x10,			// mov edi, [ebp+16] // putchar

#endif
				});
		}
		inline void shiftRight(uint8_t amount) {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x49, 0x83, 0xC3, amount,	// add r11, amount
#else // x86
				0x83, 0xC1, amount,			// add ecx, amount
#endif
			});
		}
		inline void shiftLeft(uint8_t amount) {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x49, 0x83, 0xEB, amount,	// sub r11, i.repeat
#else // x86
				0x83, 0xE9, amount,			// sub ecx, amount
#endif
			});
		}
		inline void inc(uint8_t amount) {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x41, 0x80, 0x03, amount,	// add BYTE PTR [r11],i.repeat
#else // x86
				0x80, 0x01, amount,			// add byte ptr[ecx], amount
#endif
			});
		}
		inline void dec(uint8_t amount) {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x41, 0x80, 0x2B, amount,	// sub BYTE PTR [r11], i.repeat		
#else // x86
				0x80, 0x29, amount,			// sub byte ptr[ecx], amount
#endif
			});
		}
		inline void doPrint() {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x52,						// push rdx
				0x41, 0x50,					// push r8
				0x41, 0x53,					// push r11
				0x49, 0x0F, 0xB6, 0x0B,		// movzx  rcx,BYTE PTR [r11] 
				0x48, 0x83, 0xEC, 0x10,		// sub rsp, 16
				0x41, 0xff, 0xd0,			// call r8 // putchar
				0x48, 0x83, 0xC4, 0x10,		// add rsp, 16
				0x41, 0x5B,					// pop r11
				0x41, 0x58,					// pop r8
				0x5A,						// pop rdx
#else // x86
				0x51,						// push ecx
				0x0F, 0xB6, 0x09,			// movzx ecx, byte ptr[ecx]
				0x51,						// push ecx
				0xFF, 0xD7,					// call edi
				0x83, 0xC4, 0x04,			// add esp, 4 // remove param from stack
				0x59,						// pop ecx
#endif
			});
		}
		inline void doRead() {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x52,						// push rdx
				0x41, 0x50,					// push r8
				0x41, 0x53,					// push r11
				0xFF, 0xD2,					// call rdx
				0x41, 0x88, 0x03,			// mov BYTE PTR [r11],al
				0x41, 0x5B,					// pop r11
				0x41, 0x58,					// pop r8
				0x5A,						// pop rdx
#else // x86
				0x51,						// push ecx
				0xFF, 0xD6,					// call esi
				0x59,						// pop ecx
				0x88, 0x01,					// move byte ptr[ecx], eax
#endif
			});
		}
		inline void loopStart() {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x41, 0x80, 0x3B, 0x00,				// cmp byte ptr [r11], 0
#else
				0x80, 0x39, 0x00,					// cmp byte ptr [ecx], 0
#endif
				0x0f, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // je 0xDEADBEEF
			});
		}
		inline void loopEnd() {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0x41, 0x80, 0x3B, 0x00,				// cmp byte ptr [r11], 0
#else
				0x80, 0x39, 0x00,					// cmp byte ptr [ecx], 0
#endif
				0x0f, 0x85, 0xDE, 0xAD, 0xBE, 0xEF	// jne 0xDEADBEEF
			});
		}

		inline void buildJumps(unsigned char *blob) {
			std::stack<int32_t> loopstack;
			for (int32_t pos = 0; pos < (int32_t)code.size()-6; pos++) {

				if (blob[pos] == 0x0f && blob[pos+1] == 0x84) {
					loopstack.push(pos+6); // mark *after* this jump instruction
				}
				if (blob[pos] == 0x0f && blob[pos + 1] == 0x85) {
					auto start = loopstack.top();
					loopstack.pop();

					// add 6 since the length of the jnz instruction is 6 bytes and
					// jumps are relative
					int32_t label = start - (pos+6);
					int32_t jmp = (pos+6) - start;

					// for the first one, we need to write to where we're going (back) to,
					// since we're already at right before the jnz instruction, we add 2
					// in order to replace 0xDEADBEEF and not the jnz instruction itself
					memcpy(blob + pos+2, &label, sizeof(int32_t));

					// |start| is just past where we want to start writing, let's go back
					// and write over the address
					memcpy(blob + start-4, &jmp, sizeof(int32_t));
				}
			}
		}
		inline void endFunction() {
			pushBytes({
#ifdef PLATFORM_WINDOWS_64
				0xc3,		// ret
#else //X32
				0x5F,						// pop edi
				0x5E,						// pop esi
				0x89, 0xEC, // mov esp, ebp
				0x5D,		// pop ebp
				0xc3,
#endif
			});
		}

		void spew(std::ostream& ostream) {
			for (auto byte : code) {
				printf("%02x ", byte);
			}
		}

		void pushBytes(std::vector<unsigned char> arr) noexcept {
			code.insert(code.end(), arr.begin(), arr.end());
		}

		size_t codeSize() {
			return code.size();
		}

		const unsigned char* base() const {
			return this->code.data();
		}

		void execute(bool optimize = false) noexcept {
			if (optimize) {
				Optimizer<unsigned char> o(code);
				o.optimize();
				//spew(std::cout);
				//system("pause");
			}

			auto alloc = VirtualAlloc(NULL, 0x4000000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			std::memcpy(alloc, code.data(), code.size());

			buildJumps((unsigned char *)alloc);

			DWORD old;
			VirtualProtect(alloc, code.size(), PAGE_EXECUTE_READ, &old);
			(void)old;// silence unused..

			auto const funct = reinterpret_cast<void(*)(uint8_t*, void*, void*)>(alloc);

			std::unique_ptr<uint8_t[]> cells(new uint8_t[1024]);
			std::memset(cells.get(), 0, 1024);
#ifdef BF_BENCHMARK
			auto start = std::chrono::steady_clock::now();
#endif
			funct(cells.get(), getchar, putchar);
#ifdef BF_BENCHMARK
			auto end = std::chrono::steady_clock::now();
			std::cout << "Elapsed time in milliseconds : "
				<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
				<< " milliseconds" << std::endl;
#endif
			VirtualFree(alloc, 0, MEM_RELEASE);
		}
	private:
		std::vector<unsigned char> code;
		std::stack<int> loopstack;
	};

}
#endif // H_BRAINFUCKJIT
