#pragma once

#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Lucky
{
	class Input
	{
	public:
		static bool IsKeyPressed(Key keyCode);
		static bool IsMouseButtonPressed(MouseButton button);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
