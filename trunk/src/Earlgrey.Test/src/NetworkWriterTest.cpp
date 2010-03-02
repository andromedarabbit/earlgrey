#include "stdafx.h"
#include "NetworkWriter.hpp"
#include "NetworkReader.hpp"
#include "BasicBuffer.hpp"
#include "ChainBuffer.hpp"

#include "xvector.h"
#include "xmap.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(NetworkWriterTest, WriteBoolean)
		{
			typedef basic_buffer<BYTE> BUFFER_T;

			BOOL largeTrue = TRUE;
			BOOL largeFalse = FALSE;
			bool smallTrue = true;
			bool smallFalse = false;


			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);
			ASSERT_TRUE(writer.Write(largeTrue));
			ASSERT_TRUE(writer.Write(largeFalse));
			ASSERT_TRUE(writer.Write(smallTrue));
			ASSERT_TRUE(writer.Write(smallFalse));


			NetworkReader<BUFFER_T> reader(buf);

			BOOL largeRetValue = FALSE;
			ASSERT_TRUE(reader.Read(largeRetValue));
			ASSERT_EQ(largeTrue, largeRetValue);

			ASSERT_TRUE(reader.Read(largeRetValue));
			ASSERT_EQ(largeFalse, largeRetValue);

			bool smallRetValue = false;
			ASSERT_TRUE(reader.Read(smallRetValue));
			ASSERT_EQ(smallTrue, smallRetValue);

			ASSERT_TRUE(reader.Read(smallRetValue));
			ASSERT_EQ(smallFalse, smallRetValue);
			ASSERT_FALSE(reader.Read(largeTrue));
		}


		TEST(NetworkWriterTest, WriteBooleanUsingOperator)
		{
			typedef basic_buffer<BYTE> BUFFER_T;

			BOOL largeTrue = TRUE;
			BOOL largeFalse = FALSE;
			bool smallTrue = true;
			bool smallFalse = false;


			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);
			writer << largeTrue << largeFalse << smallTrue << smallFalse;


			NetworkReader<BUFFER_T> reader(buf);

			BOOL largeRetValue = FALSE;
			reader >> largeRetValue;
			ASSERT_EQ(largeTrue, largeRetValue);

			reader >> largeRetValue;
			ASSERT_EQ(largeFalse, largeRetValue);

			bool smallRetValue = false;
			reader >> smallRetValue;
			ASSERT_EQ(smallTrue, smallRetValue);

			reader >> smallRetValue;
			ASSERT_EQ(smallFalse, smallRetValue);
			ASSERT_FALSE(reader.Read(largeTrue));
		}


		TEST(NetworkWriterTest, WriteInt32)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);

			int intMaxValue = INT_MAX;
			ASSERT_TRUE(writer.Write(intMaxValue));	

			int intMinValue = INT_MIN;
			ASSERT_TRUE(writer.Write(intMinValue));


			NetworkReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE(reader.Read(retValue));
		}

		TEST(NetworkWriterTest, WriteInt32UsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);

			int intMaxValue = INT_MAX;
			ASSERT_TRUE(writer.Write(intMaxValue));

			int intMinValue = INT_MIN;
			ASSERT_TRUE(writer.Write(intMinValue));


			NetworkReader<BUFFER_T> reader(buf);
			INT32 retValue = 0;
			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(intMaxValue, retValue);

			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(intMinValue, retValue);

			ASSERT_FALSE(reader.Read(retValue));
		}

		TEST(NetworkWriterTest, WriteFloatUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);

			float maxValue = std::numeric_limits<float>::max() / 2;
			ASSERT_TRUE(writer.Write(maxValue));

			float minValue = std::numeric_limits<float>::min() / 3;
			ASSERT_TRUE(writer.Write(minValue));


			NetworkReader<BUFFER_T> reader(buf);
			FLOAT retValue = 0;
			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE(reader.Read(retValue));
		}

		TEST(NetworkWriterTest, WriteDoubleUsingChainBuffer)
		{	
			typedef chain_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);

			double maxValue = std::numeric_limits<double>::max() / 4;
			ASSERT_TRUE(writer.Write(maxValue));

			double minValue = std::numeric_limits<double>::min() / 5;
			ASSERT_TRUE(writer.Write(minValue));


			NetworkReader<BUFFER_T> reader(buf);
			double retValue = 0;
			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(maxValue, retValue);

			ASSERT_TRUE(reader.Read(retValue));
			ASSERT_EQ(minValue, retValue);

			ASSERT_FALSE(reader.Read(retValue));
		}


		TEST(NetworkWriterTest, WriteString)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			const _txstring ABCD = _T("ABCD");
			
			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);
			writer << ABCD;
			

			NetworkReader<BUFFER_T> reader(buf);
			_txstring retValue;
			reader >> retValue;
			ASSERT_TRUE( _txstring(retValue) == ABCD );
		}

		TEST(NetworkWriterTest, WriteEmptyString)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			const _txstring empty;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);
			writer << empty;


			NetworkReader<BUFFER_T> reader(buf);
			_txstring retValue;
			reader >> retValue;
			ASSERT_TRUE( _txstring(retValue) == empty );
		}


		TEST(NetworkWriterTest, WriteBytes)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);

			BYTE ABCD[5];
			ABCD[0] = static_cast<BYTE>(1);
			ABCD[1] = static_cast<BYTE>(3);
			ABCD[2] = static_cast<BYTE>(5);
			ABCD[3] = static_cast<BYTE>(7);
			ABCD[4] = static_cast<BYTE>(9);

			ASSERT_TRUE(writer.WriteBytes(ABCD, _countof(ABCD)));


			NetworkReader<BUFFER_T> reader(buf);
			BYTE retValue[128];
			ASSERT_TRUE(reader.ReadBytes(retValue, _countof(retValue), _countof(ABCD)));

			for(int i=0; i<_countof(ABCD); i++)
			{
				ASSERT_EQ(ABCD[i], retValue[i]);
			}
		}


		TEST(BinaryWriterTest, WriteInt32Vector)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);

			xvector<BYTE>::Type ABCD;
			ABCD.push_back(static_cast<BYTE>(1));
			ABCD.push_back(static_cast<BYTE>(3));
			ABCD.push_back(static_cast<BYTE>(5));
			ABCD.push_back(static_cast<BYTE>(7));
			ABCD.push_back(static_cast<BYTE>(9));

			writer << ABCD;
			//ASSERT_TRUE(writer.WriteContainter(ABCD));


			NetworkReader<BUFFER_T> reader(buf);
			
			xvector<BYTE>::Type newContainer;
			reader >> newContainer;

			ASSERT_EQ(ABCD.size(), newContainer.size());
			for(size_t i=0; i < ABCD.size(); i++)
			{
				ASSERT_EQ(ABCD[i], newContainer[i]);
			}

			// xvector<BYTE>::Type newContainer;
			// reader >> newContainer;
		}

		TEST(BinaryWriterTest, WriteInt32Map)
		{	
			typedef basic_buffer<BYTE> BUFFER_T;

			BUFFER_T buf(128);
			NetworkWriter<BUFFER_T> writer(buf);

			xmap<TCHAR, int>::Type ABCD;
			ABCD[_T('a')] = 1;
			ABCD[_T('b')] = 2;
			ABCD[_T('c')] = 3;
			ABCD[_T('d')] = 4;
			
			writer << ABCD;
		}
	}
}
