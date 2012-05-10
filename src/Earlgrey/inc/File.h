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
		static BOOL Exists(const xstring& path);
		static BOOL Exists(const xwstring& path);
		static BOOL Delete(const xwstring& path);
		static BOOL Delete(const xstring& path);

		static BOOL Copy(
			const xwstring& sourceFileName,
			const xwstring& destFileName,
			BOOL overwrite
			);

		static BOOL Copy(
			const xstring& sourceFileName,
			const xstring& destFileName,
			BOOL overwrite
			);
	};
}