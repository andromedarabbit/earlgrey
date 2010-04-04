#pragma once
#include "Uncopyable.h"
#include "txstring.h"

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"

#include "taskqueue.h"
#include "xqueue.h"
#include "StringHelper.h"

namespace Earlgrey
{
	class Logger 		
		: public Algorithm::Lockfree::TaskQueue
	{
	private:
		typedef xwstring InternalString;
		typedef std::tr1::shared_ptr<InternalString> InternalStringPtr;
		typedef xqueue<InternalStringPtr>::Type Messages;

		friend struct Loki::CreateStatic<Logger>;
		explicit Logger()
			: m_Messages()
		{

		}

	public:			
		inline
			void Debug(const CHAR* msg, size_t len)
		{
			InternalStringPtr internalString( 
				new InternalString( 
					String::ToUnicode(msg, EARLGREY_NUMERIC_CAST<int>(len))
					, len 
					) 
				);
			AddMsgAsync(internalString);
		}

		inline
			void Debug(const WCHAR* msg, size_t len)
		{
			InternalStringPtr internalString( 
				new InternalString(msg, len) 
				);
			AddMsgAsync(internalString);
		}

 		template<class _Elem, class _Traits, class _Ax>
 		inline
			void Debug(const std::basic_string<_Elem, _Traits, _Ax>& msg)
		{
			Debug( msg.c_str(), msg.length() );
		}


	private: // methods
		void AddMsgAsync(InternalStringPtr msg)
		{
			InvokeMethod(&Logger::AddMsg_, msg);
		}

		void AddMsg_(InternalStringPtr msg)
		{
			m_Messages.push(msg);
		}

	private:
		Messages m_Messages;
		
	};

	typedef 
		Loki::SingletonHolder<Logger, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		LoggerSingleton;
}