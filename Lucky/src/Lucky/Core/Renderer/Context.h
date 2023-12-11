#pragma once

namespace Lucky
{
	class Context
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
} // namespace Lucky
