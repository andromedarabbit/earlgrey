#include "stdafx.h"
#include "ADODataReader.h"

namespace Earlgrey {
namespace ADO {


	BOOL DataReader::Read()
	{
		EARLGREY_ASSERT(m_Recordset != NULL);

		if(m_HasReadFirstRecord == TRUE) 
		{
			m_Index = static_cast<SHORT>(0);
			m_Recordset->MoveNext();
		}
		m_HasReadFirstRecord = TRUE;
		return m_Recordset->EndOfFile == false && m_Recordset->BOF == false;
	}

	

}
}
