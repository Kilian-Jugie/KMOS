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

		void* malloc(size_t s, size_t* futurSize = NULL) {
			if (s < 8) s = 8;
			else if (s % 2) s = rtpt(s);
			if (futurSize) *futurSize = s;
			const uint32_t l(log2_32(s) - 4);
			_mm_free_blk** array = _mm_core::getGblMdl()->frrblks[l];
			for (uint32_t i(0u); i < _mm_core::getGblMdl()->freeblksIndex[l]; ++i) {
				if (array[i]->size == s) {
					void* ret(array[i]->dataptr);
					array[i]->size = 0u;
					array[i]->dataptr = NULL;
					return ret;
				}
			}
			void* ret(_mm_core::getGblMdl()->fmptr);
			_mm_core::getGblMdl()->fmptr += s;
			return ret;
		}

		void free(void* ptr, size_t s, bool erase = false) {
			if (s < 8) s = 8;
			else if (s % 2) s = rtpt(s);
			if (erase) {
				for (uint32_t i(0u); i < s; ++i) {
					++ptr = NULL;
				}
			}
			const uint32_t l(log2_32(s) - 4);
			_mm_free_blk blk;
			blk.dataptr = ptr;
			blk.size = s;
			_mm_core::getGblMdl()->frrblks[l][_mm_core::getGblMdl()->freeblksIndex[l] + 1] = &blk;
		}

}

#endif