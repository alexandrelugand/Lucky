#pragma once

namespace Lucky
{
	typedef enum class MouseButton : uint16_t
	{
		ButtonLeft		= 0,
		ButtonRight		= 1,
		ButtonMiddle	= 2,
		Button4			= 3,
		Button5			= 4,
		Button6			= 5,
		Button7			= 6,
		ButtonLast		= 7
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseButton mouseButton)
	{
		os << static_cast<int32_t>(mouseButton);
		return os;
	}
}

#define LK_MOUSE_BUTTON_1         ::Lucky::Mouse::ButtonLeft
#define LK_MOUSE_BUTTON_2         ::Lucky::Mouse::ButtonRight
#define LK_MOUSE_BUTTON_3         ::Lucky::Mouse::ButtonMiddle
#define LK_MOUSE_BUTTON_4         ::Lucky::Mouse::Button4
#define LK_MOUSE_BUTTON_5         ::Lucky::Mouse::Button5
#define LK_MOUSE_BUTTON_6         ::Lucky::Mouse::Button6
#define LK_MOUSE_BUTTON_7         ::Lucky::Mouse::Button7
#define LK_MOUSE_BUTTON_8         ::Lucky::Mouse::ButtonLast
#define LK_MOUSE_BUTTON_LAST      ::Lucky::Mouse::ButtonLast
#define LK_MOUSE_BUTTON_LEFT      ::Lucky::Mouse::ButtonLeft
#define LK_MOUSE_BUTTON_RIGHT     ::Lucky::Mouse::ButtonRight
#define LK_MOUSE_BUTTON_MIDDLE    ::Lucky::Mouse::ButtonMiddle
