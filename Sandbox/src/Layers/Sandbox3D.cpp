#include "SandboxPch.h"
#include "Sandbox3D.h"

Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_SquareColor({ 0.0f, 0.407f, 0.48f, 1.0f })
{
	auto& window = Lucky::Application::Get().GetWindow();

	Lucky::Camera::Settings settings;
	settings.AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
	settings.EnableRotation = true;
	settings.Position = { 0.0f, 0.0f, 0.0f };
	settings.TranslationSpeed = 2.0f;
	settings.RotationSpeed = 90.0f;
	settings.ZoomSpeed = 0.25f;
	settings.ZoomLevel = 2.0f;

	settings.PerspectiveFov = 60.0f;
	settings.PerspectiveNearClip = 0.1f;
	settings.PerspectiveFarClip = 10.0f;

	m_CameraController = Lucky::CameraController::Create(Lucky::CameraType::Perspective, settings);
}

void Sandbox3D::OnAttach()
{
	LK_PROFILE_FUNCTION();
	Lucky::Ref<Lucky::VertexBuffer> vertexBuffer;
	Lucky::Ref<Lucky::IndexBuffer> indexBuffer;

#ifdef TRIANGLE
	m_triangleVA = Lucky::VertexArray::Create();
	m_triangleVA->Bind();

	float vertices[3 * 7] =
	{
		-0.5f,  -0.5f,  0.0f,   0.8f,   0.2f,   0.8f,   1.0f,
		0.5f,   -0.5f,  0.0f,   0.2f,   0.3f,   0.8f,   1.0f,
		0.0f,   0.5f,   0.0f,   0.8f,   0.8f,   0.2f,   1.0f
	};
	vertexBuffer = Lucky::VertexBuffer::Create(vertices, sizeof(vertices));
	vertexBuffer->SetLayout({
		{ Lucky::ShaderDataType::Float3, "a_Position" },
		{ Lucky::ShaderDataType::Float4, "a_Color" }
		});
	m_triangleVA->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	indexBuffer = Lucky::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_triangleVA->SetIndexBuffer(indexBuffer);
#endif

	m_squareVA = Lucky::VertexArray::Create();
	m_squareVA->Bind();

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
	m_squareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Lucky::Ref<Lucky::IndexBuffer> squareIB;
	squareIB = Lucky::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_squareVA->SetIndexBuffer(squareIB);

	// Shaders
	auto& shaderLibrary = Lucky::ShaderLibrary::GetInstance();
	if (!shaderLibrary.Exists("VertexPosColor"))
		shaderLibrary.Load("assets/shaders/VertexPosColor.glsl");
	if (!shaderLibrary.Exists("FlatColor"))
		shaderLibrary.Load("assets/shaders/FlatColor.glsl");
	if (!shaderLibrary.Exists("Texture"))
		shaderLibrary.Load("assets/shaders/Texture.glsl");

	// Textures
	m_Texture = Lucky::Texture2D::Create("assets/textures/Checkerboard.png");
	m_TexturePlane = Lucky::Texture2D::Create("assets/textures/f14-tomcat.png");
}

void Sandbox3D::OnDetach()
{
	LK_PROFILE_FUNCTION();
}

void Sandbox3D::OnUpdate(Lucky::Timestep ts)
{
	auto& shaderLibrary = Lucky::ShaderLibrary::GetInstance();

	// Background color
	Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Lucky::RenderCommand::Clear();

	m_CameraController->OnUpdate(ts);

	//Lucky::Renderer::BeginScene(m_Camera);
	Lucky::Renderer::BeginScene(m_CameraController);

	auto& flatColorShader = shaderLibrary.Get("FlatColor");

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	flatColorShader->Bind();
	flatColorShader->SetFloat4("u_Color", m_SquareColor);

	for (int y = -10; y < 10; y++)
	{
		for (int x = -10; x < 10; x++)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.11f, y * 0.11f, -0.4f)) * scale;
			Lucky::Renderer::Submit(flatColorShader, m_squareVA, transform);
		}
	}

	auto& textureShader = shaderLibrary.Get("Texture");
	textureShader->Bind();
	textureShader->SetFloat("u_TilingFactor", 1.0f);
	textureShader->SetFloat4("u_Color", glm::vec4(1.0f));

	m_Texture->Bind();
	Lucky::Renderer::Submit(textureShader, m_squareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_TexturePlane->Bind();
	Lucky::Renderer::Submit(textureShader, m_squareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.2f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

#ifdef TRIANGLE
	// Triangle
	m_triangleRotation -= m_triangleRotationSpeed * ts;
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_triangleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.1f)) * rotation;
	auto& vertexPosColorShader = shaderLibrary.Get("VertexPosColor");
	Lucky::Renderer::Submit(vertexPosColorShader, m_triangleVA, transform);
#endif

	Lucky::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	m_CameraController->OnImGuiRender();

	ImGui::Begin("Squares");
	ImGui::ColorEdit4("", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox3D::OnEvent(Lucky::Event& event)
{
	m_CameraController->OnEvent(event);
}
