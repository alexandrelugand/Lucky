#pragma once

namespace Lucky
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space              = 32,
		Apostrophe         = 39,  /* ' */
		Comma              = 44,  /* , */
		Minus              = 45,  /* - */
		Period             = 46,  /* . */
		Slash              = 47,  /* / */

		D0                 = 48,
		D1                 = 49,
		D2                 = 50,
		D3                 = 51,
		D4                 = 52,
		D5                 = 53,
		D6                 = 54,
		D7                 = 55,
		D8                 = 56,
		D9                 = 57,

		Semicolon          = 59,  /* ; */
		Equal              = 61,  /* = */

		A                  = 65,
		B                  = 66,
		C                  = 67,
		D                  = 68,
		E                  = 69,
		F                  = 70,
		G                  = 71,
		H                  = 72,
		I                  = 73,
		J                  = 74,
		K                  = 75,
		L                  = 76,
		M                  = 77,
		N                  = 78,
		O                  = 79,
		P                  = 80,
		Q                  = 81,
		R                  = 82,
		S                  = 83,
		T                  = 84,
		U                  = 85,
		V                  = 86,
		W                  = 87,
		X                  = 88,
		Y                  = 89,
		Z                  = 90,

		LeftBracket        = 91,  /* [ */
		Backslash          = 92,  /* \ */
		RightBracket       = 93,  /* ] */
		GraveAccent        = 96,  /* ` */

		World1             = 161, /* non-US #1 */
		World2             = 162, /* non-US #2 */

		/* Function keys */
		Escape             = 256,
		Enter              = 257,
		Tab                = 258,
		Backspace          = 259,
		Insert             = 260,
		Delete             = 261,
		Right              = 262,
		Left               = 263,
		Down               = 264,
		Up                 = 265,
		PageUp             = 266,
		PageDown           = 267,
		Home               = 268,
		End                = 269,
		CapsLock           = 280,
		ScrollLock         = 281,
		NumLock            = 282,
		PrintScreen        = 283,
		Pause              = 284,
		F1                 = 290,
		F2                 = 291,
		F3                 = 292,
		F4                 = 293,
		F5                 = 294,
		F6                 = 295,
		F7                 = 296,
		F8                 = 297,
		F9                 = 298,
		F10                = 299,
		F11                = 300,
		F12                = 301,
		F13                = 302,
		F14                = 303,
		F15                = 304,
		F16                = 305,
		F17                = 306,
		F18                = 307,
		F19                = 308,
		F20                = 309,
		F21                = 310,
		F22                = 311,
		F23                = 312,
		F24                = 313,
		F25                = 314,
		KP0                = 320,
		KP1                = 321,
		KP2                = 322,
		KP3                = 323,
		KP4                = 324,
		KP5                = 325,
		KP6                = 326,
		KP7                = 327,
		KP8                = 328,
		KP9                = 329,
		KPDecimal          = 330,
		KPDivide           = 331,
		KPMultiply         = 332,
		KPSubtract         = 333,
		KPAdd              = 334,
		KPEnter            = 335,
		KPEqual            = 336,

		LeftShift          = 340,
		LeftControl        = 341,
		LeftAlt            = 342,
		LeftSuper          = 343,
		RightShift         = 344,
		RightControl       = 345,
		RightAlt           = 346,
		RightSuper         = 347,
		Menu               = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

