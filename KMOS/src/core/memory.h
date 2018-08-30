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

			struct _mm_gbl_mdl { //MeMory GloBaL MoDeL
			private:
				void* fmptr;	//Free Memory PoinTer
				void** frrblks[100000]; //Free Blocks
				int freeblksIndex;

			public:
				void fwrd(size_t size) { //Froward memory pointer of size
					fmptr += size;
				}
			};
		}
	}
}

#endif