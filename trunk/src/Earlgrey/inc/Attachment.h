#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "xlist.h"

namespace Earlgrey
{
	namespace Mail
	{
		class Attachment : private Uncopyable
		{
		public:
			explicit Attachment(const TCHAR * fileName, const TCHAR * mediaType)
				: m_FileName(fileName)
				, m_MediaType(mediaType)
			{

			}

			inline const _txstring& FileName() const 
			{
				return m_FileName;
			}

			inline const _txstring& MediaType() const 
			{
				return m_MediaType;
			}

		private:
			_txstring m_FileName;
			_txstring m_MediaType;
		};

	} // !Mail


} // !Earlgrey