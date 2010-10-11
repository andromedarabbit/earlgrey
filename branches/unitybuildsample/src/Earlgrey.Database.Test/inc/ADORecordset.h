#pragma once
#include "Uncopyable.h"
#include "ADO.h"

namespace Earlgrey {
namespace ADO {

	class Recordset : private Uncopyable
	{
	public:
		explicit Recordset()
			: m_Recordset(NULL)
		{

		}

	private:
		RawADO::_RecordsetPtr m_Recordset;
	};

}
}