#define LK_KEY_SPACE              ::Lucky::Key::Space      
#define LK_KEY_APOSTROPHE         ::Lucky::Key::Apostrophe   /* ' */
#define LK_KEY_COMMA              ::Lucky::Key::Comma        /* , */
#define LK_KEY_MINUS              ::Lucky::Key::Minus        /* - */
#define LK_KEY_PERIOD             ::Lucky::Key::Period       /* . */
#define LK_KEY_SLASH              ::Lucky::Key::Slash        /* / */
#define LK_KEY_0                  ::Lucky::Key::D0
#define LK_KEY_1                  ::Lucky::Key::D1
#define LK_KEY_2                  ::Lucky::Key::D2
#define LK_KEY_3                  ::Lucky::Key::D3
#define LK_KEY_4                  ::Lucky::Key::D4
#define LK_KEY_5                  ::Lucky::Key::D5
#define LK_KEY_6                  ::Lucky::Key::D6
#define LK_KEY_7                  ::Lucky::Key::D7
#define LK_KEY_8                  ::Lucky::Key::D8
#define LK_KEY_9                  ::Lucky::Key::D9
#define LK_KEY_SEMICOLON          ::Lucky::Key::Semicolon  /* ; */
#define LK_KEY_EQUAL              ::Lucky::Key::Equal      /* = */
#define LK_KEY_A                  ::Lucky::Key::A
#define LK_KEY_B                  ::Lucky::Key::B
#define LK_KEY_C                  ::Lucky::Key::C
#define LK_KEY_D                  ::Lucky::Key::D
#define LK_KEY_E                  ::Lucky::Key::E
#define LK_KEY_F                  ::Lucky::Key::F
#define LK_KEY_G                  ::Lucky::Key::G
#define LK_KEY_H                  ::Lucky::Key::H
#define LK_KEY_I                  ::Lucky::Key::I
#define LK_KEY_J                  ::Lucky::Key::J
#define LK_KEY_K                  ::Lucky::Key::K
#define LK_KEY_L                  ::Lucky::Key::L
#define LK_KEY_M                  ::Lucky::Key::M
#define LK_KEY_N                  ::Lucky::Key::N
#define LK_KEY_O                  ::Lucky::Key::O
#define LK_KEY_P                  ::Lucky::Key::P
#define LK_KEY_Q                  ::Lucky::Key::Q
#define LK_KEY_R                  ::Lucky::Key::R
#define LK_KEY_S                  ::Lucky::Key::S
#define LK_KEY_T                  ::Lucky::Key::T
#define LK_KEY_U                  ::Lucky::Key::U
#define LK_KEY_V                  ::Lucky::Key::V
#define LK_KEY_W                  ::Lucky::Key::W
#define LK_KEY_X                  ::Lucky::Key::X
#define LK_KEY_Y                  ::Lucky::Key::Y
#define LK_KEY_Z                  ::Lucky::Key::Z
#define LK_KEY_LEFT_BRACKET       ::Lucky::Key::LeftBracket   /* [ */
#define LK_KEY_BACKSLASH          ::Lucky::Key::Backslash     /* \ */
#define LK_KEY_RIGHT_BRACKET      ::Lucky::Key::RightBracket  /* ] */
#define LK_KEY_GRAVE_ACCENT       ::Lucky::Key::GraveAccent   /* ` */
#define LK_KEY_WORLD_1            ::Lucky::Key::World1 /* non-US #1 */
#define LK_KEY_WORLD_2            ::Lucky::Key::World2 /* non-US #2 */

