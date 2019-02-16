#include "version.h"
#include "../core/kmoslib.h"

namespace Extensions {
	void version::base(uint32_t vers, uint32_t versionFormat) {
		uint32_t sizeMaj(0u), sizeMin(0u), sizeEdi(0u), types(0u), off(0u);
		if (MAJOR & versionFormat) { //TODO: optimizing this because it's very very ugly
			sizeMaj = 1;
			++types;
		}
		else if (DOUBLE_MAJOR & versionFormat) {
			sizeMaj = 2;
			++types;
		}

		if (MINOR & versionFormat) {
			sizeMin = 1;
			++types;
		}
		else if (DOUBLE_MINOR & versionFormat) {
			sizeMin = 2;
			++types;
		}
		else if (TRIPLE_MINOR & versionFormat) {
			sizeMin = 3;
			++types;
		}

		if (EDIT & versionFormat) {
			sizeEdi = 1;
			++types;
		}
		else if (DOUBLE_EDIT & versionFormat) {
			sizeEdi = 2;
			++types;
		}
		else if (TRIPLE_EDIT & versionFormat) {
			sizeEdi = 3;
			++types;
		}
		--types;
		m_Size = sizeEdi + sizeMin + sizeMaj;
		m_StrVersion.realloc(m_Size + 1 + types);
		m_StrVersion.fill('0');

		m_StrVersion[m_Size + types] = '\0';
		smart_ptr<char> versionStr(m_Size + 1);
		versionStr.fill('0');
		kmos::uitoc(vers, versionStr.get(), m_Size);

		versionStr.last() = '\0';
		for (uint32_t i(0u); i < sizeEdi; ++i)
			m_StrVersion.last(i + 1) = versionStr.last(i + 1);
		if (types) {
			++off;
			--types;
			m_StrVersion.last(sizeEdi + 1) = '.';
		}
		for (uint32_t i(0u); i < sizeMin; ++i)
			m_StrVersion.last(i + sizeEdi + 1 + off) = versionStr.last(i + sizeEdi + 1);
		if (types) {
			++off;
			m_StrVersion.last(sizeEdi + sizeMin + off) = '.';
		}
		for (uint32_t i(0u); i < sizeMaj; ++i)
			m_StrVersion.last(i + sizeEdi + sizeMin + 1 + off) = versionStr.last(i + sizeEdi + sizeMin + 1);

	}
}