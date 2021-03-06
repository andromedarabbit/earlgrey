#include "stdafx.h"
#include "NetworkReader.hpp"
#include "SimpleBuffer.hpp"
#include "BasicBuffer.hpp"
#include "ChainBuffer.hpp"
#include "txstring.h"

namespace Earlgrey
{
	namespace Test
	{

		TEST(NetworkReaderTest, ReadBoolean)
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


			NetworkReader<BUFFER_T> reader(buf);

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

		TEST(NetworkReaderTest, ReadInt32)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			int intMaxValue = INT_MAX;
			buf.set((BYTE*)&intMaxValue, sizeof(intMaxValue));

			int intMinValue = INT_MIN;
			buf.append((BYTE*)&intMinValue, sizeof(intMinValue));

			NetworkReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}

		TEST(NetworkReaderTest, ReadInt32UsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			int intMaxValue = INT_MAX;
			buf.append((BYTE*)&intMaxValue, sizeof(intMaxValue));

			int intMinValue = INT_MIN;
			buf.append((BYTE*)&intMinValue, sizeof(intMinValue));

			NetworkReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}

		TEST(NetworkReaderTest, ReadFloatUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			float maxValue = std::numeric_limits<float>::max() / 2;
			buf.append((BYTE*)&maxValue, sizeof(maxValue));

			float minValue = std::numeric_limits<float>::min() / 3;
			buf.append((BYTE*)&minValue, sizeof(minValue));

			NetworkReader<BUFFER_T> reader(buf);
			FLOAT retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}

		TEST(NetworkReaderTest, ReadDoubleUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			double maxValue = std::numeric_limits<double>::max() / 4;
			buf.append((BYTE*)&maxValue, sizeof(maxValue));

			double minValue = std::numeric_limits<double>::min() / 5;
			buf.append((BYTE*)&minValue, sizeof(minValue));

			NetworkReader<BUFFER_T> reader(buf);
			double retValue = 0;
			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE2(reader.Read(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE2(reader.Read(retValue));
		}


		TEST(NetworkReaderTest, ReadString)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			const UINT16 len = 5;

			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[len];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );

			BUFFER_T buf(128);
			buf.set((BYTE*)&len, sizeof(UINT16));
			buf.append((BYTE*)tempStr + 0, sizeof(tempStr));

			NetworkReader<BUFFER_T> reader(buf);
			TCHAR retValue[128];
			ASSERT_TRUE2(reader.ReadString(retValue, _countof(retValue), _countof(tempStr)));
			ASSERT_TRUE( _txstring(retValue) == ABCD );
		}

		TEST(NetworkReaderTest, ReadBytes)
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

			NetworkReader<BUFFER_T> reader(buf);
			BYTE retValue[128];
			ASSERT_TRUE2(reader.ReadBytes(retValue, _countof(retValue), _countof(ABCD)));

			for(int i=0; i<_countof(ABCD); i++)
			{
				ASSERT_EQ(ABCD[i], retValue[i]);
			}
		}
	}
}