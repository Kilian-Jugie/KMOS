/* 
	This work is licensed under the Creative Commons Attribution - NonCommercial - NoDerivatives 4.0 International License.
	To view a copy of this license, visit http ://creativecommons.org/licenses/by-nc-nd/4.0/
	or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/

#ifndef _MEMORY_H_GUARD
#define _MEMORY_H_GUARD
#include "kmoslib.h"

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
#define MINIMUM_ALIGNMENT 16 //May not be changed ! (for optimization)

//ISO C++ forbid void* increment
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"

namespace Core {
	namespace Memory {
		extern "C" {
			/*
				A free memory block
			*/
			struct _mm_free_blk { //MeMory free BLocK
				void* dataptr; //It adress
				uint32_t size; //It size (2^aligned)
			};

			/*
				A memory model: how the memory is represented for C
			*/
			struct _mm_gbl_mdl {  //MeMory GloBaL MoDeL
				void* fmptr;	//Free Memory PoiTeR
				_mm_free_blk* frrblks[MEMORY_SIZE_COUNT][100000]; //Free blocks
				uint32_t freeblksIndex[MEMORY_SIZE_COUNT]; //Next indext to store block pointer in freeblks of each size

				//Initializing indexes to 0 to avoid nullptrexception or anything else
				_mm_gbl_mdl() {
					for (uint32_t i(0u); i < MEMORY_SIZE_COUNT; ++i)
						freeblksIndex[i] = 0u;
				}
			};

			/*
				Memory with the unique memory model instance
				(used as gobal variable) but aligned thanks to struct
			*/
			struct _mm_core { //MeMory Core
			private:
				static _mm_gbl_mdl _gbl_mdl; //Memory

			public:
				inline static _mm_gbl_mdl* getGblMdl() { //Getter
					return &_gbl_mdl;
				}
			};

			
		}
	}

	using Memory::_mm_free_blk;
	using Memory::_mm_core;
	using Memory::_mm_gbl_mdl;
	using kmos::rtpt;
	using kmos::log2_32;

	_mm_gbl_mdl _mm_core::_gbl_mdl;

	extern "C" {
		void __initMemory() { //Init fmptr to a valable adresse
			int BEGIN;
			_mm_core::getGblMdl()->fmptr = &BEGIN; //DANGEROUS !!! -> TO CHANGE 
		}

		/*
			Allocate s memory size and return a pointer to it
			futurSize = size after aligned
		*/
		void* malloc(uint32_t s, uint32_t* futurSize = 0) {
			if (s < MINIMUM_ALIGNMENT) s = MINIMUM_ALIGNMENT;
			else s = rtpt(s); //We align s on power of 2
			if (futurSize) *futurSize = s; //Put the aligned size to futurSize
			const uint32_t l(log2_32(s) - 4); //get the power of two used (-4 because minimum is 16=2^4)
			_mm_free_blk** array = _mm_core::getGblMdl()->frrblks[l]; //We get the power's correspondant array 
			for (uint32_t i(0u); i < _mm_core::getGblMdl()->freeblksIndex[l]; ++i) {
				if (array[i]->size == s) { //If a sufficient space is founded
					void* ret(array[i]->dataptr); //Save the space position
					array[i]->size = 0u; //Set size to 0 to make this block marked
					array[i]->dataptr = 0; //Set the dataptr to null to hide possible futurs data
					return ret;
				}
			}
			void* ret(_mm_core::getGblMdl()->fmptr); //If no spaces are founded, we use the fmptr who point the free space begin
			_mm_core::getGblMdl()->fmptr += s; //We increment the fmptr by the size to still point the free space begin
			return ret;
		}

		void __aligned_free(void* ptr, uint32_t s, bool erase = false) {
			if (erase) { //Erase is used with critical data who must leave no trace
				for (uint32_t i(0u); i < s; ++i) {
					++ptr = 0;
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
			//TODO: check if array isn't full
			_mm_core::getGblMdl()->frrblks[l][_mm_core::getGblMdl()->freeblksIndex[l]++] = &blk; //If no spaces were founded, we add a new freeblock at the end of the array
		}

		void free(void* ptr, uint32_t s, bool erase = false) {
			if (s < MINIMUM_ALIGNMENT) s = MINIMUM_ALIGNMENT; //We align s to power of 2
			else s = rtpt(s);
			__aligned_free(ptr, s, erase);
		}

		
	}

}

#pragma GCC diagnostic pop

#endif