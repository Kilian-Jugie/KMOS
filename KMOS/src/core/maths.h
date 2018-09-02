#ifndef _MATHS_H_GUARD
#define _MATHS_H_GUARD

namespace Maths {

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
}


#endif