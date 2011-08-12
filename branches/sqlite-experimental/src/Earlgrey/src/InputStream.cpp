#include "stdafx.h"
#include "InputStream.h"

namespace Earlgrey {


	InputStream::InputStream( std::tr1::shared_ptr<AsyncStream> stream ) : _Stream( stream )
	{

	}

}