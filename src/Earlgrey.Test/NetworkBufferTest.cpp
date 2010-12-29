#include "stdafx.h"
#include "shared_ptr_helper.h"
#include "NetworkBuffer.h"

using namespace Earlgrey;

TEST(NetworkBufferTest, ReadWriteTest)
{
	std::tr1::shared_ptr<NetworkBuffer> buffer = make_ptr(new NetworkBuffer());

	int i32 = 100;
	std::wstring wstr = L"으라차차!";

	buffer->SetValue( reinterpret_cast<const BYTE*>(&i32), sizeof(int) );
	EXPECT_EQ( sizeof(int), buffer->GetBufferSize() );

	buffer->SetValue( reinterpret_cast<const BYTE*>(wstr.c_str()), wstr.size() * sizeof(WCHAR) );
	EXPECT_EQ( sizeof(int) + wstr.length() * sizeof(WCHAR), buffer->GetBufferSize() );

	i32 = 0;
	buffer->GetValue( 0, reinterpret_cast<BYTE*>(&i32), sizeof(int) );
	EXPECT_EQ( 100, i32 );

	WCHAR* strbuf = new WCHAR[wstr.size() + 1];
	memset( strbuf, 0, (wstr.size() + 1) * sizeof(WCHAR) );

	buffer->GetValue( sizeof(int), reinterpret_cast<BYTE*>(strbuf), wstr.size() * sizeof(WCHAR) );
	EXPECT_STREQ( wstr.c_str(), strbuf );

	delete[] strbuf;

	buffer->Clear();
	EXPECT_EQ( 0,  buffer->GetBufferSize() );

	WSABUF* wsabuf = buffer->GetSockRecvBuffer();
	EXPECT_GE( EARLGREY_NUMERIC_CAST<size_t>(1024), buffer->GetBufferCapacity() );
	buffer->OnReceived( 100 );
	EXPECT_EQ( 100, buffer->GetBufferSize() );

	delete[] wsabuf;
}