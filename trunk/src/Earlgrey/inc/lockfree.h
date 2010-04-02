#pragma once
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <functional>
#include "EarlgreyAssert.h"
#include "Uncopyable.h"


namespace Earlgrey { namespace Algorithm { 

	inline BOOL CAS(volatile LONG *Dest, LONG Compare, LONG Exchange)
	{   
		return InterlockedCompareExchange(Dest, Exchange, Compare) == Compare;
	}

	inline BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
	{   
#ifdef _WIN64
		return InterlockedCompareExchange64(Dest, Exchange, Compare) == Compare;
#else
		BOOL Return = TRUE;
		__asm
		{		
			push ecx
			push ebx
			push eax
			push edx
			push edi

			mov edx, dword ptr [Compare] + 4
			mov eax, dword ptr [Compare]
			mov ecx, dword ptr [Exchange] + 4
			mov ebx, dword ptr [Exchange]
			mov edi, Dest
			//	Compare (EDX:EAX) with operand, if equal (ECX:EBX) is stored
			lock cmpxchg8b qword ptr [edi]
			pop edi
			pop edx
			pop eax
			pop ebx
			pop ecx
			je	success
			mov dword ptr [Return], 0		
success:
		}
		return Return;
#endif
	}
}}

namespace Earlgrey { namespace Algorithm { namespace Lockfree {



	// http://msdn.microsoft.com/en-us/library/aa366778.aspx
	// assume that user-mode virtual address space is 7TB (using just low 42 bits),
	// we won't use high 20 bits (more precisely 22 bits).
	// So, we use high 20 bits as a counter;
	static const ULONGLONG maskOfCounter64 = 0xfffff00000000000;
	static const ULONGLONG maskOfPointer64 = 0x00000fffffffffff;


	template<typename T>
	union Pointer
	{
		explicit Pointer() {}

		explicit Pointer(typename T* pointer, ULONG count) {

			p(pointer);
			Count(count);
		}

		explicit Pointer(typename T* pointer) {
			p(pointer);
			Count(Count() + 1);

		}
		volatile ULONGLONG val64;

#ifdef _WIN64
		struct {
			ULONGLONG p : 44;		// low 44 bits for pointer
			ULONGLONG count : 20;	// high 20 bits for counter
		} val;

		inline typename T* p() const
		{
			return reinterpret_cast<T*>( val.p );
		}

		inline void p(typename T* pointer)
		{
			EARLGREY_ASSERT((reinterpret_cast<ULONGLONG>(pointer) & maskOfCounter64) == 0);
			val.p = reinterpret_cast<ULONGLONG>(pointer);
		}

		inline ULONG Count() const
		{
			return (ULONG)val.count;
		}

		inline void Count(ULONG count)
		{
			val.count = count;
		}
#else
		struct {
			T* p;
			ULONG count;
		} val;

		inline typename T* p() const
		{
			return (T*)val.p;
		}

		inline void p(typename T* pointer)
		{
			val.p = pointer;
		}

		inline ULONG Count() const
		{
			return (ULONG)val.count;
		}

		inline void Count(ULONG count)
		{
			val.count = count;
		}
#endif
		inline ULONG NextCount() const
		{
			return Count() + 1;
		}
	};


	template<class T>
	inline bool operator==(const Pointer<T>& A, const Pointer<T>& B)
	{
		return A.val64 == B.val64;
	}

	template<class T>
	inline bool operator!=(const Pointer<T>& A, const Pointer<T>& B)
	{
		return A.val64 != B.val64;
	}



	template<typename T>
	struct Cell
	{
		typedef struct Cell<T> CellType;

		typedef union Pointer<CellType> PointerType;
		
		explicit Cell(CellType* next, T value)
		{
			this->next.p(next);
			this->value = value;
		}

		explicit Cell(T value)
		{
			this->next.p(NULL);
			this->value = value;
		}

		explicit Cell() { this->next.p(NULL); }

		PointerType		next;
		T				value;
		
	};

	//! Cell 의 포인터를 관리한다. 
	/*!
		lockfree 알고리즘에서 cell 객체를 delete할 때 문제가 발생하는데, 그 이유는 다른 스레드에서
		삭제된 객체를 참조할 수 있기 때문이다. PointerPool 클래스는 cell 객체를 delete 하지 않고 pool에 저장한다.
		그리고 pool 에 있는 객체를 재사용한다.
		주의-1 : 이 클래스는 객체의 생성자와 파괴자를 호출하지 않기 때문에 주의해야 한다.
		주의! 이 클래스는 TLS 메모리로 관리돼야 한다.
	*/
	template<typename T, class Allocator = std::allocator<T>>
	class PointerPool
	{
		typedef Cell<T>		CellType;
		typedef CellType*	CellPointer;
		typedef typename Allocator::rebind<CellType>::other AllocType;

		typedef std::vector<CellPointer, typename Allocator::rebind<CellPointer>::other> HazardPointerListType;
	public:
		~PointerPool()
		{
			HazardPointerListType::const_iterator iter = _cellList.begin();
			for (; iter != _cellList.end(); iter++)
			{
				_allocator.deallocate( *iter, 0 );
			}
		}

		//! cell pointer를 할당한다. 이미 풀에 있다면 풀에서 가져온다.
		CellPointer Allocate()
		{
			if (_cellList.empty())
			{
				CellPointer pointer = new(_allocator.allocate(1)) CellType();
				return pointer;
			}

			CellPointer pointer = _cellList.back();
			_cellList.pop_back();

			// initialize the object
			pointer->next.p( NULL );
			pointer->value = T();

			return pointer;
		}

		//! cell 을 풀에 저장한다.
		void Release(CellPointer pointer)
		{
			_cellList.push_back( pointer );
		}

	private:
		HazardPointerListType _cellList;	//!< 포인터 리스트를 저장한다.
		AllocType _allocator;				//!< 할당자 std::vector 에서도 사용하는 타입이다.
	};

}}}