#pragma once 

namespace Earlgrey
{
	class CompletionHandler;
	class NetworkBuffer;

	class AsyncStream
	{
	public:
		explicit AsyncStream() {};		
		virtual ~AsyncStream() {}; //! \todo Close() �� �ҷ��� �ǳ�?

		BOOL Open(SOCKET Socket, CompletionHandler* Handler); // Socket socket -> HANDLE handle ?
		void Close();

		BOOL Post();
		BOOL AsyncRead(); 
		BOOL AsyncWrite();

		NetworkBuffer* GetNetworkBuffer() { return _PacketBuffer; }
	private:
		NetworkBuffer* _PacketBuffer;
		CompletionHandler* _Handler;
		SOCKET _Handle;
	};
}