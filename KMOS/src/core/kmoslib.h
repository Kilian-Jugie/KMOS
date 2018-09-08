#ifndef _KMOSLIB_H_GUARD
#define _KMOSLIB_H_GUARD
#include <stdint.h>
#include "kmosdefs.h"



namespace kmos {
	
	constexpr auto SHORT_CHAR_MAX_LENGTH = 6;
	constexpr auto INT_CHAR_MAX_LENGTH = 11;
	constexpr auto LONG_CHAR_MAX_LENGTH = INT_CHAR_MAX_LENGTH;
	constexpr auto LONG_LONG_CHAR_MAX_LENGTH = 20;
	constexpr auto USHORT_CHAR_MAX_LENGTH = SHORT_CHAR_MAX_LENGTH;
	constexpr auto UINT_CHAR_MAX_LENGTH = INT_CHAR_MAX_LENGTH;
	constexpr auto ULONG_CHAR_MAX_LENGTH = UINT_CHAR_MAX_LENGTH;
	constexpr auto ULONG_LONG_CHAR_MAX_LENGTH = LONG_LONG_CHAR_MAX_LENGTH+1;

	constexpr unsigned int rtpt(unsigned x) { //Round To Power of Two
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		++x;
		return x;
	}

	constexpr unsigned long ppow(unsigned long a, unsigned long b) { //Positive Pow
		unsigned long re(1l);
		while (b > 0) {
			if ((b & 1) == 1) {
				re *= a;
			}
			b >>= 1;
			a *= a;
		}
		return re;
	}

	constexpr unsigned int __log2_32_tab32[32] = {
	 0,  9,  1, 10, 13, 21,  2, 29,
	11, 14, 16, 18, 22, 25,  3, 30,
	 8, 12, 20, 28, 15, 17, 24,  7,
	19, 27, 23,  6, 26,  5,  4, 31 };

	constexpr unsigned int log2_32(uint32_t value)
	{
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		return __log2_32_tab32[(uint32_t)(value * 0x07C4ACDD) >> 27];
	}

	inline constexpr int ctoi(const char ch) {
		return ch-'0';
	}

	/*
		char to int
	*/
	constexpr int32_t ctoi(const char* ch, const size_t size) {
		int32_t ret=0;
		for (uint32_t i(ch[0]=='-'?1u:0u); i<size; ++i) 
			ret += ctoi(ch[i])*ppow(10, size - i-1);
		if (ch[0] == '-') ret = -ret;
		return ret;
	}

	/*
		char to unsigned int
	*/
	constexpr uint32_t ctoui(const char* ch, const size_t size) {
		uint32_t ret = 0u;
		for (uint32_t i(0u); i < size; ++i) 
			ret += ctoi(ch[i])*ppow(10, size - i - 1);
		return ret;
	}

	/*
		char to long long
	*/
	constexpr int64_t ctoll(const char* ch, const size_t size) {
		int64_t ret = 0;
		for (uint32_t i(ch[0] == '-' ? 1u : 0u); i < size; ++i)
			ret += ctoi(ch[i])*ppow(10, size - i - 1);
		if (ch[0] == '-') ret = -ret;
		return ret;
	}

	/*
		char to unsigned long long
	*/
	constexpr uint64_t ctoull(const char* ch, const size_t size) {
		uint64_t ret = 0u;
		for (uint32_t i(0u); i < size; ++i)
			ret += ctoi(ch[i])*ppow(10, size - i - 1);
		return ret;
	}

	///DO NOT USE THIS FUNCTION: use itoc, uitoc, lltoc, ulltoc instead
	template<typename _Ty>constexpr int __itoc_base(_Ty i, char* buf, uint32_t size) {
		if (0 == i) {
			buf = const_cast<char*>("0\0");
			return 2;
		}
		buf[size - 1] = '\0';
		if (i < 0) {
			uint32_t _i(size - 2);
			for (; i; i /= 10, --_i)
				buf[_i] = '0' + char(-(i % 10));
			buf[_i] += '-';
		}
		else {
			for (uint32_t _i(size - 2); i; i /= 10, --_i) {
				buf[_i] = ('0' + char(i % 10));
			}
		}
		return size;
	}

	/*
		@brief Convert integer to char
		@param i : integer to convert
		@param buf : buffer to fill of value
		@param size : size of buffer (11 = maximum int size)
		@return used size
	*/
	constexpr int itoc(int i, char* buf, uint32_t size = INT_CHAR_MAX_LENGTH) {
		return __itoc_base(i, buf, size);
	}

	/*
		@brief Convert unsigned integer to char
		@param i : unsigned integer to convert
		@param buf : buffer to fill of value
		@param size : size of buffer (11 = maximum int size)
		@return used size
	*/
	constexpr int uitoc(unsigned int i, char* buf, uint32_t size = UINT_CHAR_MAX_LENGTH) {
		return __itoc_base(i, buf, size);
	}

	/*
		@brief Convert long long integer to char
		@param i : long long integer to convert
		@param buf : buffer to fill of value
		@param size : size of buffer (11 = maximum int size)
		@return used size
	*/
	constexpr int lltoc(long long i, char* buf, uint32_t size = LONG_LONG_CHAR_MAX_LENGTH) {
		return __itoc_base(i, buf, size);
	}

	/*
		@brief Convert unsigned long long integer to char
		@param i : unsigned long long integer to convert
		@param buf : buffer to fill of value
		@param size : size of buffer (11 = maximum int size)
		@return used size
	*/
	constexpr int ulltoc(unsigned long long i, char* buf, uint32_t size = ULONG_LONG_CHAR_MAX_LENGTH) {
		return __itoc_base(i, buf, size);
	}

	


	
}

#endif