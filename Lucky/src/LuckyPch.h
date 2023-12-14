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
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#ifdef PLATFORM_WINDOWS
	#include <Window.h>
#endif

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

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "stb/stb_image.h"


// #include "Lucky/Application/MouseButtonCodes.h"
// #include "Lucky/Application/Window.h"

// #include "Lucky/Core/Timestep.h"
// #include "Lucky/Core/Layer.h"
// #include "Lucky/Core/LayerStack.h"
// #include "Lucky/Core/Log.h"
// #include "Lucky/Core/Events/ApplicationEvent.h"
// #include "Lucky/Core/Events/Event.h"
// #include "Lucky/Core/Events/KeyEvent.h"
// #include "Lucky/Core/Events/MouseEvent.h"
// #include "Lucky/Core/ImGui/ImGuiLayer.h"
// #include "Lucky/Core/Renderer/Renderer.h"
// #include "Lucky/Core/Renderer/RendererApi.h"
// #include "Lucky/Core/Renderer/RenderCommand.h"
// #include "Lucky/Core/Renderer/Context.h"
// #include "Lucky/Core/Renderer/VertexArray.h"
// #include "Lucky/Core/Renderer/VertexBuffer.h"
// #include "Lucky/Core/Renderer/IndexBuffer.h"
// #include "Lucky/Core/Renderer/VertexBufferLayout.h"
// #include "Lucky/Core/Renderer/Shader.h"
// #include "Lucky/Core/Renderer/Texture.h"
// #include "Lucky/Core/Renderer/Camera.h"
// #include "Lucky/Core/Renderer/OrthographicCamera.h"
// #include "Lucky/Core/Renderer/PerspectiveCamera.h"

// #include "Lucky/Platforms/OpenGL/OpenGLRendererApi.h"
// #include "Lucky/Platforms/OpenGL/OpenGLContext.h"
// #include "Lucky/Platforms/OpenGL/OpenGLVertexArray.h"
// #include "Lucky/Platforms/OpenGL/OpenGLVertexBuffer.h"
// #include "Lucky/Platforms/OpenGL/OpenGLIndexBuffer.h"
// #include "Lucky/Platforms/OpenGL/OpenGLShader.h"
// #include "Lucky/Platforms/OpenGL/OpenGLTexture2D.h"
// #include "Lucky/Platforms/Windows/WindowsInput.h"
// #include "Lucky/Platforms/Windows/WindowsWindow.h"