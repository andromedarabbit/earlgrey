#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "BasicBuffer.hpp"
#include "lexical_cast.hpp"

namespace Earlgrey
{
	class IniSection : private Uncopyable
	{
		friend class IniReader;

	private:
		explicit IniSection(const _txstring& filePath, const _txstring& name);

	public:
		typedef basic_buffer<TCHAR> Buffer;
		
		class TypeConverter : Uncopyable
		{
		public:
			explicit TypeConverter(const _txstring& src);

			template <typename T>
			T GetValue() const;
					
			template <>
			const _txstring& GetValue() const;

			const _txstring& operator*() const;

		private:
			_txstring m_Src;
		};

		const _txstring& Name() const ;

		template<typename T>
		T Read(const _txstring& keyName) const;

		template<>
		_txstring Read(const _txstring& keyName) const;

		TypeConverter operator[] (const _txstring& keyName) const;

	private: // methods
		_txstring ReadString(const _txstring& keyName) const;

	private:
		_txstring m_FilePath;
		_txstring m_Name;
		mutable Buffer m_Buffer;

	};


	// inline methods
	inline
		IniSection::IniSection(const _txstring& filePath, const _txstring& name)
		: m_FilePath(filePath)
		, m_Name(name)
		, m_Buffer()
	{
		EARLGREY_ASSERT(m_Name.empty() == false);
	}

	inline 
		const _txstring& IniSection::Name() const 
	{
		return m_Name;
	}

	inline 
		IniSection::TypeConverter IniSection::operator[] (const _txstring& keyName) const
	{
		return TypeConverter( Read<_txstring>(keyName) );
	}

	template<typename T>
	inline
		T IniSection::Read(const _txstring& keyName) const
	{
		_txstring valueStr = ReadString(keyName); 
		if(valueStr.empty())
			throw std::exception("");
		return String::lexical_cast<T, _txstring>(valueStr);
	}

	template<>
	inline 
		_txstring IniSection::Read(const _txstring& keyName) const
	{
		return ReadString(keyName);
	}

	// TypeConverter
	inline 
 	IniSection::TypeConverter::TypeConverter(const _txstring& src)
 		: m_Src(src)
 	{
 
 	}

	inline 
		const _txstring& IniSection::TypeConverter::operator*() const
	{
		return m_Src;
	}

	template <typename T>
	inline T IniSection::TypeConverter::GetValue() const
	{
		if(m_Src.empty())
			throw std::exception("");
		return String::lexical_cast<T, _txstring>(m_Src);
	}

	template <>
	inline const _txstring& IniSection::TypeConverter::GetValue() const
	{
		return m_Src;
	}

	template <>
	inline _txstring IniSection::TypeConverter::GetValue() const
	{
		return m_Src;
	}
}