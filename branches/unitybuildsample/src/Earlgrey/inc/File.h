#pragma once
#include "Uncopyable.h"
#include "txstring.h"

namespace Earlgrey
{
	class File : private Uncopyable
	{
	private:
		explicit File();

	public:
		static BOOL Exists(const _txstring& path);
		static BOOL Delete(const _txstring& path);

		static BOOL Copy(
			const _txstring& sourceFileName,
			const _txstring& destFileName,
			BOOL overwrite
			);
	};
}