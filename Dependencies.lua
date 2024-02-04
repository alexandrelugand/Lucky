-- Lucky dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

includeDir = {}
includeDir["spdlog"]="%{wks.location}/Vendors/spdlog/include"
includeDir["stb"]="%{wks.location}/Vendors/stb"
includeDir["yaml"]="%{wks.location}/Vendors/yaml-cpp/include"
includeDir["GLFW"]="%{wks.location}/Vendors/GLFW/include"
includeDir["GLAD"]="%{wks.location}/Vendors/GLAD/include"
includeDir["ImGui"]="%{wks.location}/Vendors/ImGui"
includeDir["ImGuizmo"]="%{wks.location}/Vendors/ImGuizmo"
includeDir["GLM"]="%{wks.location}/Vendors/GLM"
includeDir["entt"]="%{wks.location}/Vendors/entt/include"
includeDir["nameof"]="%{wks.location}/Vendors/nameof"
includeDir["Box2D"]="%{wks.location}/Vendors/Box2D/include"
includeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
includeDir["SPIRV_Cross"] = "%{VULKAN_SDK}/spirv_cross"
includeDir["shaderc"] = "%{VULKAN_SDK}/Include/shaderc"

libDir = {}
libDir["GLFW"] = "%{wks.location}/Vendors/GLFW/lib-vc2022"
libDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

library = {}
library["Vulkan"] = "%{libDir.VulkanSDK}/vulkan-1.lib"
library["VulkanUtils"] = "%{libDir.VulkanSDK}/VkLayer_utils.lib"

library["ShaderC_Debug"] = "%{libDir.VulkanSDK}/shaderc_sharedd.lib"
library["SPIRV_Cross_Debug"] = "%{libDir.VulkanSDK}/spirv-cross-cored.lib"
library["SPIRV_Cross_GLSL_Debug"] = "%{libDir.VulkanSDK}/spirv-cross-glsld.lib"
library["SPIRV_Tools_Debug"] = "%{libDir.VulkanSDK}/SPIRV-Toolsd.lib"

library["ShaderC_Release"] = "%{libDir.VulkanSDK}/shaderc_shared.lib"
library["SPIRV_Cross_Release"] = "%{libDir.VulkanSDK}/spirv-cross-core.lib"
library["SPIRV_Cross_GLSL_Release"] = "%{libDir.VulkanSDK}/spirv-cross-glsl.lib"
