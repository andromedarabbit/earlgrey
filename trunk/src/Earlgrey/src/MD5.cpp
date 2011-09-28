#include "stdafx.h"
#include "md5.h"

#include <assert.h>
#include <string.h>
#include <iostream>

using namespace std;

//#include <openssl/ssl.h>
//#include <openssl/md5.h>

#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")


namespace Earlgrey
{
	
	MD5::MD5()
		: finalized(FALSE)
		, mdctx()
	{
		EVP_DigestInit(&mdctx, EVP_md5());
	}

	MD5::~MD5()
	{
		EVP_MD_CTX_cleanup(&mdctx);		
	}


	void MD5::update (const BYTE *input, size_t input_length)
	{
		EVP_DigestUpdate(&mdctx, input, input_length);
	}

	void MD5::finalize ()
	{
		EVP_DigestFinal_ex(&mdctx, buffer, &md_length);
		finalized = TRUE;
	}


	BYTE * MD5::raw_digest()
	{
		BYTE * s = new BYTE[EVP_MAX_MD_SIZE];

		if (!finalized){
			cerr << "MD5::raw_digest:  Can't get digest if you haven't "<<
			  "finalized the digest!" << endl;
			return ( (unsigned char*) "");
		}

		memcpy_s(s, EVP_MAX_MD_SIZE, buffer, EVP_MAX_MD_SIZE);
		return s;
	}



	char * MD5::hex_digest()
	{

	  int i;
	  char *s= new char[33];

	  if (!finalized){
		cerr << "MD5::hex_digest:  Can't get digest if you haven't "<<
		  "finalized the digest!" <<endl;
		return "";
	  }

	  for (i = 0; i < EVP_MAX_IV_LENGTH; i++)
	  {
		  // TODO: 하드 코딩 제거
		  sprintf_s(s + i * 2, 33 - i * 2, "%02x", buffer[i] & 255);
	  }		

	  s[32]='\0';

	  return s;
	}

}

