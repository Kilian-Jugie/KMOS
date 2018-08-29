#ifndef TERMINAL_H_GUARD
#define TERMINAL_H_GUARD
#include "vgautils.h"
namespace Core {
	size_t strlen(const char* str)
	{
		size_t len = 0;
		while (str[len])
			len++;
		return len;
	}

	template<class _CharType>class Terminal {
		size_t m_Row, m_Column, m_MaxWidth, m_MaxHeight;
		uint8_t m_Color;
		uint16_t* m_Buffer;

		void putEntryAt(const _CharType c, const uint8_t color, const size_t x, const size_t y) {
			const size_t i = y * m_MaxWidth + x;
			m_Buffer[i] = vga_utils::entry(c, color);
		}

		void putChar(const _CharType c) {
			if (c == '\n') {
				++m_Row;
				m_Column = 0;
				return;
			}
			putEntryAt(c, m_Color, m_Column, m_Row);
			if (++m_Column == m_MaxWidth) {
				m_Column = 0u;
				if (++m_Row == m_MaxHeight)
					m_Row = 0u;
			}
		}

	public:
		void init(const unsigned long width, const unsigned long height,
			const vga_utils::vga_color fontColor=vga_utils::VGA_COLOR_LIGHT_GREY,
			const vga_utils::vga_color backgroundColor=vga_utils::VGA_COLOR_BLACK) {
			m_MaxHeight = height;
			m_MaxWidth = width;
			m_Row = m_Column = 0u;
			m_Color = vga_utils::entryColor(fontColor, backgroundColor);
			m_Buffer = (uint16_t*)VGA_ADDR;
			for (size_t y(0u); y < m_MaxHeight; ++y) {
				for (size_t x(0u); x < m_MaxWidth; ++x) {
					const size_t i = y * m_MaxWidth + x;
					m_Buffer[i] = vga_utils::entry(' ', m_Color);
				}
			}
		}

		void setColor(const vga_utils::vga_color fontColor = vga_utils::VGA_COLOR_LIGHT_GREY,
			const vga_utils::vga_color backgroundColor = vga_utils::VGA_COLOR_BLACK) {
			m_Color = vga_utils::entryColor(fontColor, backgroundColor);
		}

		void write(const _CharType* str) {
			write(str, strlen(str));
		}

		void write(const _CharType* str, const size_t size) {
			for (size_t i(0u); i < size; ++i)
				putChar(str[i]);
		}
	};
}

#endif