#ifndef _MEMORY_H_GUARD
#define _MEMORY_H_GUARD

//Align on 16 bits
#define _MEM_ALIGNMENT 16
#define _ALIGN(x) (((x) + (_MEM_ALIGNMENT-1)) & ~(_MEM_ALIGNMENT-1))

namespace Core {
	namespace Memory {
		extern "C" {
			struct _mm_free_blk { //MeMory free BLocK
				void** dataptr;
				size_t size;
			};

			struct _mm_gbl_mdl {  //MeMory GloBaL MoDeL
			private:
				void* fmptr;	//Free Memory PoiTeR
				_mm_free_blk frrblks[100000]; //Free blocks
				int freeblksIndex;

			public:
				void fwrd(size_t size) { //Forward
					fmptr += size;
				}
			};

			struct _mm_core { //MeMory Core
			private:
				static _mm_gbl_mdl _gbl_mdl;

			public:
				inline static const _mm_gbl_mdl* getGblMdl() {
					return &_gbl_mdl;
				}
			};
		}
	}

	class _fnctr_malloc {
	public:
		public void* operator()(size_t size) {

		}
	};

	_fnctr_malloc malloc;
}

#endif