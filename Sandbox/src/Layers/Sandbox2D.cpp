#include "SandboxPch.h"
#include "Sandbox2D.h"

static const uint32_t s_MapWidth = 18;
static const uint32_t s_MapHeight = 18;

static const uint32_t s_TileWidth = 24;
static const char* s_MapTiles =
"                        "
"           7________9   "
"           lXXXXXXXX|   "
"           lXa----zX|   "
" 7_________sX|    lX|   "
" lXXXXXXXXXXX|    lX|   "
" lXa---------3    lX|   "
" lX|              lX|   "
" lX|              lX|   "
" lX|              lX|   "
" lX|              lX|   "
" lX|              lX|   "
" lXq______T_______sX|   "
" lXXXXXXXXGXXXXXXXXX|   "
" 1--------B---------3   "
;

std::unordered_map<char, Lucky::Ref<Lucky::SubTexture2D>> Sandbox2D::s_TextureMap;
std::unordered_map<uint32_t, Lucky::Ref<Lucky::SubTexture2D>> Sandbox2D::s_SpriteSheetMap;
std::unordered_map<uint32_t, Lucky::Ref<Lucky::SubTexture2D>> Sandbox2D::s_NumberMap;

Sandbox2D::Sandbox2D()
    :   Layer("Sandbox2D"), 
        m_SquareColor({0.0f, 0.407f, 0.48f, 1.0f})
{        
	auto& window = Lucky::Application::Get().GetWindow();

    Lucky::CameraSettings settings;
    settings.AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
    settings.EnableRotation = true;
    settings.TranslationSpeed = 10.0f;
    settings.RotationSpeed = 90.0f;
	settings.ZoomSpeed = 0.5f;
	settings.ZoomLevel = 10.0f;

    m_CameraController = Lucky::CameraController::Create(Lucky::CameraType::Orthographic, settings);

	m_MapWidth = s_TileWidth;
	m_MapHeight = (uint32_t)strlen(s_MapTiles) / s_TileWidth;
}

void Sandbox2D::OnAttach()
{
	LK_PROFILE_FUNCTION();

	LoadGameSprites();
	LoadToolTextures();

	// Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{
	LK_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Lucky::Timestep ts)
{
	LK_PROFILE_FUNCTION();

	m_CameraController->OnUpdate(ts);

	Lucky::Renderer2D::ResetStats();

    // Prepare scene
	{
		LK_PROFILE_SCOPE("void Sandbox2D::OnUpdate() - Prepare");
		Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Lucky::RenderCommand::Clear();
	}

	// Example scene
#if 0
	{
		LK_PROFILE_SCOPE("void Sandbox2D::OnUpdate() - Example scene");
		Lucky::Renderer2D::BeginScene(m_CameraController);

		static float rotation = 0.0f;
		rotation += ts * 45.0f;

		Lucky::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Lucky::Renderer2D::DrawQuad({ 0.5f, -0.5f}, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Lucky::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.5f }, { 20.0f, 20.0f }, m_Texture, 10.0f);
		Lucky::Renderer2D::DrawRotateQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_Texture, 20.0f);
		Lucky::Renderer2D::DrawRotateQuad(m_SquarePosition, m_SquareSize, glm::radians(m_SquareRotation), m_SquareColor);

		for(float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Lucky::Renderer2D::DrawQuad({ x, y, -0.4f }, { 0.45f, 0.45f }, color);
			}
		}

		Lucky::Renderer2D::EndScene();
	}
