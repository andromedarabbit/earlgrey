#include "stdafx.h"
#include "StackWriter.h"
#include "File.h"

#include <fstream>

using namespace std;

namespace
{
	class MyStackWalker : public StackWalker
	{
		explicit MyStackWalker();
		MyStackWalker& operator = (const MyStackWalker&);

	public:
		explicit MyStackWalker(ofstream& stream) 
			: StackWalker() 
			, m_Stream(stream)
		{
		}

		explicit MyStackWalker(ofstream& stream, DWORD dwProcessId, HANDLE hProcess) 
			: StackWalker(dwProcessId, hProcess) 
			, m_Stream(stream)
		{

		}

		virtual void OnOutput(LPCSTR szText) 
		{ 
			m_Stream << szText << endl;
			StackWalker::OnOutput(szText); 
		}

	private:
		ofstream& m_Stream;
	};
}

namespace Earlgrey
{
	const WCHAR * StackWriter::ClassName() const
	{
		return L"Earlgrey::StackWriter";
	}

	void StackWriter::HandleException(LPEXCEPTION_POINTERS exceptionPtrs)
	{
		const WCHAR * logFilePath = m_LogFilePath.c_str();
		if( File::Exists(logFilePath) )
		{
			if( File::Delete(logFilePath) )
			{
				// TODO: 오류 처리
				return;
			}
		}
 
		ofstream outfile(logFilePath);
	
		// 먼저 콜스택과 모듈들을 문자열로 만든다.
		MyStackWalker msw(outfile);
		if( !msw.ShowCallstack(GetCurrentThread(), exceptionPtrs->ContextRecord) )
		{
			// TODO: 오류 처리
			
		}
		
	}

}