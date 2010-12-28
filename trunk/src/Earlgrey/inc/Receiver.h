#pragma once 

#include "CompletionHandler.h"

namespace Earlgrey
{
	class AsyncStream;
	class AsyncResult;

	class Receiver : public CompletionHandler
	{
	public:
		explicit Receiver(AsyncStream* Stream);

		virtual ~Receiver();

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result);

		virtual void OnDisconnected();

		//! ��Ŷ �ڵ鷯; ��Ŷ �Ľ� ���� �����ؾ� �Ѵ�. 
		/*!
			��Ŷ�� ó���ߴٸ� _Start ����� ������ ��ġ�� �̵����Ѿ� �Ѵ�.
		*/
		virtual bool HandlePacket(NetworkBuffer* buffer);
	protected:
		AsyncStream* _Stream;
		size_t _Start;	//!< ���ۿ��� ó���� �������� ���� ��ġ
		size_t _End;	//!< ���ۿ� ��ϵ� �������� ���� ����Ų��.
	};
}