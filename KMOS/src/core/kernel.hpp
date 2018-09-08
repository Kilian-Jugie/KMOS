#ifndef _KERNEL_H_GUARD
#define _KERNEL_H_GUARD
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"



//#define NULL 0LL

//#ifdef UNICODE
//using char_t = wchar_t;
//#else
using char_t = char;
//#endif

namespace Core {
	class Kernel {
		Terminal<char_t> m_Terminal;
	public:
		int init() {
			m_Terminal.init(vga_utils::VGA_WIDTH, vga_utils::VGA_HEIGHT);
			return 0;
		}

		//Write string to terminal
		void write(const char_t* ch) {
			m_Terminal.write(ch);
		}

		void write(const char_t* ch, size_t size) {
			m_Terminal.write(ch, size);
		}




	};
}

#endif