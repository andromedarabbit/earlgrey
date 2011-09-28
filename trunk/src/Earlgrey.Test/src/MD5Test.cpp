#include "stdafx.h"
#include "MD5.h"

#include <utility>

#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")

namespace Earlgrey
{
	namespace Test
	{
		class MD5Test : public ::testing::Test 
		{
		public:
			typedef std::pair<std::string, std::string> ValueHashPair;
			typedef std::list<ValueHashPair> ValueHashContainer;

		protected:
			// Some expensive resource shared by all tests.
			ValueHashContainer m_Container;

			// Per-test-case set-up.
			// Called before the first test in this test case.
			// Can be omitted if not needed.
			static void SetUpTestCase() {
			}

			// Per-test-case tear-down.
			// Called after the last test in this test case.
			// Can be omitted if not needed.
			static void TearDownTestCase() {
			}

			// You can define per-test set-up and tear-down logic as usual.
			virtual void SetUp();			
			virtual void TearDown() { }

		private:
			void AddNew(const ValueHashPair& item);
			void AddNew(const std::string& key, const std::string& hash);
		};

		void MD5Test::AddNew(const ValueHashPair& item)
		{
			m_Container.push_back(item);
		};

		void MD5Test::AddNew(const std::string& key, const std::string& hash)
		{
			ValueHashPair item(key, hash);
			m_Container.push_back(item);
		};

		void MD5Test::SetUp() 
		{ 
			AddNew("Hello World!", "ed076287532e86365e841e92bfc50d8c");
			AddNew("What if we were wrong?", "fde042e831d08628e3f9de8c6ed87b48");
		}


		TEST_F(MD5Test, Simple)
		{	
			for(ValueHashContainer::const_iterator it = m_Container.begin(); it != m_Container.end(); it++)
			{
				std::string sourceValue = (*it).first;

				MD5 md5pass2;
				md5pass2.update(
					reinterpret_cast<const BYTE*>(sourceValue.c_str())
					, static_cast<unsigned int>( sourceValue.length() )
					);
				md5pass2.finalize();

				char * hexDigest = md5pass2.hex_digest();			
				ASSERT_STREQ((*it).second.c_str(), hexDigest);

				delete hexDigest;
			}

		}

		//namespace
		//{
		//	void MessageDigest( const void *msg , size_t len ,void *msgout,int *outsize)
		//	{
		//		EVP_MD_CTX mdctx;
		//		unsigned char md_result[EVP_MAX_MD_SIZE];
		//		unsigned int md_length;

		//		EVP_DigestInit(&mdctx, EVP_md5());
		//		EVP_DigestUpdate(&mdctx, msg, len);
		//		EVP_DigestFinal_ex(&mdctx, md_result, &md_length);
		//		EVP_MD_CTX_cleanup(&mdctx);
		//		memcpy( msgout, md_result, md_length);
		//		*outsize= md_length;

		//	}
		//}

		//TEST_F(MD5Test, Temp)
		//{
		//	for(ValueHashContainer::const_iterator it = m_Container.begin(); it != m_Container.end(); it++)
		//	{
		//		std::string sourceValue = (*it).first;

		//		char output[EVP_MAX_MD_SIZE * 1024];
		//		int len;

		//		MessageDigest((const void *)sourceValue.c_str(), sourceValue.length(), output, &len);
		//		output[len] = NULL;


		//		for(int i = 0; i < len; i++)
		//		{

		//			printf ("%02x", output[i]&255);
		//		}
		//		printf("\r\n");

		//		// delete hexDigest;
		//	}
		//}

	}
}