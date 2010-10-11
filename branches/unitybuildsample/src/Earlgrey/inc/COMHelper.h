#pragma once

namespace Earlgrey
{

	class COMHelper : private Uncopyable
	{
	private:
		explicit COMHelper();

		static __declspec(thread) BOOL s_Initialized; 

	public:
		static BOOL Initialize();
		static BOOL Uninitialize();

		static BOOL Initialized();
	};
}