/* Function keys */
#define LK_KEY_ESCAPE             ::Lucky::Key::Escape     
#define LK_KEY_ENTER              ::Lucky::Key::Enter      
#define LK_KEY_TAB                ::Lucky::Key::Tab        
#define LK_KEY_BACKSPACE          ::Lucky::Key::Backspace  
#define LK_KEY_INSERT             ::Lucky::Key::Insert     
#define LK_KEY_DELETE             ::Lucky::Key::Delete     
#define LK_KEY_RIGHT              ::Lucky::Key::Right      
#define LK_KEY_LEFT               ::Lucky::Key::Left       
#define LK_KEY_DOWN               ::Lucky::Key::Down       
#define LK_KEY_UP                 ::Lucky::Key::Up         
#define LK_KEY_PAGE_UP            ::Lucky::Key::PageUp     
#define LK_KEY_PAGE_DOWN          ::Lucky::Key::PageDown   
#define LK_KEY_HOME               ::Lucky::Key::Home       
#define LK_KEY_END                ::Lucky::Key::End        
#define LK_KEY_CAPS_LOCK          ::Lucky::Key::CapsLock   
#define LK_KEY_SCROLL_LOCK        ::Lucky::Key::ScrollLock 
#define LK_KEY_NUM_LOCK           ::Lucky::Key::NumLock    
#define LK_KEY_PRINT_SCREEN       ::Lucky::Key::PrintScreen
#define LK_KEY_PAUSE              ::Lucky::Key::Pause      
#define LK_KEY_F1                 ::Lucky::Key::F1 
#define LK_KEY_F2                 ::Lucky::Key::F2 
#define LK_KEY_F3                 ::Lucky::Key::F3 
#define LK_KEY_F4                 ::Lucky::Key::F4 
#define LK_KEY_F5                 ::Lucky::Key::F5 
#define LK_KEY_F6                 ::Lucky::Key::F6 
#define LK_KEY_F7                 ::Lucky::Key::F7 
#define LK_KEY_F8                 ::Lucky::Key::F8 
#define LK_KEY_F9                 ::Lucky::Key::F9 
#define LK_KEY_F10                ::Lucky::Key::F10
#define LK_KEY_F11                ::Lucky::Key::F11
#define LK_KEY_F12                ::Lucky::Key::F12
#define LK_KEY_F13                ::Lucky::Key::F13
#define LK_KEY_F14                ::Lucky::Key::F14
#define LK_KEY_F15                ::Lucky::Key::F15
#define LK_KEY_F16                ::Lucky::Key::F16
#define LK_KEY_F17                ::Lucky::Key::F17
#define LK_KEY_F18                ::Lucky::Key::F18
#define LK_KEY_F19                ::Lucky::Key::F19
#define LK_KEY_F20                ::Lucky::Key::F20
#define LK_KEY_F21                ::Lucky::Key::F21
#define LK_KEY_F22                ::Lucky::Key::F22
#define LK_KEY_F23                ::Lucky::Key::F23
#define LK_KEY_F24                ::Lucky::Key::F24
#define LK_KEY_F25                ::Lucky::Key::F25
#define LK_KEY_KP_0               ::Lucky::Key::KP0
#define LK_KEY_KP_1               ::Lucky::Key::KP1
#define LK_KEY_KP_2               ::Lucky::Key::KP2
#define LK_KEY_KP_3               ::Lucky::Key::KP3
#define LK_KEY_KP_4               ::Lucky::Key::KP4
#define LK_KEY_KP_5               ::Lucky::Key::KP5
#define LK_KEY_KP_6               ::Lucky::Key::KP6
#define LK_KEY_KP_7               ::Lucky::Key::KP7
#define LK_KEY_KP_8               ::Lucky::Key::KP8
#define LK_KEY_KP_9               ::Lucky::Key::KP9
#define LK_KEY_KP_DECIMAL         ::Lucky::Key::KPDecimal 
#define LK_KEY_KP_DIVIDE          ::Lucky::Key::KPDivide  
#define LK_KEY_KP_MULTIPLY        ::Lucky::Key::KPMultiply
#define LK_KEY_KP_SUBTRACT        ::Lucky::Key::KPSubtract
#define LK_KEY_KP_ADD             ::Lucky::Key::KPAdd     
#define LK_KEY_KP_ENTER           ::Lucky::Key::KPEnter   
#define LK_KEY_KP_EQUAL           ::Lucky::Key::KPEqual   
#define LK_KEY_LEFT_SHIFT         ::Lucky::Key::LeftShift   
#define LK_KEY_LEFT_CONTROL       ::Lucky::Key::LeftControl 
#define LK_KEY_LEFT_ALT           ::Lucky::Key::LeftAlt     
#define LK_KEY_LEFT_SUPER         ::Lucky::Key::LeftSuper   
#define LK_KEY_RIGHT_SHIFT        ::Lucky::Key::RightShift  
#define LK_KEY_RIGHT_CONTROL      ::Lucky::Key::RightControl
#define LK_KEY_RIGHT_ALT          ::Lucky::Key::RightAlt    
#define LK_KEY_RIGHT_SUPER        ::Lucky::Key::RightSuper  
#define LK_KEY_MENU               ::Lucky::Key::Menu        
