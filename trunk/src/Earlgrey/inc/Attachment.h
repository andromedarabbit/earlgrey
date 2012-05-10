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
			explicit Attachment(const WCHAR * fileName, const WCHAR * mediaType)
				: m_FileName(fileName)
				, m_MediaType(mediaType)
			{

			}

			inline const xwstring& FileName() const 
			{
				return m_FileName;
			}

			inline const xwstring& MediaType() const 
			{
				return m_MediaType;
			}

		private:
			xwstring m_FileName;
			xwstring m_MediaType;
		};

	} // !Mail


} // !Earlgrey