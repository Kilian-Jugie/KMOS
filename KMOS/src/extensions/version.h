#ifndef _VERSION_H_GUARD
#define _VERSION_H_GUARD

#include "smartptr.h"

#include <stdint.h>

#define VERS_STR(x) #x

namespace Extensions {

	class version {
	public:
		uint32_t m_Version, m_Size;
		smart_ptr<char> m_StrVersion;

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
			base(v, format);
		}

		void base(uint32_t vers, uint32_t versionFormat);
	};
}

#endif
