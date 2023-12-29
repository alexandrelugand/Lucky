#pragma once

#include "KeyCodes.h"

namespace Lucky
{
	class Input
	{
	public:
		static bool IsKeyPressed(Key keyCode);
		static bool IsMouseButtonPressed(int button);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
} // namespace Lucky
