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

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <GLES3/gl3.h>
#else
	#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>