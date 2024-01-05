#ifdef _MSC_VER
#pragma once
#endif

#include <cstdint>
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <format>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <GLES3/gl3.h>
#else
	#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Lucky/Core/Base.h"
#include "Lucky/Core/Log.h"
#include "Lucky/Core/Assert.h"
#include "Lucky/Debug/Instrumentation.h"

#include "stb/stb_image.h"
