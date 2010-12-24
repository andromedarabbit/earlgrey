#include "StdAfx.h"
#include "OutputStream.h"
#include "AsyncStream.h"

namespace Earlgrey {

	OutputStream::OutputStream( std::tr1::shared_ptr<AsyncStream> stream ) : _Stream(stream)
	{
		EARLGREY_ASSERT( stream.get() );
	}

	void OutputStream::Flush()
	{
		_Stream->Write();
	}

}