#pragma once
namespace Earlgrey
{

#ifdef _WIN64
	template<class PointerType>
	struct LockfreePointer
	{
		volatile UInt64		Alignment;
		static const UInt64 COUNT_BITMASK	= 0xFFFFFF0000000000;
		static const UInt64 POINTER_BITMASK	= 0x000000FFFFFFFFFF;
		static const UInt64 COUNT_UNIT		= 0x0000010000000000;

		explicit LockfreePointer()
		{
			SetPointer(NULL);
		}

		explicit LockfreePointer(PointerType *InPointer, UInt64 InCount)
		{
			Alignment = (reinterpret_cast<ULONGLONG>(InPointer) & POINTER_BITMASK) | (COUNT_BITMASK & InCount);
		}

		inline PointerType* GetPointer() const
		{
			return reinterpret_cast<PointerType*>(Alignment & POINTER_BITMASK);
		}

		inline void SetPointer(PointerType* InPointer)
		{
			Alignment = ((Alignment + COUNT_UNIT) & COUNT_BITMASK) | (reinterpret_cast<ULONGLONG>( InPointer ) & POINTER_BITMASK);
		}

		inline UInt64 GetCount() const
		{
			return (Alignment & COUNT_BITMASK);
		}

		inline UInt64 GetNextCount() const
		{
			return (Alignment + COUNT_UNIT) & COUNT_BITMASK;
		}
	};
#else

	template<class PointerType>
	union LockfreePointer
	{
		volatile UInt64	Alignment;
		struct _SafePointer
		{
			PointerType	*Pointer;
			UInt32		Count;
		} SafePointer;

		explicit LockfreePointer()
		{
			SafePointer.Pointer = 0;
		}

		explicit LockfreePointer(PointerType *InPointer, UInt64 InCount)
		{
			SafePointer.Pointer = InPointer;
			SafePointer.Count	= InCount;
		}

		inline PointerType* GetPointer() const
		{
			return SafePointer.Pointer;
		}

		inline void SetPointer(PointerType* Pointer)
		{
			SafePointer.Count++;
			SafePointer.Pointer = Pointer;
		}

		inline UInt32 GetCount() const
		{
			return SafePointer.Count;
		}

		inline UInt32 GetNextCount() const
		{
			return SafePointer.Count + 1;
		}
	};
#endif


	template<class PointerType>
	inline BOOL operator==(const LockfreePointer<PointerType>& A, const LockfreePointer<PointerType>& B)
	{
		return A.Alignment == B.Alignment;
	}

}