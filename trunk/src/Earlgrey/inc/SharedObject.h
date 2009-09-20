#pragma once

#include "Uncopyable.h"

namespace Earlgrey {

	//! \todo TLS Memory 로 변경 필요
	template<class T>
	struct DefaultCreator
	{
		static T* Create()
		{
			return new T;
		}
	};

	//! \todo TLS Memory 로 변경 필요
	template<class T>
	struct DefaultDeleter 
	{
		static void Delete(T* p)
		{
			delete p;
		}
	};

	struct ReferenceCount
	{
		ReferenceCount() : _Count(0)
		{
		}

		long AddRef() 
		{
			return InterlockedIncrement( &_Count );
		}

		long Release()
		{
			return InterlockedDecrement( &_Count );
		}

		volatile long _Count;
	};

	//! \todo TLS Memory 로 변경 필요
	struct DefaultRefCountHelper
	{
		static ReferenceCount* Create()
		{
			return new ReferenceCount();
		}

		static void Delete(ReferenceCount* p)
		{
			delete p;
		}
	};


	template<
		class T, 
		template<class> class Deleter = DefaultDeleter,
		class RefCountHelper = DefaultRefCountHelper
	>
	class SharedPtr
	{
	public:
		typedef T ObjectType;

		SharedPtr() : _p(NULL), _RefCount(NULL)
		{
		}

		explicit SharedPtr(ObjectType* p) : _p(NULL), _RefCount(NULL)
		{
			_ResetP( p );
		}

		SharedPtr(const SharedPtr& ptr) : _p(NULL), _RefCount(NULL)
		{
			_Reset( ptr );
		}

		template<class U>
		SharedPtr(const SharedPtr<U>& ptr) : _p(NULL), _RefCount(NULL)
		{
			_Reset( ptr );
		}

		~SharedPtr()
		{
			Release();
		}

		SharedPtr& operator=(const SharedPtr& ptr)
		{
			_Reset( ptr );
			return *this;
		}

		template<class U>
		SharedPtr& operator=(const SharedPtr<U>& ptr)
		{
			_Reset( ptr );
			return *this;
		}

		void Reset(ObjectType* p = NULL)
		{
			_ResetP( p );
		}

		T* operator->() const
		{
			return _p;
		}

		operator T*() const
		{
			return _p;
		}

		T* Get() const
		{
			return _p;
		}

		bool IsUnique() const
		{
			if (!_p)
			{
				return false;
			}
			return _RefCount->_Count == 1;
		}

		LONG UseCount() const
		{
			if (!_p)
			{
				return 0L;
			}
			return _RefCount->_Count;
		}

		void Swap(SharedPtr& ptr)
		{
			::std::swap( _p, ptr._p );
			::std::swap( _RefCount, ptr._RefCount );
		}

		template<class T, class U>
		friend bool operator<(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs)
		{
			return lhs.UseCount() < rhs.UseCount();
		}
		
	private:
		template<class U>
		void _ResetP(U* p)
		{
			Release();
			if (!p)
			{
				return;
			}
			_RefCount = RefCountHelper::Create();
			_p = p;
			AddRef();
		}

		template<class U>
		void _Reset(const SharedPtr<U>& ptr)
		{
			Release();
			if (!ptr.Get())
			{
				return;
			}
			_p = ptr._p;
			_RefCount = ptr._RefCount;
			AddRef();
		}

		void AddRef() 
		{
			if (_p)
			{
				_RefCount->AddRef();
			}
		}

		void Release()
		{
			if (_p && _RefCount->Release() <= 0)
			{
				Deleter<T>::Delete( _p );
				RefCountHelper::Delete( _RefCount );
			}
			_p = NULL;
			_RefCount = NULL;
		}

		ObjectType* _p;
		ReferenceCount* _RefCount;
	};

	template<class T, class U>
	bool operator==(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs)
	{
		return lhs.Get() == rhs.Get();
	}

	template<class T, class U>
	bool operator!=(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs)
	{
		return !(lhs == rhs);
	}
	
}