#pragma once 
#include <winsock2.h>

namespace Earlgrey
{
	class SocketSubsystem
	{
	public:

		// \note �� �Լ��� �ٸ� Ŭ������ �ִ� ���� ������?
		// \note �ʱ�ȭ�� ������ ���ᵵ �־�� �ϳ�? - WSACleanup
		static BOOL InitializeSubSystem();
		INT GetLastErrorCode()
		{
			return WSAGetLastError();
		}
		const TCHAR* GetSocketError(INT Code = -1);
	
		/*INT GetHostByName(ANSICHAR* HostName, FInternetIpAddr& Addr);
		BOOL GetHostName(String& HostName);
		BOOL GetLocalHostAddr(FInternetIpAddr& HostAddr);*/
	};
}