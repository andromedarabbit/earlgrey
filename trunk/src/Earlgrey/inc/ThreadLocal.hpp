#pragma once

#ifdef __cplusplus_cli
#pragma unmanaged
#endif

#include "EarlgreyAssert.h"
#include <vector>
#include <functional>
#include <algorithm>
#include "loki/Threads.h"
#include "ScopedLock.h"


namespace Earlgrey
{
	template <typename T>
	class ThreadLocal
	{
	private:
		ThreadLocal(ThreadLocal const&);

		DWORD _threadLocalIndex;
		::Loki::Mutex _lock;

		//! dtor에서만 호출한다.
		T* Release()
		{
			T* ret = Get();
			::TlsFree( GetIndex() );
			return ret;
		}

	public:
		ThreadLocal() : _threadLocalIndex(TLS_OUT_OF_INDEXES)
		{
		}

		virtual ~ThreadLocal()
		{
			Release();
		}

		DWORD GetIndex()
		{
			if (_threadLocalIndex == TLS_OUT_OF_INDEXES)
			{
				ScopedLock<> lock( _lock );
				if (_threadLocalIndex == TLS_OUT_OF_INDEXES)
				{
					_threadLocalIndex = ::TlsAlloc();
				}
			}

			return _threadLocalIndex;
		}

		T* Get(void)
		{
			EARLGREY_ASSERT( GetIndex() != TLS_OUT_OF_INDEXES );
			return static_cast<T*>( ::TlsGetValue( GetIndex() ) );
		}

		void Set(T *value)
		{
			::TlsSetValue(GetIndex(), static_cast<void*>(value));
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
		ThreadLocalValue() {}
		ThreadLocalValue(const T& value) : _initialValue(value)
		{
			Set( value );
		}

		~ThreadLocalValue()
		{
			Free();
		}

		T* operator->()
		{
			return GetPointer();
		}

		ThreadLocalValue<T>& operator=(const T& value)
		{
			Set( value );
			return *this;
		}

		T& Get()
		{
			return *GetPointer();
		}

		operator T&()
		{
			return Get();
		}

		T* operator&()
		{
			return GetPointer();
		}

		bool operator !()
		{
			return Get() == NULL;
		}

		bool IsAllocated() 
		{
			return _tls.Get() != NULL;
		}

	private:
		ThreadLocalValue(const ThreadLocalValue&);

		T* Allocate(const T& value)
		{
			return new T( value );
		}

		T* GetPointer()
		{
			T* pointer = _tls.Get();
			if (!pointer)
			{
				return SetPointer( Allocate( _initialValue ) );
			}
			return pointer;
		}

		T* SetPointer(T* pointer)
		{
			_tls.Set( pointer );
			return pointer;
		}

		void Free() 
		{
			T* pointer = GetPointer();
			if (pointer)
			{
				delete pointer;
			}
		}

		void Set(const T& value)
		{
			T* pointer = GetPointer();
			EARLGREY_ASSERT(pointer);
			*pointer = value;
		}

	private:
		ThreadLocal<T>  _tls;
		T				_initialValue;
	};

#ifdef __cplusplus_cli
#pragma managed
#endif
}