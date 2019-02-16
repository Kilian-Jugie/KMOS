#include "kernel.hpp"
#include "../extensions/version.h"

/*
#define __CONC(x, y) x##y
#define __STR(x) #x
#define TO_STR(x) __STR(x)
#define __ADD_COMMA(...) __VA_ARGS__ ,
#define CMPLRKR(x) x

#define BIT_GENERATE(n, max)  n==max?__STR(max):__STR(n)
#define BIT_GENERATE1(n) n
#define BIT_GENERATE4(n) BIT_GENERATE1(n),BIT_GENERATE1(n+1),BIT_GENERATE1(n+2),BIT_GENERATE1(n+3)
#define BIT_GENERATE16(n) BIT_GENERATE4(n),BIT_GENERATE4(n+4),BIT_GENERATE4(n+8),BIT_GENERATE4(n+12)
#define BIT_GENERATE64(n) BIT_GENERATE16(n), BIT_GENERATE16(n+16), BIT_GENERATE16(n+32), BIT_GENERATE16(n+48)
#define BIT_GENERATE256(n) BIT_GENERATE64(n),BIT_GENERATE64(n+64),BIT_GENERATE64(n+128),BIT_GENERATE64(n+192)

constexpr int intTable[] = { BIT_GENERATE16(0) };

#define BIT_CONVERT(n) __STR(n)
*/

void memtest(Core::Kernel& k) {
	char* content = (char*)Core::malloc(64);
	char* array = (char*)Core::malloc(5);
	array = "loli\0";
	k.write(array);
	k.write(" Addr: ");
	kmos::uitoc((unsigned int)array, content);
	k.write("k");
	content[63] = '\0';
	k.write(content);
	Core::free(array, 5);
	k.write("\n Void Addr: ");
	kmos::uitoc((unsigned int)array, content);
	content[63] = '\0';
	k.write(content);
	array = (char*)Core::malloc(5);
	k.write("\n New Addr: ");
	kmos::uitoc((unsigned int)array, content);
	content[63] = '\0';
	k.write(content);
	Core::free(array, 5);
	Core::free(content, 64);
}

template<typename _Ty>constexpr int _uitoc_base(_Ty i, char* buf, uint32_t size, Core::Kernel& k) {
	if (i==0) {
		buf = const_cast<char*>("0\0");
		return 2;
	}
	buf[size - 1] = '\0';
	for (uint32_t _i(size - 2); i; i /= 10, --_i) {
		//k.write("looped:");
		char d_buf = ('0' + char(i % 10));
		k.write(&d_buf, 1);
		buf[_i] = d_buf; // ('0' + char(i % 10));
		//k.write("2");
	}
	//k.write("kkk");
	return size;
}

const char* indexes[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};

template<typename _Ty>constexpr int _sptuitoc_base(_Ty i, Extensions::smart_ptr<char>& buf, uint32_t size, Core::Kernel& k) {
	if (0 == i) {
		buf = const_cast<char*>("0\0");
		return 2;
	}
	buf[size - 1] = '\0';
	k.write("Using smart_ptr buffer with properties :\n");
	k.write("Official size: ");
	k.write(indexes[buf.size()]);
	k.write("\nContent: \"");
	k.write(buf.get());
	k.write("\"\nLocal size: ");
	k.write(indexes[size]);
	k.write("\nStrlen: ");
	k.write(indexes[Core::strlen(buf.get())]);
	k.write("\nLast char: ");
	//k.write((unsigned)buf[size - 1], 1);

	for (uint32_t _i(size - 2); i; i /= 10, --_i) {
		char d_buf = ('0' + char(i % 10));
		k.write("Index: ");
		k.write(indexes[_i]);
		k.write(" Content: ");
		k.write(&d_buf, 1);

		buf[_i] = ('0' + char(i % 10));
		k.write("\n");
	}
	return size;
}

extern "C" {
	void main(Core::Kernel& k) {
		k.write("Starting KMOS...\n");
		k.displayCPU();
		k.write("\n");
	}

	void kernel_main(void)
	{
		Core::Kernel k;
		k.init();
		Core::__initMemory();
		main(k);
		//memtest(k);
		//k.write(">");
		//k.write(table[5]);
		//k.write("<");

		//return;

		Extensions::smart_ptr<char> a(16);
		unsigned p = (unsigned)a.get();
		unsigned h = (unsigned)&k;

		k.write("/c04STARTING..../c15\n");
		_sptuitoc_base(545464, a, 16, k);
		k.write("ENDED\n");
		//k.write("converted");
		//_uitoc_base(h, nullptr, 16, k);
		//k.write("\n", 1);
		//_uitoc_base(p, a.get(), 16, k);
		//k.write("chared");
		//k.write(a.get());
		//k.write("\ndisplayed");
		//k.write("/c04LOL\n");
		
	}
}

