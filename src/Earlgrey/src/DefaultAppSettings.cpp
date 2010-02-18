#include "stdafx.h"
#include "DefaultAppSettings.h"
#include "Environment.h"
#include "EarlgreyProcess.h"
#include "Path.h"
#include "File.h"
#include "MiniDump.h"
#include "StackWriter.h"

#include <memory>

namespace Earlgrey
{

	namespace
	{
		std::tr1::shared_ptr<UnhandledExceptionHandler> GetMiniDump(const _txstring& appName)
		{
			const _txstring baseDir = Environment::BaseDirectory();
			const _txstring fileName(appName + _T(".dmp"));
			const _txstring filePath( Path::Combine(baseDir, fileName) );

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

		std::tr1::shared_ptr<UnhandledExceptionHandler> GetStackWriter(const _txstring& appName)
		{
			const _txstring baseDir = Environment::BaseDirectory();
			const _txstring fileName(appName + _T(".StackWriter.txt.dmp"));
			const _txstring filePath( Path::Combine(baseDir, fileName) );

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
		: m_executableName(Process::MainModuleFileName())
		, m_shortNmae(Path::GetFileName(m_executableName))
		, m_versionInfo( FileVersionInfo::GetVersionInfo(m_executableName) )
		, m_UnhandledExceptions(new UnhandledExceptionCollection())
	{
		EARLGREY_ASSERT(m_executableName.length() > 0);

		const _txstring appName(this->ShortName());

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
		return Environment::ProcessorCount();
		// return 2;
	}

	const TCHAR * const DefaultAppSettings::ShortName() const
	{
		return m_shortNmae.c_str();
	}

	const TCHAR * const DefaultAppSettings::LongName() const
	{
		return m_versionInfo.ProductName().c_str();
	}

	const TCHAR * const DefaultAppSettings::Description() const
	{	
		return m_versionInfo.FileDescription().c_str();
	}

	const TCHAR * const DefaultAppSettings::Version() const
	{	
		return m_versionInfo.ProductVersion().c_str();
	}


	
	AppSettings::UnhandledExceptionCollectionPtr DefaultAppSettings::UnhandledExceptions() 
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
	

}