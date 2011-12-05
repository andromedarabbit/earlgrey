#pragma once
#include "Uncopyable.h"
#include "txstring.h"

namespace Earlgrey
{
	class ProcessStartInfo : private Uncopyable
	{
	public:
		explicit ProcessStartInfo();
		explicit ProcessStartInfo(const _txstring& fileName);
		explicit ProcessStartInfo(const _txstring& fileName, const _txstring& arguments);


		inline _txstring& FileName()
		{
			return m_fileName;
		}

		inline const _txstring& FileName() const
		{
			return m_fileName;
		}

		inline void FileName(const _txstring& fileName)
		{
			m_fileName = fileName;
		}

		inline _txstring& Arguments()
		{
			return m_arguments;
		}

		inline const _txstring& Arguments() const
		{
			return m_arguments;
		}

		inline void Arguments(const _txstring& arguments)
		{
			m_arguments = arguments;
		}
	private:
		_txstring m_fileName;
		_txstring m_arguments;
	};

}