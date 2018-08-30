#include "kernel.hpp"

extern "C" {
	void kernel_main(void)
	{
		Core::Kernel k;
		k.init();

		//k.write("Bienvenu sur BranlOS\n");
		//k.write("CE TEXTE N'EST PAS ROUGE !!!!\n");
		int* testPtr = NULL;

		while (++testPtr) {
			k.write((char*)((*testPtr)+'0'));
		}
	}
}