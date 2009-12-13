#pragma once

#include "LockfreeQueue.hpp"
// Message Per Thread Pattern


namespace Earlgrey
{

	class IThreadMessage 
	{

	public:
		virtual ~IThreadMessage() {}


		virtual void OnReceive() = 0;

	};

	class Thread;

	class ThreadMessageQueue 
	{

	public:
		explicit ThreadMessageQueue(std::tr1::shared_ptr<Thread> thread) 
			: Thread_(thread) 
		{

		}

		void Enqueue(std::tr1::shared_ptr<IThreadMessage> message) 
		{
			MessageQueue_.Enqueue(message);
		}

		void InvokeAll() 
		{
			std::tr1::shared_ptr<IThreadMessage> message;
			
			while(MessageQueue_.Dequeue(message)) {
				message->OnReceive();
			}
		}


	private:
		std::tr1::weak_ptr<Thread> Thread_;
		Algorithm::Lockfree::Queue<std::tr1::shared_ptr<IThreadMessage>> MessageQueue_;
	};

}