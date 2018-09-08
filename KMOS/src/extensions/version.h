#ifndef _VERSION_H_GUARD
#define _VERSION_H_GUARD

#include "../core/memory.h"
#include "../core/kmoslib.h"
#include <stdint.h>

#define VERS_STR(x) #x

namespace Extensions {
	class version {
		uint32_t m_Version;
		
	public:
		enum Format {
			MAJOR = 1,
			MINOR = 1 << 1,
			EDIT = 1 << 2,
			DOUBLE_MAJOR = 1 << 3,
			DOUBLE_MINOR = 1 << 4,
			DOUBLE_EDIT = 1 << 5,
			TRIPLE_MINOR = 1 << 6,
			TRIPLE_EDIT = 1 << 7
		};

		version(uint32_t v, uint32_t format) : m_Version(v) {

		}

		void base(uint32_t vers, uint32_t versionFormat) {
			uint32_t sizeMaj(0u), sizeMin(0u), sizeEdi(0u);
			if (MAJOR & versionFormat) {
				sizeMaj = 1;
			}
			else if (DOUBLE_MAJOR & versionFormat) {
				sizeMaj = 2;
			}
			
			if (MINOR & versionFormat) {
				sizeMin = 1;
			}
			else if (DOUBLE_MINOR & versionFormat) {
				sizeMin = 2;
			}
			else if (TRIPLE_MINOR & versionFormat) {
				sizeMin = 3;
			}

			if (EDIT & versionFormat) {
				sizeEdi = 1;
			}
			else if (DOUBLE_EDIT & versionFormat) {
				sizeEdi = 2;
			}
			else if (TRIPLE_EDIT & versionFormat) {
				sizeEdi = 3;
			}

		}
	};
}

#endif
