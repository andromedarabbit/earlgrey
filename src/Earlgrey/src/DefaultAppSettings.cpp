#include "stdafx.h"
#include "DefaultAppSettings.h"
#include "Environment.h"
#include "Processor.h"
#include "EarlgreyProcess.h"
#include "Path.h"
#include "File.h"
#include "MiniDump.h"
#include "StackWriter.h"
#include "Thread.h"

#include <memory>

namespace Earlgrey
{

	namespace
	{
		std::tr1::shared_ptr<UnhandledExceptionHandler> GetMiniDump(const xwstring& appName)
		{
			const xwstring baseDir = Environment::BaseDirectoryW();
			const xwstring fileName(appName + L".dmp");
			const xwstring filePath( Path::Combine(baseDir, fileName) );

			if( File::Exists(filePath) )
			{
				EARLGREY_ASSERT( File::Delete(filePath) );
			}

			const MINIDUMP_TYPE dumpType = MiniDumpNormal;

			return std::tr1::shared_ptr<UnhandledExceptionHandler>( 
				new MiniDump(filePath.c_str(), dumpType) 
				);

			/*
			miniDump->AddExtendedMessage(
			static_cast<MINIDUMP_STREAM_TYPE>(LastReservedStream + 1)
			, _T("사용자 정보 1")
			);
			*/

			// GlobalExceptionHandler::Register(miniDump);
		}

		std::tr1::shared_ptr<UnhandledExceptionHandler> GetStackWriter(const xwstring& appName)
		{
			const xwstring baseDir = Environment::BaseDirectoryW();
			const xwstring fileName(appName + L".StackWriter.txt.dmp");
			const xwstring filePath( Path::Combine(baseDir, fileName) );

			if( File::Exists(filePath) )
			{
				EARLGREY_ASSERT( File::Delete(filePath) );
			}

			std::tr1::shared_ptr<UnhandledExceptionHandler> writer( 
				new StackWriter(filePath, StackWalker::OptionsAll) 
				);
			return writer;
		}
	}


	DefaultAppSettings::DefaultAppSettings()
		: m_executableName(Process::MainModuleFileNameW())
		, m_shortName(Path::GetFileName(m_executableName))
		, m_versionInfo( FileVersionInfo::GetVersionInfo(m_executableName) )
		, m_UnhandledExceptions(new UnhandledExceptionCollection())
	{
		EARLGREY_ASSERT(m_executableName.length() > 0);

		const xwstring appName(this->ShortName());

		std::tr1::shared_ptr<UnhandledExceptionHandler> miniDump( GetMiniDump(appName) );
		m_UnhandledExceptions->push_back(miniDump);

		std::tr1::shared_ptr<UnhandledExceptionHandler> stackWriter( GetStackWriter(appName) );
		m_UnhandledExceptions->push_back(stackWriter);
	}

	DefaultAppSettings::~DefaultAppSettings()
	{

	}

	BOOL DefaultAppSettings::AllowOnlyOneInstance() const
	{
		return TRUE;
	}

	DWORD DefaultAppSettings::NumberOfIOThreads() const
	{
		return Processor::ProcessorCount();
	}

	DWORD DefaultAppSettings::NumberOfConcurrentIOThreads() const
	{
		return NumberOfIOThreads();
	}

	const WCHAR * const DefaultAppSettings::ShortName() const
	{
		return m_shortName.c_str();
	}

	const WCHAR * const DefaultAppSettings::LongName() const
	{
		return m_versionInfo.ProductName().c_str();
	}

	const WCHAR * const DefaultAppSettings::Description() const
	{	
		return m_versionInfo.FileDescription().c_str();
	}

	const WCHAR * const DefaultAppSettings::Version() const
	{	
		return m_versionInfo.ProductVersion().c_str();
	}


	
	AppSettings::UnhandledExceptionCollectionPtr DefaultAppSettings::UnhandledExceptions() const
	{
		return m_UnhandledExceptions;
	}

	/*
	AppSettings::UnhandledExceptionCollection& DefaultAppSettings::UnhandledExceptions()
	{
		return m_UnhandledExceptions;
	}

	const AppSettings::UnhandledExceptionCollection& DefaultAppSettings::UnhandledExceptions() const
	{
		return m_UnhandledExceptions;
	}
	*/

// 	ThreadIdType DefaultAppSettings::LoggingThreadId() const
// 	{
// 		return WIN_MAIN_THREAD_ID;
// 	}
	
	BOOL DefaultAppSettings::IsThisLoggingThread() const
	{
		return Thread::CurrentThread()->ThreadId() == WIN_MAIN_THREAD_ID;
	}

	TimeSpan DefaultAppSettings::LoggingFlushInterval() const
	{
		static TimeSpan interval(TimeSpan::FromSeconds(1));
		return interval;
	}
}