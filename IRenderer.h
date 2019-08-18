#pragma once

namespace fb
{
	class IRenderer
	{
	public:
		virtual bool Initialize(void* windowHandle) = 0;
		virtual bool Finalize() = 0;
	};
}

