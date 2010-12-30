#pragma once

namespace Earlgrey {

	class NetworkBuffer;

	class IPacketHandler
	{
	public:
		//! ��Ŷ�� ó���Ѵ�.
		/*!
			���ۿ��� �̹� ó���� �����ͱ��� ���Ե� �ִ�. ó���� �����ʹ� Start �� End ���̿� ��� �����Ƿ�,
			�� ������ ������� �ȵȴ�. ��Ŷó���� �� �Ŀ� ó���� �������� ���� ��� �Ķ������ HandledSize �� �����ؾ� �Ѵ�.
			����, ó���ϱ⿡ �������� ���� �ʹ� ���ٸ� HandleSize �� 0 �� �����ؼ� ���� ��Ŷ�� ��ٸ� �� �ִ�.

			\param Buffer ��Ŷ �����Ͱ� ����ִ�.
			\param Start ó���� �� �ִ� �����Ͱ� ����ִ� ������ ���� ��ġ
			\param End ó���� �� �ִ� �����Ͱ� ����ִ� ������ �� ��ġ
			\param HandledSize [out] ó���� �������� ũ��

			\return ó���� �����ϸ� true; false �� �����ϸ� ������ �����.
		*/
		virtual bool Handle(NetworkBuffer* Buffer, size_t Start, size_t End, size_t& HandledSize) = 0;
	};

}