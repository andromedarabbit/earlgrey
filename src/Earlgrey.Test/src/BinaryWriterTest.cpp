#include "stdafx.h"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"
#include "BasicBuffer.hpp"
#include "ChainBuffer.hpp"

namespace Earlgrey
{
	namespace Test
	{
		TEST(BinaryWriterTest, WriteBoolean)
		{
			typedef basic_buffer<BYTE> BUFFER_T;

			BOOL largeTrue = TRUE;
			BOOL largeFalse = FALSE;
			bool smallTrue = true;
			bool smallFalse = false;


			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);
			ASSERT_TRUE2(writer.Write(largeTrue));
			ASSERT_TRUE2(writer.Write(largeFalse));
			ASSERT_TRUE2(writer.Write(smallTrue));
			ASSERT_TRUE2(writer.Write(smallFalse));


			BinaryReader<BUFFER_T> reader(buf);

			BOOL largeRetValue = FALSE;
			ASSERT_TRUE2(reader.Read(largeRetValue));
			ASSERT_EQ(largeTrue, largeRetValue);

			ASSERT_TRUE2(reader.Read(largeRetValue));
			ASSERT_EQ(largeFalse, largeRetValue);

			bool smallRetValue = false;
			ASSERT_TRUE2(reader.Read(smallRetValue));
			ASSERT_EQ(smallTrue, smallRetValue);

			ASSERT_TRUE2(reader.Read(smallRetValue));
			ASSERT_EQ(smallFalse, smallRetValue);
			ASSERT_FALSE2(reader.Read(largeTrue));
		}


		TEST(BinaryWriterTest, WriteInt32)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			int intMaxValue = INT_MAX;
			ASSERT_TRUE2(writer.Write(intMaxValue));	

			int intMinValue = INT_MIN;
			ASSERT_TRUE2(writer.Write(intMinValue));


			BinaryReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}

		TEST(BinaryWriterTest, WriteInt32UsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			int intMaxValue = INT_MAX;
			ASSERT_TRUE2(writer.Write(intMaxValue));

			int intMinValue = INT_MIN;
			ASSERT_TRUE2(writer.Write(intMinValue));


			BinaryReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}

		TEST(BinaryWriterTest, WriteFloatUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			float maxValue = std::numeric_limits<float>::max() / 2;
			ASSERT_TRUE2(writer.Write(maxValue));

			float minValue = std::numeric_limits<float>::min() / 3;
			ASSERT_TRUE2(writer.Write(minValue));


			BinaryReader<BUFFER_T> reader(buf);
			FLOAT retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}

		TEST(BinaryWriterTest, WriteDoubleUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			double maxValue = std::numeric_limits<double>::max() / 4;
			ASSERT_TRUE2(writer.Write(maxValue));

			double minValue = std::numeric_limits<double>::min() / 5;
			ASSERT_TRUE2(writer.Write(minValue));


			BinaryReader<BUFFER_T> reader(buf);
			double retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}

// 		TEST(BinaryWriterTest, WriteInt32Pointer)
// 		{	
// 			typedef chain_buffer<BYTE> BUFFER_T;
// 
// 			BUFFER_T buf(128);
// 			BinaryWriter<BUFFER_T> writer(buf);
// 
// 			std::auto_ptr<int> intMaxValue( new int(INT_MAX) );
// 			ASSERT_TRUE2(writer.Write(intMaxValue.get()));
// 
// 			std::auto_ptr<int> intMinValue( new int(INT_MIN) );
// 			ASSERT_TRUE2(writer.Write(intMinValue.get()));
// 
// 			BinaryReader<BUFFER_T> reader(buf);
// 			INT32 * retValue = NULL;
// 			ASSERT_TRUE2(reader.Read(retValue));
// 			ASSERT_EQ(*intMaxValue, *retValue);
// 
// 			ASSERT_TRUE2(reader.Read(retValue));
// 			ASSERT_EQ(*intMinValue, *retValue);
// 
// 			ASSERT_FALSE2(reader.Read(retValue));
// 		}

		TEST(BinaryWriterTest, WriteString)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			ASSERT_TRUE2(writer.Write(tempStr, _countof(tempStr)));


			BinaryReader<BUFFER_T> reader(buf);
			TCHAR retValue[128];
			ASSERT_TRUE2(reader.Read(retValue, _countof(retValue), _countof(tempStr)));
			ASSERT_TRUE( _txstring(retValue) == ABCD );
		}

		TEST(BinaryWriterTest, WriteEmptyString)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			
			TCHAR emptyStr[5] = { NULL, };

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			ASSERT_TRUE2(writer.Write(emptyStr, _countof(emptyStr)));


			BinaryReader<BUFFER_T> reader(buf);
			TCHAR retValue[128];
			ASSERT_TRUE2(reader.Read(retValue, _countof(retValue), _countof(emptyStr)));
			ASSERT_TRUE( _txstring(retValue) == emptyStr );
		}

		TEST(BinaryWriterTest, WriteBytes)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			BYTE ABCD[5];
			ABCD[0] = static_cast<BYTE>(1);
			ABCD[1] = static_cast<BYTE>(3);
			ABCD[2] = static_cast<BYTE>(5);
			ABCD[3] = static_cast<BYTE>(7);
			ABCD[4] = static_cast<BYTE>(9);

			ASSERT_TRUE2(writer.WriteBytes(ABCD, _countof(ABCD)));


			BinaryReader<BUFFER_T> reader(buf);
			BYTE retValue[128];
			ASSERT_TRUE2(reader.ReadBytes(retValue, _countof(retValue), _countof(ABCD)));

			for(int i=0; i<_countof(ABCD); i++)
			{
				ASSERT_EQ(ABCD[i], retValue[i]);
			}
		}

		TEST(BinaryWriterTest, WriteIntegers)
		{
			typedef basic_buffer<int> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			int ABCD[5];
			ABCD[0] = 1;
			ABCD[1] = 3;
			ABCD[2] = 5;
			ABCD[3] = 7;
			ABCD[4] = 9;

			ASSERT_TRUE2(writer.Write(ABCD, _countof(ABCD)));
		}

		TEST(BinaryWriterTest, BufferIsFull)
		{
			typedef basic_buffer<int> BUFFER_T;

			BUFFER_T buf(4);
			BinaryWriter<BUFFER_T> writer(buf);

			int ABCD[5];
			ABCD[0] = 1;
			ABCD[1] = 3;
			ABCD[2] = 5;
			ABCD[3] = 7;
			ABCD[4] = 9;

			ASSERT_FALSE2(writer.Write(ABCD, _countof(ABCD)));
			ASSERT_FALSE(writer.good());
			ASSERT_FALSE(writer.eof());
			ASSERT_TRUE(writer.bad());
			ASSERT_TRUE(writer.fail());
		}
	}
}
