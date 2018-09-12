#ifndef _SMARTPTR_H_GUARD
#define _SMARTPTR_H_GUARD
#include "../core/memory.h"

namespace Extensions {

	template<typename _Ty>class smart_ptr {
		_Ty* m_Adrr;
		size_t m_Size;
		size_t m_Index;

		constexpr void _free() {
			if (m_Size == 0)
				return;
			if (m_Adrr) {
				//delete[] m_Adrr;
				Core::free(m_Adrr, kmos::rtpt(m_Size));
				m_Adrr = NULL;
			}
		}

		constexpr void _alloc(size_t size) {
			//m_Adrr = new _Ty[size];
			void* tmp = Core::malloc(size);
			m_Adrr = (_Ty*)tmp;
		}

	public:
		constexpr smart_ptr(_Ty* addr, size_t size = 0u) : m_Adrr(addr), m_Size(size), m_Index(0u) {}
		constexpr smart_ptr(const size_t size) : m_Size(size), m_Index(0u) {
			_alloc(size);
		}
		constexpr smart_ptr() : m_Adrr(NULL), m_Size(0), m_Index(0u) {}

		constexpr void assign(_Ty* addr, size_t size, bool isArray, bool isDynamic) {
			if (!isDynamic) size = 0;
			_free();
			m_Size = isArray ? size : 0;
			m_Adrr = addr;
			return;
		}

		constexpr size_t size() const {
			return m_Size;
		}

		constexpr smart_ptr& fill(const _Ty& v) {
			for (uint32_t i(0u); i < m_Size; ++i)
				m_Adrr[i] = v;
			return *this;
		}

		constexpr char* get() const {
			return m_Adrr;
		}

		constexpr void realloc(size_t newSize) {
			_free();
			_alloc(newSize);
			m_Size = newSize;
		}

		constexpr smart_ptr& operator+=(_Ty v) {
			if(m_Index<m_Size)
				m_Adrr[m_Index] = v;
		}

		constexpr _Ty& last(uint32_t off = 0) {
			return m_Adrr[m_Size - 1 - off];
		}

		constexpr smart_ptr& operator=(const _Ty v) {
			if (m_Size > 0)
				m_Adrr[0] = v;
			else
				*m_Adrr = v;
			return *this;
		}

		constexpr _Ty& operator()() const {
			if (m_Size > 0)
				return m_Adrr[0];
			return *m_Adrr;
		}

		constexpr _Ty& operator[](const size_t i) {
			if (m_Size == 0) return *m_Adrr;
			return m_Adrr[i];
		}

		~smart_ptr() {
			_free();
		}

	};
}

#endif

