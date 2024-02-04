#include "LuckyPch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Components.h"
#include "imgui_internal.h"

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Lucky
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static RigidBody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyType)
	{
		if (bodyType == NAMEOF_ENUM(RigidBody2DComponent::BodyType::Static).data()) return RigidBody2DComponent::BodyType::Static;
		if (bodyType == NAMEOF_ENUM(RigidBody2DComponent::BodyType::Dynamic).data()) return RigidBody2DComponent::BodyType::Dynamic;
		if (bodyType == NAMEOF_ENUM(RigidBody2DComponent::BodyType::Kinematic).data()) return RigidBody2DComponent::BodyType::Kinematic;

		LK_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUuid();

		if(entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if(entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if(entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFov" << YAML::Value << camera.GetPerspectiveFov();
			out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if(entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			if (src.Texture)
				out << YAML::Key << "Texture" << YAML::Value << src.Texture->GetPath();
			out << YAML::Key << "TilingFactor" << YAML::Value << src.TilingFactor;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap; // CircleRendererComponent

			auto& crc = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << crc.Color;
			out << YAML::Key << "Thickness" << YAML::Value << crc.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << crc.Fade;

			out << YAML::EndMap; // CircleRendererComponent
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap; // RigidBody2DComponent

			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "Type" << YAML::Value << NAMEOF_ENUM(rb2d.Type).data();
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;

			out << YAML::EndMap; // RigidBody2DComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // RigidBody2DComponent

			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d.RestitutionThreshold;

			out << YAML::EndMap; // BoxCollider2DComponent
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap; // CircleCollider2DComponent

			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2d.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2d.RestitutionThreshold;

			out << YAML::EndMap; // CircleCollider2DComponent
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

#ifdef COMPILER_MSVC
#pragma warning( push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
		m_Scene->m_Registry.each([&](auto entityId)
		{
			Entity entity{ entityId, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
#ifdef COMPILER_MSVC
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::error_code err;
		if (!CreateDirectoryRecursive(GetDirectoryName(filePath), err))
		{
			LK_CORE_ERROR("Create directory recursively failed: {0}", err.message());
			return;
		}

#ifndef __EMSCRIPTEN__
		std::ofstream fout(filePath);
		fout << out.c_str();
#else	

		std::string current_filePath = filePath;
		bool exportFile = false;
		auto pos = current_filePath.find("--export=");
		if(pos != std::string::npos)
		{
			exportFile = true;
			current_filePath = current_filePath.replace(pos, 9, "");
		}	

		FILE* f = fopen(current_filePath.c_str(), "wt");
		if (f)
		{
			fwrite(out.c_str(), sizeof(char), out.size(), f);
			fclose(f);
			EM_ASM(
				FS.syncfs(function(err) {
				if (err != undefined)
					console.log("Save scene failed. " + err);
				});
			);
		}

		if(exportFile)
		{
			auto path = std::filesystem::path(current_filePath);
			EM_ASM({
				let content = UTF8ToString($0);
				let fileName = UTF8ToString($1);
				const link = document.createElement("a");
				const file = new Blob([content], { type: 'text/plain' });
				link.href = URL.createObjectURL(file);
				link.download = fileName;
				link.click();
				URL.revokeObjectURL(link.href);
			}, out.c_str(), path.filename().c_str());
		}
#endif
	}

	void SceneSerializer::SerializeRuntime(const std::string& filePath)
	{
		LK_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filePath)
	{
		try
		{
			YAML::Node yaml;
#ifndef __EMSCRIPTEN__
			try
			{
				yaml = YAML::LoadFile(filePath);
			}
			catch (const YAML::ParserException& e)
			{
				LK_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filePath, e.what());
				return false;
			}
#else
			FILE *f = fopen(filePath.c_str(), "rb");
			fseek(f, 0, SEEK_END);
			long fsize = ftell(f);
			fseek(f, 0, SEEK_SET);

			char* data = (char*)malloc(fsize + 1);
			fread(data, fsize, 1, f);
			fclose(f);
			data[fsize] = 0;

			try
			{
				yaml = YAML::Load(data);
			}
			catch (const YAML::ParserException& e)
			{
				LK_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filePath, e.what());
				return false;
			}
#endif
			if (!yaml["Scene"])
			{
				LK_CORE_ERROR("Deserialize scene failed: No scene node");
				return false;
			}

			std::string sceneName = yaml["Scene"].as<std::string>();
			LK_CORE_TRACE("Deserializing scene '{0}'", sceneName);

			auto entities = yaml["Entities"];
			if(entities)
			{
				for(auto entity : entities)
				{
					uint64_t uuid = entity["Entity"].as<uint64_t>();

					std::string name;
					auto tagComponent = entity["TagComponent"];
					if (tagComponent)
						name = tagComponent["Tag"].as<std::string>();

					LK_CORE_TRACE("Deserializing entity with Id = {0}, name = {1}", uuid, name);

					Entity deserializedEntity = m_Scene->CreateEntity(uuid, name);

					auto transformComponent = entity["TransformComponent"];
					if(transformComponent)
					{
						// Entities always have transforms
						auto& tc = deserializedEntity.GetComponent<TransformComponent>();
						tc.Translation = transformComponent["Translation"].as<glm::vec3>();
						tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
						tc.Scale = transformComponent["Scale"].as<glm::vec3>();
					}

					auto cameraComponent = entity["CameraComponent"];
					if (cameraComponent)
					{
						auto& cc = deserializedEntity.AddComponent<CameraComponent>();

						auto cameraProps = cameraComponent["Camera"];
						cc.Camera.SetProjectionType((Camera::ProjectionType)cameraProps["ProjectionType"].as<int>());

						cc.Camera.SetPerspectiveFov(cameraProps["PerspectiveFov"].as<float>());
						cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNearClip"].as<float>());
						cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFarClip"].as<float>());

						cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
						cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNearClip"].as<float>());
						cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFarClip"].as<float>());

						cc.Primary = cameraComponent["Primary"].as<bool>();
						cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
					}

					auto spriteRendererComponent = entity["SpriteRendererComponent"];
					if (spriteRendererComponent)
					{
						auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
						src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
						if (spriteRendererComponent["Texture"])
						{
							auto texturePath = spriteRendererComponent["Texture"].as<std::string>();
							src.Texture = Texture2D::Create(texturePath);
						}
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
					}

					auto circleRendererComponent = entity["CircleRendererComponent"];
					if (circleRendererComponent)
					{
						auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
						crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
						crc.Thickness = circleRendererComponent["Thickness"].as<float>();
						crc.Fade = circleRendererComponent["Fade"].as<float>();
					}

					auto rigidBody2DComponent = entity["RigidBody2DComponent"];
					if (rigidBody2DComponent)
					{
						auto& rb2d = deserializedEntity.AddComponent<RigidBody2DComponent>();
						rb2d.Type = RigidBody2DBodyTypeFromString(rigidBody2DComponent["Type"].as<std::string>());
						rb2d.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
					}

					auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
					if (boxCollider2DComponent)
					{
						auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
						bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
						bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
						bc2d.Density = boxCollider2DComponent["Density"].as<float>();
						bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
						bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
						bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
					}

					auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
					if (circleCollider2DComponent)
					{
						auto& bc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
						bc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
						bc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
						bc2d.Density = circleCollider2DComponent["Density"].as<float>();
						bc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
						bc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
						bc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
					}
				}
			}

			return true;
		}
		catch (const YAML::ParserException& e)
		{
			LK_CORE_ERROR("Deserialize scene failed. YAML Error: {0}", e.what());
			return false;
		}
		catch(const std::exception& e)
		{
			LK_CORE_ERROR("Deserialize scene failed: {0}", e.what());
			return false;
		}
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filePath)
	{
		LK_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}
