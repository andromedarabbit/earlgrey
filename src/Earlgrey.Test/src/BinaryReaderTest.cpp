#include "stdafx.h"
#include "BinaryReader.hpp"
#include "BasicBuffer.hpp"
#include "ChainBuffer.hpp"
#include "txstring.h"

namespace Earlgrey
{
	namespace Test
	{

		TEST(BinaryReaderTest, ReadBoolean)
		{
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);

			BOOL largeTrue = TRUE;
			buf.append((BYTE*)&largeTrue, sizeof(largeTrue));
			BOOL largeFalse = FALSE;
			buf.append((BYTE*)&largeFalse, sizeof(largeFalse));

			bool smallTrue = true;
			buf.append((BYTE*)&smallTrue, sizeof(smallTrue));
			bool smallFalse = false;
			buf.append((BYTE*)&smallFalse, sizeof(smallFalse));


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

		TEST(BinaryReaderTest, ReadInt32)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			int intMaxValue = INT_MAX;
			buf.set((BYTE*)&intMaxValue, sizeof(intMaxValue));

			int intMinValue = INT_MIN;
			buf.append((BYTE*)&intMinValue, sizeof(intMinValue));

			BinaryReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE(reader.ReadInt32(retValue));
		}

		TEST(BinaryReaderTest, ReadInt32UsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			int intMaxValue = INT_MAX;
			buf.set((BYTE*)&intMaxValue, sizeof(intMaxValue));

			int intMinValue = INT_MIN;
			buf.append((BYTE*)&intMinValue, sizeof(intMinValue));

			BinaryReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE(reader.ReadInt32(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE(reader.ReadInt32(retValue));
		}

		TEST(BinaryReaderTest, ReadFloatUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			float maxValue = std::numeric_limits<float>::max() / 2;
			buf.set((BYTE*)&maxValue, sizeof(maxValue));

			float minValue = std::numeric_limits<float>::min() / 3;
			buf.append((BYTE*)&minValue, sizeof(minValue));

			BinaryReader<BUFFER_T> reader(buf);
			FLOAT retValue = 0;
			ASSERT_TRUE(reader.ReadFloat(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE(reader.ReadFloat(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE(reader.ReadFloat(retValue));
		}

		TEST(BinaryReaderTest, ReadDoubleUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			double maxValue = std::numeric_limits<double>::max() / 4;
			buf.set((BYTE*)&maxValue, sizeof(maxValue));

			double minValue = std::numeric_limits<double>::min() / 5;
			buf.append((BYTE*)&minValue, sizeof(minValue));

			BinaryReader<BUFFER_T> reader(buf);
			double retValue = 0;
			ASSERT_TRUE(reader.ReadDouble(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE(reader.ReadDouble(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE(reader.ReadDouble(retValue));
		}


		TEST(BinaryReaderTest, ReadString)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;
			
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );

			BUFFER_T buf(128);
			buf.set((BYTE*)tempStr + 0, sizeof(tempStr));

			BinaryReader<BUFFER_T> reader(buf);
			TCHAR retValue[128];
			ASSERT_TRUE(reader.ReadString(retValue, _countof(tempStr)));
			ASSERT_TRUE( _txstring(retValue) == ABCD );
		}

		TEST(BinaryReaderTest, ReadBytes)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);

			BYTE ABCD[5];
			ABCD[0] = static_cast<BYTE>(1);
			ABCD[1] = static_cast<BYTE>(3);
			ABCD[2] = static_cast<BYTE>(5);
			ABCD[3] = static_cast<BYTE>(7);
			ABCD[4] = static_cast<BYTE>(9);

			
			buf.set(ABCD + 0, sizeof(ABCD));

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