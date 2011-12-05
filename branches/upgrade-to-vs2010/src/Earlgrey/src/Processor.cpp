#include "stdafx.h"
#include "Processor.h"

namespace Earlgrey
{
	// TODO: 메모리 해제를 스마트 포인터로...
	namespace 
	{
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION GetSystemLogicalProcessorInformation(DWORD& returnLength)
		{
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL; //, ptr;
			returnLength = 0;

			for( ; ; )
			{
				if( ::GetLogicalProcessorInformation (buffer, &returnLength) == TRUE )
					break;

				const DWORD errorCode = GetLastError();
				EARLGREY_VERIFY(errorCode == ERROR_INSUFFICIENT_BUFFER);

				if (errorCode == ERROR_INSUFFICIENT_BUFFER) 
				{
					if (buffer) 
						::free(buffer);

					buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)::malloc(returnLength);
					EARLGREY_VERIFY(buffer != NULL); // Allocation failure
				} 
			}

			return buffer;
		}


		DWORD GetProcessorCount()
		{	
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;	
		}

		WORD GetProcessorCacheLineSize()
		{
			DWORD returnLength = 0;
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = GetSystemLogicalProcessorInformation(returnLength);
			EARLGREY_ASSERT(buffer != NULL);

			WORD cacheLineSize = 0;
			DWORD byteOffset = 0;

			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
			while (byteOffset < returnLength) 
			{
				switch (ptr->Relationship) 
				{
				case RelationCache:
					cacheLineSize = ptr->Cache.LineSize;
					::free(buffer);
					return cacheLineSize;

				default:
					break;
				}
				byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
				ptr++;
			}

			::free(buffer);
			throw std::exception("");
			// ::free(buffer);
			// return procCoreCount ;
		}


		DWORD GetActiveProcessorCoresCount()
		{
			DWORD returnLength = 0;
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = GetSystemLogicalProcessorInformation(returnLength);
			EARLGREY_ASSERT(buffer != NULL);

			DWORD procCoreCount = 0;
			DWORD byteOffset = 0;

			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
			while (byteOffset < returnLength) 
			{
				switch (ptr->Relationship) 
				{
				case RelationProcessorCore:
					procCoreCount++;
					break;

				default:
					break;
				}
				byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
				ptr++;
			}

			::free(buffer);
			return procCoreCount ;
		}

		//! \ref Hard code vol.1
		BOOL IsLittleEndianSystem()
		{
			union 
			{
				unsigned long bits32;
				unsigned char bytes[4];
			} theValue;

			theValue.bytes[0] = 0;
			theValue.bytes[1] = 1;
			theValue.bytes[2] = 0;
			theValue.bytes[3] = 0;

			return theValue.bits32 == 256;
		}
	}

	BOOL Processor::s_Initialized = FALSE;
	Processor::mutex_type Processor::s_InternalSyncObject;


	DWORD Processor::s_ProcessorCount = 0;
	DWORD Processor::s_ActiveProcessorCoresCount = 0;
	WORD Processor::s_ProcessorCacheLineSize = 0;
	BOOL Processor::s_IsLittleEndian = FALSE;

	void Processor::Initialize()
	{
		scoped_lock_type scopeLock(s_InternalSyncObject);

		s_ProcessorCount = GetProcessorCount();

		s_ProcessorCacheLineSize = GetProcessorCacheLineSize();

		s_ActiveProcessorCoresCount = GetActiveProcessorCoresCount();

		s_IsLittleEndian = IsLittleEndianSystem();

		s_Initialized = TRUE;
	}

	DWORD Processor::ProcessorCount()
	{	
		EARLGREY_ASSERT(s_Initialized == TRUE);
		return s_ProcessorCount;
	}

	//! \ref Hard code vol.1
	BOOL Processor::IsLittleEndian()
	{
		EARLGREY_ASSERT(s_Initialized == TRUE);
		return s_IsLittleEndian;
	}

	WORD Processor::ProcessorCacheLineSize()
	{
		EARLGREY_ASSERT(s_Initialized == TRUE);
		return s_ProcessorCacheLineSize;
	}

	DWORD Processor::ActiveProcessorCoresCount()
	{
		EARLGREY_ASSERT(s_Initialized == TRUE);
		return s_ActiveProcessorCoresCount;
	}
}