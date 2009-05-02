#pragma once

namespace Earlgrey
{
	template <typename T> //< See note below
	class circular_buffer
	{
	public:
		explicit circular_buffer(size_t capacity);
	};
}