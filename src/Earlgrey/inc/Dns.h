#pragma once 
#include "Uncopyable.h"
#include "txstring.h"
#include "xlist.h"

namespace Earlgrey
{
	class IPHostEntry;
	class IPAddress;
	class IPAddress2;

	class Dns : private Uncopyable
	{
	private:
		explicit Dns();

	public:
		typedef std::tr1::shared_ptr<IPHostEntry> IPHostEntryPtr;

		// ±â¿Ï¾¾°¡ Â§ ¿¾³¯ ÄÚµå
		static DWORD GetHostByName(LPCSTR Name, IPAddress& Addr);


		typedef std::tr1::shared_ptr<IPAddress2> IPAddressPtr;
		typedef xlist<IPAddressPtr>::Type IPAddresses;

		static _txstring GetHostName();

		static void GetHostAddresses(const _txstring& hostNameOrAddress, IPAddresses& addresses);

 		static IPHostEntryPtr GetHostEntry(const IPAddress2& address);
 		static IPHostEntryPtr GetHostEntry(const _txstring& hostNameOrAddress);
	};

	
}