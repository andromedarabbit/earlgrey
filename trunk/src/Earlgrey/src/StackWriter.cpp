#include "stdafx.h"
#include "StackWriter.h"
#include "File.h"

#ifdef HAS_STACK_WALKER
#include "..\..\..\vendor\StackWalker\StackWalker.h"
#endif

#include <fstream>

using namespace std;

namespace
{
#ifdef HAS_STACK_WALKER
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
#else
	class MyStackWalker 
	{
		explicit MyStackWalker();
		MyStackWalker& operator = (const MyStackWalker&);

	public:
		explicit MyStackWalker(ofstream&) 
		{
		}

		explicit MyStackWalker(ofstream&, DWORD, HANDLE) 			
		{
		}

		virtual void OnOutput(LPCSTR) 
		{ 
		}
	};
#endif
}

namespace Earlgrey
{
	void StackWriter::HandleException(LPEXCEPTION_POINTERS exceptionPtrs)
	{
		const TCHAR * logFilePath = m_LogFilePath.c_str();
		if( File::Exists(logFilePath) )
		{
			if( File::Delete(logFilePath) )
			{
				// TODO: ���� ó��
				return;
			}
		}
 
		ofstream outfile(logFilePath);
	
		// ���� �ݽ��ð� ������ ���ڿ��� �����.
		MyStackWalker msw(outfile);
		if( !msw.ShowCallstack(GetCurrentThread(), exceptionPtrs->ContextRecord) )
		{
			// TODO: ���� ó��
			
		}
		
	}

}