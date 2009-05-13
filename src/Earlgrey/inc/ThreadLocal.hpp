#pragma once

#ifdef __cplusplus_cli
#pragma unmanaged
#endif

namespace Earlgrey
{
	template <typename T>
	class ThreadLocal
	{
	private:
		DWORD threadLocalIndex;
		ThreadLocal(ThreadLocal const&);

		T *GetPointer(void)
		{
			return static_cast<T*>(::TlsGetValue(this->threadLocalIndex));
		}

		void SetPointer(T *value)
		{
			::TlsSetValue(this->threadLocalIndex, static_cast<void*>(value));
		}

		T t;

	public:
		void SetValue(const T &value)
		{
			T* currentPointer = this->GetPointer();

			if (currentPointer == NULL)
			{
				this->SetPointer(new T(value));
			}
			else
			{
				*currentPointer = value;
			}
		}

		T &GetValue(void)
		{
			T* currentPointer = this->GetPointer();

			if (currentPointer == NULL)
			{
				// modified
				this->SetPointer(new T(t));
			}

			return *this->GetPointer();
		}

		void DeleteValue()
		{
			T* currentPointer = this->GetPointer();

			if (currentPointer != NULL)
			{
				delete currentPointer;
				this->SetPointer(NULL);
			}
		}

		// explicit 
			ThreadLocal(const T& value)
		{
			this->threadLocalIndex = ::TlsAlloc();
			// modified
			this->t = value;
		}

		explicit
			ThreadLocal()
		{
			this->threadLocalIndex = ::TlsAlloc();
		}

		virtual ~ThreadLocal()
		{
			this->DeleteValue();
			::TlsFree(this->threadLocalIndex);
		}

		inline operator T&()
		{
			return GetValue();
		}

		inline bool operator !()
		{
			T* currentPointer = this->GetPointer();
			return currentPointer == NULL 
				|| *currentPointer != NULL;
		}

		inline T& operator->()
		{
			return GetValue();
		}

		inline ThreadLocal& operator=(const T& Value)
		{
			SetValue(Value);
			return *this;
		}
	};

}