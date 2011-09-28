// MD5.CC - source code for the C++/object oriented translation and 
//          modification of MD5.

// Translation and modification (c) 1995 by Mordechai T. Abzug 

// This translation/ modification is provided "as is," without express or 
// implied warranty of any kind.

// The translator/ modifier does not claim (1) that MD5 will do what you think 
// it does; (2) that this translation/ modification is accurate; or (3) that 
// this software is "merchantible."  (Language for this disclaimer partially 
// copied from the disclaimer below).

/* based on:

   MD5.H - header file for MD5C.C
   MDDRIVER.C - test driver for MD2, MD4 and MD5

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/
#pragma once
#ifndef __MD5_H__
#define __MD5_H__

#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;

#include <openssl/ssl.h>
#include <openssl/md5.h>


namespace Earlgrey
{

	class MD5 
	{
	public:	
		explicit MD5();  // simple initializer
		~MD5();

		void update     (const BYTE *input, size_t input_length);
		void finalize   ();

		// methods to acquire finalized result
		BYTE * raw_digest ();  // digest as a 16-byte binary array
		char * hex_digest ();  // digest as a 33-byte ascii-hex string

	private:
		EVP_MD_CTX mdctx;

		unsigned int md_length;

		BYTE buffer[EVP_MAX_MD_SIZE];   // input buffer
		BOOL finalized;

	};
}

#endif // __MD5_H__