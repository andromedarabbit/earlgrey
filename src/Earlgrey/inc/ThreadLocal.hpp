#pragma once

#ifdef __cplusplus_cli
#pragma unmanaged
#endif

#include "EarlgreyAssert.h"
#include <vector>
#include <functional>
#include <algorithm>


namespace Earlgrey
{
	template <typename T>
	class ThreadLocal
	{
	private:
		DWORD	_threadLocalIndex;

		ThreadLocal(ThreadLocal const&);

		void Allocate()
		{
			this->_threadLocalIndex = ::TlsAlloc();
		}

		T* Release()
		{
			T* ret = Get();
			::TlsFree( _threadLocalIndex );
			_threadLocalIndex = TLS_OUT_OF_INDEXES;
			return ret;
		}

	public:
		explicit ThreadLocal(T* pointer = NULL) : _threadLocalIndex(TLS_OUT_OF_INDEXES)
		{
			Allocate();
			if (pointer)
			{
				Set( pointer );
			}
		}

		virtual ~ThreadLocal()
		{
			Release();
		}

		T *Get(void)
		{
			if (TLS_OUT_OF_INDEXES == _threadLocalIndex)
			{
				// 초기화가 안됐으므로 프로그램을 종료하는 것이 좋다.
				return NULL;
			}
			return static_cast<T*>(::TlsGetValue(this->_threadLocalIndex));
		}

		void Set(T *value)
		{
			EARLGREY_ASSERT( TLS_OUT_OF_INDEXES != _threadLocalIndex );
			if (TLS_OUT_OF_INDEXES == _threadLocalIndex)
			{
				return;
			}
			::TlsSetValue(this->_threadLocalIndex, static_cast<void*>(value));
		}

		T* operator->()
		{
			return Get();
		}

		T& operator*()
		{
			return *Get();
		}
	};

	template<typename T>
	class ThreadLocalValue
	{
	public:
		explicit ThreadLocalValue() {}
		ThreadLocalValue(const T& value)
		{
			Set( value );
		}

		~ThreadLocalValue()
		{
			Free();
		}

		T* operator->()
		{
			T* pointer = _tls.Get();
			EARLGREY_ASSERT( pointer );
			return pointer;
		}

		ThreadLocalValue<T>& operator=(const T& value)
		{
			Set( value );
			return *this;
		}

		T& Get()
		{
			return *_tls.Get();
		}

		operator T&()
		{
			return Get();
		}

		T* operator&()
		{
			return _tls.Get();
		}

		bool operator !()
		{
			return this != NULL;
		}

	private:
		ThreadLocalValue(const ThreadLocalValue&);

		T* Allocate(const T& value)
		{
			return new T( value );
		}

		void Free() 
		{
			T* value = _tls.Get();
			if (value)
			{
				delete value;
			}
		}

		void Set(const T& value)
		{
			_tls.Set( Allocate( value ) );
		}

	private:
		ThreadLocal<T> _tls;
	};
}