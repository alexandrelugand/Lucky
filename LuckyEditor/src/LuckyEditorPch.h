#ifdef _MSC_VER
#pragma once
#endif

#include <stdint.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <format>

#include <string>
#include <sstream>
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
#include <nameof.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_decompose.hpp>
