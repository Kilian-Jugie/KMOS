#include "kernel.hpp"

extern "C" {
	void kernel_main(void)
	{
		Core::Kernel k;
		k.init();
		Core::__initMemory();
		k.write("Bienvenu sur BranlOS\n");
		char* str = reinterpret_cast<char*>(Core::malloc(16));
		char* str2 = reinterpret_cast<char*>(Core::malloc(16));
		
		str[0] = 'B';
		str[1] = 'O';
		str[2] = 'N';
		str[3] = 'J';
		str[4] = 'O';
		str[5] = 'U';
		str[6] = 'R';
		str[7] = ' ';
		str[8] = 'l';
		str[9] = 'o';
		str[10] = 'l';
		str[11] = '\n';

		str2 = "excellent\n";

		k.write(str, 12);
		k.write(str2, 10);
		k.write(str, 12);

		Core::free(str2, 16);
		Core::free(str, 16);
		k.write("/c04CE TEXTE EST ROUGE !!!!\n");
	}
}