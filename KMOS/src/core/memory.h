#ifndef _MEMORY_H_GUARD
#define _MEMORY_H_GUARD
#include "maths.h"

/*		KMOS MEMORY MANAGMENT
 *
 * How it works:
 * -> Memory is requested by malloc with a size which we will call simply 'size'
 * -> Size is aligned to the upper power of 2 and with a minimum of 16 to get memory adresses null terminated (hexadecimal = 16 possibilities per char) (used for optimizations)
 * -> All empty spaces of memory are repertoried in frrblks of _mm_glb_mdl and sorted by size (21 size = last size 2^(4+21) = ~16Mo per block)
 * -> If a space is finded we reserved it
 * -> If no space are founded, we alloc a new space with fmptr who point to the end of used memory
 *
 * N.B: It is possible to use a larger space if needed but due to the memory wastage of this practice, it will be avoided at maximum
 */

#define MEMORY_SIZE_COUNT 21
#define MINIMUM_ALIGNMENT 16

namespace Core {
	namespace Memory {
		extern "C" {
			struct _mm_free_blk { //MeMory free BLocK
				void* dataptr;
				size_t size;
			};

			struct _mm_gbl_mdl {  //MeMory GloBaL MoDeL
				void* fmptr;	//Free Memory PoiTeR
				_mm_free_blk* frrblks[MEMORY_SIZE_COUNT][100000]; //Free blocks
				size_t freeblksIndex[MEMORY_SIZE_COUNT];

				_mm_gbl_mdl() {
					for (uint32_t i(0u); i < MEMORY_SIZE_COUNT; ++i)
						freeblksIndex[i] = 0u;
				}
			};

			struct _mm_core { //MeMory Core
			private:
				static _mm_gbl_mdl _gbl_mdl;

			public:
				inline static _mm_gbl_mdl* getGblMdl() {
					return &_gbl_mdl;
				}
			};

			
		}
	}

	using Memory::_mm_free_blk;
	using Memory::_mm_core;
	using Memory::_mm_gbl_mdl;
	using Maths::rtpt;
	using Maths::log2_32;

	_mm_gbl_mdl _mm_core::_gbl_mdl;

	extern "C" {
		void __initMemory() { //Init fmptr to a valable adresse
			int BEGIN;
			_mm_core::getGblMdl()->fmptr = &BEGIN;
		}

		/*
			Allocate s memory size and return a pointer to it
			futurSize = size after aligned
		*/
		void* malloc(size_t s, size_t* futurSize = NULL) {
			if (s < MINIMUM_ALIGNMENT) s = MINIMUM_ALIGNMENT;
			else if (s % 2) s = rtpt(s); //If s is not a power of 2, align it
			if (futurSize) *futurSize = s; //Put the aligned size to futurSize
			const uint32_t l(log2_32(s) - 4); //get the power of two used (-4 because minimum is 16=2^4)
			_mm_free_blk** array = _mm_core::getGblMdl()->frrblks[l]; //We get the power's correspondant array 
			for (uint32_t i(0u); i < _mm_core::getGblMdl()->freeblksIndex[l]; ++i) {
				if (array[i]->size == s) { //If a sufficient space is founded
					void* ret(array[i]->dataptr); //Save the space position
					array[i]->size = 0u; //Set size to 0 to make this block marked
					array[i]->dataptr = NULL; //Set the dataptr to null to hide possible futurs data
					return ret;
				}
			}
			void* ret(_mm_core::getGblMdl()->fmptr); //If no spaces are founded, we use the fmptr who point the free space begin
			_mm_core::getGblMdl()->fmptr += s; //We increment the fmptr by the size to still point the free space begin
			return ret;
		}

		void free(void* ptr, size_t s, bool erase = false) {
			if (s < MINIMUM_ALIGNMENT) s = MINIMUM_ALIGNMENT; //We align s to power of 2
			else if (s % 2) s = rtpt(s);
			if (erase) { //Erase is used with critical data who must leave no trace
				for (uint32_t i(0u); i < s; ++i) {
					++ptr = NULL;
				}
			}
			const uint32_t l(log2_32(s) - 4); //We get the correspondant power of 2
			_mm_free_blk blk; //We create a _mm_free_blk structure to mark the block empty
			blk.dataptr = ptr;
			blk.size = s;
			_mm_free_blk** array = _mm_core::getGblMdl()->frrblks[l];  //We get the power's correspondant array to put our new free block
			for (uint32_t i(0u); i < _mm_core::getGblMdl()->freeblksIndex[l]; ++i) { //We check if there is already an index occuped by a free space and if found, replace it
				if (array[i]->size == 0) {
					array[i] = &blk;
					return;
				}
			}
			_mm_core::getGblMdl()->frrblks[l][_mm_core::getGblMdl()->freeblksIndex[l]++] = &blk; //Si aucun espace n'est trouvé, on rajoute à la fin en pensant à incrémenter l'index des blocks libres
		}
	}

}

#endif