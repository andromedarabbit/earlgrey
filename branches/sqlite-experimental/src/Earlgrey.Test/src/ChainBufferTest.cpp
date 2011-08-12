#include "stdafx.h"
#include "ChainBuffer.hpp"
#include "StlCustomAllocator.hpp"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ChainBufferTest, SetAndGet)
		{
			chain_buffer<TCHAR> buf(5);

			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());

			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );
			buf.append(tempStr + 0, _countof(tempStr));

			// operator[]
			const TCHAR& thirdChar = buf[2];
			TCHAR& secondChar = buf[1];
			ASSERT_EQ(_T('C'), thirdChar);
			ASSERT_EQ(_T('B'), secondChar);
			
			//// front
			const TCHAR& frontChar = buf.front();
			ASSERT_EQ(_T('A'), frontChar);

			//// back
			const TCHAR& backChar = buf.back();
			ASSERT_EQ(_T('\0'), backChar);

			ASSERT_EQ(_countof(tempStr), buf.size());
			ASSERT_FALSE(buf.empty());

			//// iterator
			chain_buffer<TCHAR>::iterator it = buf.begin();
			ASSERT_EQ(_T('A'), *it);

			it++;
			ASSERT_EQ(_T('B'), *it);

			it++;
			ASSERT_EQ(_T('C'), *it);

			it++;
			ASSERT_EQ(_T('D'), *it);

			it++;
			ASSERT_EQ(_T('\0'), *it);

			it++;
			ASSERT_TRUE(it == buf.end());

			//// replace
			buf.set( 1, _T("EF"), 2 ); // we want it to be "AEFD"
			chain_buffer<TCHAR>::iterator it2 = buf.begin();

			ASSERT_EQ(_T('A'), *it2);

			it2++;
			ASSERT_EQ(_T('E'), *it2);

			it2++;
			ASSERT_EQ(_T('F'), *it2);

			it2++;
			ASSERT_EQ(_T('D'), *it2);

			it2++;
			ASSERT_EQ(_T('\0'), *it2);

			it2++;
			ASSERT_TRUE(it2 == buf.end());

		}

		TEST(ChainBufferTest, GetDescription)
		{
			chain_buffer<BYTE> buf(1024);
			buf.append( (const BYTE*)"hello", 5 );

			chain_buffer<BYTE>::desc_vector_type descs;
			buf.get_descriptions( 1, descs ); 

			EXPECT_EQ( 1, descs.size() );
			EXPECT_EQ( 4, std::tr1::get<1>( descs[0] ) );
		}

		TEST(ChainBufferTest, AutoResize)
		{
			const int COUNT = 3;

			chain_buffer<TCHAR> buf(5);

			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());

			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );

			for(int i = 0; i < COUNT; i++)
			{
				buf.append(tempStr, _countof(tempStr));
			}

			//// front
			const TCHAR& frontChar = buf.front();
			ASSERT_EQ(_T('A'), frontChar);

			//// back
			const TCHAR& backChar = buf.back();
			ASSERT_EQ(_T('\0'), backChar);

			ASSERT_EQ(_countof(tempStr) * COUNT, buf.size());
			ASSERT_FALSE(buf.empty());

			//// iterator
			chain_buffer<TCHAR>::iterator it = buf.begin();
			for(int i = 0; i < COUNT; i++)
			{
				ASSERT_EQ(_T('A'), *it);

				it++;
				ASSERT_EQ(_T('B'), *it);

				it++;
				ASSERT_EQ(_T('C'), *it);

				it++;
				ASSERT_EQ(_T('D'), *it);

				it++;
				ASSERT_EQ(_T('\0'), *it);

				it++;
			}
			
			ASSERT_TRUE(it == buf.end());

			TCHAR buf_get[5 * COUNT];

			buf.get( 0, buf_get, 5 * COUNT );
			
			for(int i = 0; i < COUNT; i++)
			{
				ASSERT_EQ(_T('A'), buf_get[i * 5]);
				ASSERT_EQ(_T('B'), buf_get[i * 5 + 1]);
				ASSERT_EQ(_T('C'), buf_get[i * 5 + 2]);
				ASSERT_EQ(_T('D'), buf_get[i * 5 + 3]);
				ASSERT_EQ(_T('\0'), buf_get[i * 5 + 4]);
			}

			chain_buffer<TCHAR>::desc_vector_type desc_vec = buf.expand( 20 );
			chain_buffer<TCHAR>::desc_vector_type::const_iterator iter = desc_vec.begin();
			size_t expanded_total = 0;
			for (; iter != desc_vec.end(); iter++)
			{
				expanded_total += std::tr1::get<1>( *iter );
			}

			ASSERT_EQ( 20, expanded_total );

			size_t size = buf.size();
			buf.increase_size( 20 );
			ASSERT_EQ( 20, buf.size() - size );

			buf.expand( 1024 );
			buf.increase_size( 50 );

			buf.expand( 1024 );
			buf.increase_size( 50 );

			buf.expand( 1024 );
		}

		/*TEST(ChainBufferTest, BasicBufferUseWithStlDefaultAllocator)
		{
			basic_buffer< BYTE, StlDefaultAllocator<BYTE>::Type >* bbuf = new basic_buffer<BYTE>(1);
			BYTE tempStr = 'a';
			bbuf->set(&tempStr , 1);

			chain_buffer< basic_buffer<BYTE>, StlDefaultAllocator<basic_buffer<BYTE> >::Type > buf(1024);
			buf.set(bbuf, bbuf->size());
		}

		TEST(ChainBufferTest, BasicBufferUseWithStdAllocator)
		{
			basic_buffer< BYTE, std::allocator<BYTE> >* bbuf = new basic_buffer< BYTE, std::allocator<BYTE> >(1);
			BYTE tempStr = 'a';
			bbuf->set(&tempStr , 1);

			chain_buffer< basic_buffer< BYTE, std::allocator<BYTE> >, std::allocator<basic_buffer< BYTE, std::allocator<BYTE>> > > buf(1024);
			buf.set(bbuf, bbuf->size());
		}*/


	}
}