#endif

	// Tool texture map
	{
		LK_PROFILE_SCOPE("void Sandbox2D::OnUpdate() - Tool texture map");
		Lucky::Renderer2D::BeginScene(m_CameraController);

		for (uint32_t y = 0; y < s_MapHeight + 1; y++)
		{
			for (uint32_t x = 0; x < s_MapWidth + 1; x++)
			{
				Lucky::Ref<Lucky::SubTexture2D> texture;
				if (x == 0 && y == 0)
				{
					texture = m_EmptySprite;
					Lucky::Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f + 0.1f, (y - s_MapHeight / 2.0f) + 20.0f, 0.5f }, { 1.0f, 1.0f }, texture);
					continue;
				}

				if (y == 0)
				{
					texture = s_NumberMap[x - 1];
					Lucky::Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f, (y - s_MapHeight / 2.0f) + 20.0f, 0.5f }, { 1.0f, 1.0f }, texture);
					continue;
				}

				if (x == 0)
				{
					texture = s_NumberMap[y - 1];
					Lucky::Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f, (y - s_MapHeight / 2.0f) + 20.0f, 0.5f }, { 1.0f, 1.0f }, texture);
					continue;
				}

				texture = s_SpriteSheetMap[(x - 1) + (y - 1) * s_MapWidth];
				Lucky::Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f, (y - s_MapHeight / 2.0f) + 20.0f, 0.5f }, { 0.9f, 0.9f }, texture);
			}
		}

		Lucky::Renderer2D::EndScene();
	}

	{
		LK_PROFILE_SCOPE("void Sandbox2D::OnUpdate() - Game level");
		Lucky::Renderer2D::BeginScene(m_CameraController);

		// Particle system
		if (Lucky::Input::IsMouseButtonPressed(LK_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Lucky::Input::GetMousePosition();
			auto width = Lucky::Application::Get().GetWindow().GetWidth();
			auto height = Lucky::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController->GetBounds();
			auto pos = m_CameraController->GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 50; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender();

		Lucky::Renderer2D::EndScene();

		// Game level (circuit)
		Lucky::Renderer2D::BeginScene(m_CameraController);
		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				Lucky::Ref<Lucky::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
					texture = s_TextureMap[tileType];
				else
					texture = m_EmptySprite;

				Lucky::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, (m_MapHeight - y - m_MapHeight / 2.0f), 0.5f }, { 1.06f, 1.06f }, texture);
			}
		}

		Lucky::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
    m_CameraController->OnImGuiRender();

    ImGui::Begin("Square");
    ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));
    ImGui::DragFloat2("Position", glm::value_ptr(m_SquarePosition), 0.1f, -100.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::DragFloat("Rotation", &m_SquareRotation, 1.0f, -360.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::DragFloat2("Size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::End();

	auto stats = Lucky::Renderer2D::GetStats();
	ImGui::Begin("Stats");
	ImGui::Text("Draw calls: %d", stats.DrawCalls),
	ImGui::Text("Quads: %d", stats.QuadCount),
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount()),
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount()),
	ImGui::End();
}

void Sandbox2D::OnEvent(Lucky::Event &event)
{
    m_CameraController->OnEvent(event);
}

void Sandbox2D::LoadGameSprites()
{
	m_Texture = Lucky::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Lucky::Texture2D::Create("assets/game/textures/spritesheet_tiles.png");
	m_EmptySheet = Lucky::Texture2D::Create("assets/game/textures/Empty.png");

	for (uint32_t y = 0; y < s_MapHeight; y++)
	{
		for (uint32_t x = 0; x < s_MapWidth; x++)
		{
			s_SpriteSheetMap[x + y * s_MapWidth] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { x + y * s_MapWidth, y }, { 128, 128 });
		}
	}

	m_EmptySprite = Lucky::SubTexture2D::CreateFromCoords(m_EmptySheet, { 0, 0 }, { 128, 128 });
	s_TextureMap[' '] = m_EmptySprite;
	s_TextureMap['G'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 4 }, { 128, 128 });
	s_TextureMap['X'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 7 }, { 128, 128 });

	s_TextureMap['_'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 5, 7 }, { 128, 128 }); //Top
	s_TextureMap['-'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 7 }, { 128, 128 }); //Bottom

	s_TextureMap['l'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 6 }, { 128, 128 }); //Left
	s_TextureMap['|'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 8 }, { 128, 128 }); //Right

	s_TextureMap['9'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 5, 10 }, { 128, 128 });
	s_TextureMap['z'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 13 }, { 128, 128 });

	s_TextureMap['3'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 10 }, { 128, 128 });
	s_TextureMap['s'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 13 }, { 128, 128 });

	s_TextureMap['7'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 5, 9 }, { 128, 128 });
	s_TextureMap['a'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 12 }, { 128, 128 });

	s_TextureMap['1'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 9 }, { 128, 128 });
	s_TextureMap['q'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 12 }, { 128, 128 });

	s_TextureMap['T'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 0 }, { 128, 128 });
	s_TextureMap['G'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 1 }, { 128, 128 });
	s_TextureMap['B'] = Lucky::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 2 }, { 128, 128 });
}

void Sandbox2D::LoadToolTextures()
{
	m_NumberSheet = Lucky::Texture2D::Create("assets/game/textures/Numbers.png");

	for (uint32_t y = 0; y < 3; y++)
	{
		for (uint32_t x = 0; x < 10; x++)
		{
			s_NumberMap[x + y * 10] = Lucky::SubTexture2D::CreateFromCoords(m_NumberSheet, { x + y * 10, y }, { 128, 128 });
		}
	}
}
