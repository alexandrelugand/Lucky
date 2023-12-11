#pragma once

#include "Lucky/Application/Input.h"

namespace Lucky
{
	class WindowsInput : public Input
	{
	protected:
		bool IsKeyPressedImpl(int keyCode) override;
		bool IsMouseButtonPressedImpl(int button) override;

		std::pair<float, float> GetMousePositionImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};
} // namespace Lucky
