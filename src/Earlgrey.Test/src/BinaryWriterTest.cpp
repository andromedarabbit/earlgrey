#include "stdafx.h"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"
#include "BasicBuffer.hpp"
#include "ChainBuffer.hpp"
// #include "txstring.h"

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
			ASSERT_TRUE(writer.WriteBoolean(largeTrue));
			ASSERT_TRUE(writer.WriteBoolean(largeFalse));
			ASSERT_TRUE(writer.WriteBoolean(smallTrue));
			ASSERT_TRUE(writer.WriteBoolean(smallFalse));


			BinaryReader<BUFFER_T> reader(buf);

			BOOL largeRetValue = FALSE;
			ASSERT_TRUE(reader.ReadBoolean(largeRetValue));
			ASSERT_EQ(largeTrue, largeRetValue);

			ASSERT_TRUE(reader.ReadBoolean(largeRetValue));
			ASSERT_EQ(largeFalse, largeRetValue);

			bool smallRetValue = false;
			ASSERT_TRUE(reader.ReadBoolean(smallRetValue));
			ASSERT_EQ(smallTrue, smallRetValue);

			ASSERT_TRUE(reader.ReadBoolean(smallRetValue));
			ASSERT_EQ(smallFalse, smallRetValue);
			ASSERT_FALSE(reader.ReadBoolean(largeTrue));
		}


		TEST(BinaryWriterTest, WriteInt32)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			int intMaxValue = INT_MAX;
			ASSERT_TRUE(writer.WriteInt32(intMaxValue));	

			int intMinValue = INT_MIN;
			ASSERT_TRUE(writer.WriteInt32(intMinValue));


			BinaryReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE(reader.ReadInt32(retValue));
		}

		TEST(BinaryWriterTest, WriteInt32UsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			int intMaxValue = INT_MAX;
			ASSERT_TRUE(writer.WriteInt32(intMaxValue));
			
			int intMinValue = INT_MIN;
			ASSERT_TRUE(writer.WriteInt32(intMinValue));


			BinaryReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE(reader.ReadInt32(retValue));
		}

		TEST(BinaryWriterTest, WriteFloatUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			float maxValue = std::numeric_limits<float>::max() / 2;
			ASSERT_TRUE(writer.WriteFloat(maxValue));

			float minValue = std::numeric_limits<float>::min() / 3;
			ASSERT_TRUE(writer.WriteFloat(minValue));


			BinaryReader<BUFFER_T> reader(buf);
			FLOAT retValue = 0;
			ASSERT_TRUE(reader.ReadFloat(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE(reader.ReadFloat(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE(reader.ReadFloat(retValue));
		}

		TEST(BinaryWriterTest, WriteDoubleUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			double maxValue = std::numeric_limits<double>::max() / 4;
			ASSERT_TRUE(writer.WriteDouble(maxValue));

			double minValue = std::numeric_limits<double>::min() / 5;
			ASSERT_TRUE(writer.WriteDouble(minValue));


			BinaryReader<BUFFER_T> reader(buf);
			double retValue = 0;
			ASSERT_TRUE(reader.ReadDouble(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE(reader.ReadDouble(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE(reader.ReadDouble(retValue));
		}


		TEST(BinaryWriterTest, WriteString)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );

			BUFFER_T buf(128);
			BinaryWriter<BUFFER_T> writer(buf);

			ASSERT_TRUE(writer.WriteString(tempStr, _countof(tempStr)));
		

			BinaryReader<BUFFER_T> reader(buf);
			TCHAR retValue[128];
			ASSERT_TRUE(reader.ReadString(retValue, _countof(tempStr)));
			ASSERT_TRUE( _txstring(retValue) == ABCD );
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

			ASSERT_TRUE(writer.WriteBytes(ABCD, _countof(ABCD)));


			BinaryReader<BUFFER_T> reader(buf);
			BYTE retValue[128];
			ASSERT_TRUE(reader.ReadBytes(retValue, _countof(ABCD)));

			for(int i=0; i<_countof(ABCD); i++)
			{
				ASSERT_EQ(ABCD[i], retValue[i]);
			}
		}
	}
}
