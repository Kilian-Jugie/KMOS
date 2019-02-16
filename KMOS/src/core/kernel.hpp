#ifndef _KERNEL_H_GUARD
#define _KERNEL_H_GUARD
#include <stdbool.h>
#include "../extern/stddef.h"
#include "../extern/stdint.h"
#include "terminal.h"
#include "../extern/cpuid.h"
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

		void write(const char_t* ch, uint32_t size) {
			m_Terminal.write(ch, size);
		}

		void displayCPU() {
			union {
				char txt[32];
				uint32_t r[4];
			} buffer = { .txt = {0} };
			uint32_t id(0u);
			__cpuid(0, id, buffer.r[0], buffer.r[2], buffer.r[1]);
			write("CPU: ");
			bool unsupported(false);
			/*
				Vendor codes:
				"AMDisbetter!" – early engineering samples of AMD K5 processor
				"AuthenticAMD" – AMD
				"CentaurHauls" – Centaur (Including some VIA CPU)
				"CyrixInstead" – Cyrix
				"HygonGenuine" – Hygon
				"GenuineIntel" – Intel
				"TransmetaCPU" – Transmeta
				"GenuineTMx86" – Transmeta
				"Geode by NSC" – National Semiconductor
				"NexGenDriven" – NexGen
				"RiseRiseRise" – Rise
				"SiS SiS SiS " – SiS
				"UMC UMC UMC " – UMC
				"VIA VIA VIA " – VIA
				"Vortex86 SoC" – Vortex
			
			*/
			switch (buffer.txt[0]) //Full name comparison may too long so we compare certains characters which can are differents
			{
			case 'G': {
				switch (buffer.txt[11])
				{
				case 'l':
					write("Intel");
					break;
				case '6': {
					write("Transmeta");
					unsupported = true;
				}break;
				case 'C': {
					write("National Semiconductor");
					unsupported = true;
				}break;
				default: {
					write(buffer.txt);
					unsupported = true;
				}break;
				}
			}break;
			case 'A': 
				write("AMD");
			break;
			case 'C': {
				switch (buffer.txt[1]) {
				case 'y': {
					write("Cyrix");
					unsupported = true;
				}break;
				case 'e': {
					write("Centaur");
					unsupported = true;
				}break;
				default: {
					write("Unknown");
					unsupported = true;
				}break;
				}
			}break;
			case 'N': {
				write("NexGen");
				unsupported = true;
			}break;
			case 'R': {
				write("Rise");
				unsupported = true;
			}break;
			case 'U': {
				write("UMC");
				unsupported = true;
			}break;
			case 'S': {
				write("SiS");
				unsupported = true;
			}break;
			default: {
				write(buffer.txt);
				unsupported = true;
			}break;
			}
			write(" ");
			__cpuid(0x80000002, buffer.r[0], buffer.r[1], buffer.r[2], buffer.r[3]);
			write(buffer.txt);
			__cpuid(0x80000003, buffer.r[0], buffer.r[1], buffer.r[2], buffer.r[3]);
			write(buffer.txt);
			__cpuid(0x80000004, buffer.r[0], buffer.r[1], buffer.r[2], buffer.r[3]);
			write(buffer.txt);
			write("\n");
			if (unsupported)
				write(", WARNING: this CPU is not officially supported !!!"); //But can ((MAYBE)) work
		}
		 
		


	};
}

#endif