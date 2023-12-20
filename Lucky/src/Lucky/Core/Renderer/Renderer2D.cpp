#include "LuckyPch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "ShaderLibrary.h"
#include "RenderCommand.h"
#include "ShaderLibrary.h"

namespace Lucky
{
	struct Renderer2DData
	{
		Ref<Lucky::VertexArray> squareVA;
		Ref<Shader> TextureShader;
		Ref<Texture> WhiteTexture;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Init()
	{
		s_Data= new Renderer2DData();
		s_Data->squareVA = Lucky::VertexArray::Create();
		s_Data->squareVA->Bind();

		float squareVertices[5 * 4] = 
		{
			-0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,
			0.5f,   -0.5f,  0.0f,   1.0f,   0.0f,
			0.5f,   0.5f,   0.0f,   1.0f,   1.0f,
			-0.5f,  0.5f,   0.0f,   0.0f,   1.0f
		};

		Lucky::Ref<Lucky::VertexBuffer> squareVB;
		squareVB = Lucky::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Lucky::ShaderDataType::Float3, "a_Position" },
			{ Lucky::ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data->squareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Lucky::Ref<Lucky::IndexBuffer> squareIB;
		squareIB = Lucky::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->squareVA->SetIndexBuffer(squareIB);

		// Shaders
		auto& shaderLibrary = ShaderLibrary::GetInstance();
		s_Data->TextureShader =shaderLibrary.Load("assets/shaders/Texture.glsl");

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteColor = 0xFFFFFFFF;
		s_Data->WhiteTexture->SetData(&whiteColor, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Scope<CameraController>& cameraController)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", cameraController->GetCamera().GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
	{
		DrawQuad({ position.x, position.y, 0.0f}, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
	{
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
							glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
							glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->squareVA->Bind();
		RenderCommand::DrawIndexed(s_Data->squareVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f}, size, rotation, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<Texture>& texture)
	{
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
							glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
							glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->squareVA->Bind();
		RenderCommand::DrawIndexed(s_Data->squareVA);
	}

} // namespace Lucky
