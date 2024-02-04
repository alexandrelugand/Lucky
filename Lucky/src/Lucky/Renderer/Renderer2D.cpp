#include "LuckyPch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "ShaderLibrary.h"
#include "RenderCommand.h"
#include "UniformBuffer.h"

namespace Lucky
{
	struct QuadVertex
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f};
		glm::vec2 TexCoord{0.0f, 0.0f };
		glm::vec4 Color{1.0f, 1.0f , 1.0f , 1.0f };
		float TexIndex = 0.0f;
		float TilingFactor = 0.0f;
		int EntityId = -1;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition{ 0.0f, 0.0f, 0.0f };
		glm::vec3 LocalPosition{ 0.0f, 0.0f, 0.0f };
		glm::vec4 Color{ 1.0f, 1.0f , 1.0f , 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;;
		int EntityId = -1;
	};

	struct LineVertex
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec4 Color{ 1.0f, 1.0f , 1.0f , 1.0f };
		int EntityId = -1;
	};

	struct Renderer2DData
	{
		const static uint32_t MaxQuads = 20000;
		const static uint32_t MaxVertices = MaxQuads * 4;
		const static uint32_t MaxIndices = MaxQuads * 6;
#ifndef __EMSCRIPTEN__
		const static uint32_t MaxTextureSlots = 32;
#else
		const static uint32_t MaxTextureSlots = 16;
#endif

		// Quads
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Circles
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		// Lines
		float LineWidth = 2.0f;
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statictics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		LK_PROFILE_FUNCTION();

		// Quad

		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_Position"     },
			{ ShaderDataType::Float2,	"a_TexCoord"     },
			{ ShaderDataType::Float4,	"a_Color"        },
			{ ShaderDataType::Float,	"a_TexIndex"     },
			{ ShaderDataType::Float,	"a_TilingFactor" },
			{ ShaderDataType::Int,		"a_EntityId"     }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for(uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		// Circles
		s_Data.CircleVertexArray = VertexArray::Create();
		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_WorldPosition" },
			{ ShaderDataType::Float3,	"a_LocalPosition" },
			{ ShaderDataType::Float4,	"a_Color"         },
			{ ShaderDataType::Float,	"a_Thickness"     },
			{ ShaderDataType::Float,	"a_Fade"          },
			{ ShaderDataType::Int,		"a_EntityId"      }
		});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// Lines
		s_Data.LineVertexArray = VertexArray::Create();
		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"a_Position" },
			{ ShaderDataType::Float4,	"a_Color"    },
			{ ShaderDataType::Int,		"a_EntityId" }
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteColor = 0xFFFFFFFF;
		s_Data.WhiteTexture->SetData(&whiteColor, sizeof(uint32_t));

		// Shaders
		auto& shaderLibrary = ShaderLibrary::GetInstance();

		if(!shaderLibrary.Exists("Quad"))
			shaderLibrary.LoadByApi("Quad.glsl");
		if (!shaderLibrary.Exists("Circle"))
			shaderLibrary.LoadByApi("Circle.glsl");
		if (!shaderLibrary.Exists("Line"))
			shaderLibrary.LoadByApi("Line.glsl");

		s_Data.QuadShader = shaderLibrary.Get("Quad");
		s_Data.CircleShader = shaderLibrary.Get("Circle");
		s_Data.LineShader = shaderLibrary.Get("Line");
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		if(RendererApi::GetApi() == RendererApi::Api::OpenGL)
			s_Data.CameraUniformBuffer = UniformBuffer::Create("", nullptr, 0, sizeof(Renderer2DData::CameraData));
	}

	void Renderer2D::Shutdown()
	{
		LK_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		LK_PROFILE_FUNCTION();

		const auto view = camera.GetProjection() * glm::inverse(transform);

		if (RendererApi::GetApi() == RendererApi::Api::OpenGL)
		{
			s_Data.CameraBuffer.ViewProjection = view;
			s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		}
		else
		{
			s_Data.QuadShader->Bind();
			s_Data.QuadShader->SetMat4("u_ViewProjection", view);

			s_Data.CircleShader->Bind();
			s_Data.CircleShader->SetMat4("u_ViewProjection", view);
		}

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform, const RenderPass& pass)
	{
		LK_PROFILE_FUNCTION();

		const auto view = camera.GetProjection() * glm::inverse(transform);

		pass.Framebuffer->Bind();
		if (pass.Shader)
			pass.Shader->Bind();

		if (RendererApi::GetApi() == RendererApi::Api::OpenGL)
		{
			s_Data.CameraBuffer.ViewProjection = view;
			s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		}
		else
		{
			s_Data.CameraBuffer.ViewProjection = view;
			pass.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		}

		if (pass.BeforeRenderCallback)
			pass.BeforeRenderCallback(pass);

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		LK_PROFILE_FUNCTION();
		if (RendererApi::GetApi() == RendererApi::Api::OpenGL)
		{
			s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
			s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		}
		else
		{
			s_Data.QuadShader->Bind();
			s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

			s_Data.CircleShader->Bind();
			s_Data.CircleShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		}

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera, const RenderPass& pass)
	{
		LK_PROFILE_FUNCTION();

		pass.Framebuffer->Bind();
		if(pass.Shader)
			pass.Shader->Bind();

		if (RendererApi::GetApi() == RendererApi::Api::OpenGL)
		{
			s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
			s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		}
		else
		{
			s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
			pass.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		}

		if (pass.BeforeRenderCallback)
			pass.BeforeRenderCallback(pass);

		StartBatch();
	}

	void Renderer2D::BeginScene(const BaseCamera& baseCamera)
	{
		LK_PROFILE_FUNCTION();
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", baseCamera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const Scope<CameraController>& cameraController)
	{
		LK_PROFILE_FUNCTION();
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", cameraController->GetCamera().GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		LK_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::EndScene(const RenderPass& pass)
	{
		LK_PROFILE_FUNCTION();

		Flush(pass);

		if (pass.AfterRenderCallback)
			pass.AfterRenderCallback(pass);

		if (pass.Shader)
			pass.Shader->Unbind();
		pass.Framebuffer->Unbind();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			s_Data.QuadShader->Bind();

			int32_t samplers[s_Data.MaxTextureSlots];
			for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
				samplers[i] = i;

			//Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.QuadShader->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);

			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::Flush(const RenderPass& pass)
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			if (pass.Shader)
			{
				pass.Shader->Bind();
			}
			else
			{
				int32_t samplers[s_Data.MaxTextureSlots];
				for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
					samplers[i] = i;

				//Bind textures
				for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
					s_Data.TextureSlots[i]->Bind(i);

				s_Data.QuadShader->Bind();
				s_Data.QuadShader->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);
			}
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			if (pass.Shader)
				pass.Shader->Bind();
			else
				s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			if (pass.Shader)
				pass.Shader->Bind();
			else
				s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		LK_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		LK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}
	
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		LK_PROFILE_FUNCTION();
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		LK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		LK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		LK_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		constexpr size_t quadVertexCount = 4;
		constexpr float textureIndex = 0.0f; //White texture
		constexpr float tilingFactor = 1.0f;
		const glm::vec2 textureCoords[] =
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for(size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityId = entityId;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor, int entityId)
	{
		LK_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2 textureCoords[] =
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get()) //Don't compare share_ptr
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityId = entityId;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int entityId)
	{
		LK_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const Ref<Texture>& texture = subTexture->GetTexture();
		const glm::vec2* textureCoords = subTexture->GetTexCoords();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get()) //Don't compare share_ptr
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityId = entityId;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRenderComponent, int entityId)
	{
		if(spriteRenderComponent.Texture)
			DrawQuad(transform, spriteRenderComponent.Texture, spriteRenderComponent.TilingFactor, spriteRenderComponent.Color, entityId);
		else
			DrawQuad(transform, spriteRenderComponent.Color, entityId);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityId)
	{
		LK_PROFILE_FUNCTION();

		// TODO: implements for circle
		/*if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();*/

		constexpr size_t quadVertexCount = 4;
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityId = entityId;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityId)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityId = entityId;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityId = entityId;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityId);
		DrawLine(p1, p2, color, entityId);
		DrawLine(p2, p3, color, entityId);
		DrawLine(p3, p0, color, entityId);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		glm::vec3 vertices[4];

		for (size_t i = 0; i < 4; i++)
			vertices[i] = transform * s_Data.QuadVertexPositions[i];

		DrawLine(vertices[0], vertices[1], color, entityId);
		DrawLine(vertices[1], vertices[2], color, entityId);
		DrawLine(vertices[2], vertices[3], color, entityId);
		DrawLine(vertices[3], vertices[0], color, entityId);
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	void Renderer2D::ResetStats()
	{
		s_Data.Stats.DrawCalls = 0;
		s_Data.Stats.QuadCount = 0;
	}

	Renderer2D::Statictics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::NextBatch()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}
}
