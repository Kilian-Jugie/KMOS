#ifndef _SMARTPTR_H_GUARD
#define _SMARTPTR_H_GUARD
#include "../core/memory.h"

namespace Extensions {

	template<typename _Ty>class smart_ptr {
		_Ty* m_Adrr;
		size_t m_Size;

		void _free() {
			if (m_Size == 0)
				return;
			if (m_Adrr) {
				delete[] m_Adrr;
				m_Adrr = NULL;
			}
		}

	public:
		smart_ptr(_Ty* addr, size_t size = 0u) : m_Adrr(addr), m_Size(size) {}
		smart_ptr(const size_t size) : m_Size(size) {
			m_Adrr = new _Ty[size];
		}
		smart_ptr() : m_Adrr(NULL), m_Size(0) {}

		void assign(_Ty* addr, size_t size, bool isArray, bool isDynamic) {
			if (!isDynamic) size = 0;
			_free();
			m_Size = isArray ? size : 0;
			m_Adrr = addr;
			return;
		}


		smart_ptr& operator=(const _Ty v) {
			if (m_Size > 0)
				m_Adrr[0] = v;
			else
				*m_Adrr = v;
			return *this;
		}

		_Ty& operator()() {
			if (m_Size > 0)
				return m_Adrr[0];
			return *m_Adrr;
		}

		_Ty& operator[](const size_t i) {
			if (m_Size == 0) return *m_Adrr;
			return m_Adrr[i];
		}

		~smart_ptr() {
			_free();
		}

	};
}

#endif

