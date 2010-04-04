#pragma once
#include "xlist.h"

namespace Earlgrey
{
namespace ADO
{
namespace Test
{



	// template <RawADO::DataTypeEnum DataType>//, LONG DefinedSize>
	template <typename DataType>//, LONG DefinedSize>
	class Column : private Uncopyable
	{
		friend std::ostream& operator << (std::ostream& stream, const Column<DataType>& value)
		{
			stream << value.m_Value;
			return stream;
		}

		//! \ref http://www.parashift.com/c++-faq-lite/templates.html#faq-35.16
		friend bool operator == <> (const Column<DataType>& column, const DataType& value);
		friend bool operator == <> (const DataType& value, const Column<DataType>& column);
		friend bool operator != <> (const Column<DataType>& column, const DataType& value);
		friend bool operator != <> (const DataType& value, const Column<DataType>& column);
		friend bool operator > (const Column<DataType>& column, const DataType& value)
		{
			return column.m_Value > value;
		}
		friend bool operator > (const DataType& value, const Column<DataType>& column)
		{
			return column.m_Value > value;
		}
		friend bool operator >= (const Column<DataType>& column, const DataType& value)
		{
			return column.m_Value >= value;
		}
		friend bool operator >= (const DataType& value, const Column<DataType>& column)
		{
			return column.m_Value >= value;
		}
		friend bool operator < (const Column<DataType>& column, const DataType& value)
		{
			return column.m_Value < value;
		}
		friend bool operator < (const DataType& value, const Column<DataType>& column)
		{
			return column.m_Value < value;
		}
		friend bool operator <= (const Column<DataType>& column, const DataType& value)
		{
			return column.m_Value <= value;
		}
		friend bool operator <= (const DataType& value, const Column<DataType>& column)
		{
			return column.m_Value <= value;
		}

	public:
		explicit Column()
			: m_Value()
		{

		}

		explicit Column(const DataType& value)
			: m_Value(value)
		{

		}

		const DataType& Value() const 
		{
			return m_Value;
		}

	private:
		DataType m_Value;
	};

	template <typename DataType>
	bool operator == (const Column<DataType>& column, const DataType& value)  
	{
		return column.m_Value == value;
	}

	template <typename DataType>
	bool operator == (const DataType& value, const Column<DataType>& column) 
	{
		return column.m_Value == value;
	}

	template <typename DataType>
	bool operator != (const Column<DataType>& column, const DataType& value)  
	{
		return column.m_Value != value;
	}

	template <typename DataType>
	bool operator != (const DataType& value, const Column<DataType>& column) 
	{
		return column.m_Value != value;
	}



	class Row
	{
	public:

	};

	class AccountRow : public Row
	{
	public:
		// DatabaseColumn<RawADO::adInteger> Index;
		Column<INT32> Index;
	};

	class DatabaseTable
	{
	public:
		typedef std::tr1::shared_ptr<Row> RowPtr;
		typedef xlist<RowPtr>::Type Rows;

		explicit DatabaseTable(const _txstring& tableName)
			: m_TableName(tableName)
		{

		}

		inline
			const _txstring& TableName() const 
		{
			return m_TableName;
		}

		// RowPtr FindRow

	protected:
		virtual void Load() = NULL;
		virtual void Commit() = NULL;
		virtual void Rollback() = NULL;

	private:
		_txstring m_TableName;
	};

	class Account : public DatabaseTable
	{
		//TABLE(Account);
	public:

		explicit Account(const _txstring& tableName)
			: DatabaseTable(tableName)
		{

		}

	protected:
		virtual void Load()
		{

		}

		virtual void Commit()
		{

		}

		virtual void Rollback()
		{

		}

	};

}
}
}