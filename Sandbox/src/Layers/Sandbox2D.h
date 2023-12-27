#pragma once
#include <Lucky.h>

class Sandbox2D : public Lucky::Layer
{
public:
    Sandbox2D();

	void OnAttach() override;
	void OnDetach() override;
    void OnUpdate(Lucky::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Lucky::Event& event) override;

private:
	void LoadGameSprites();
	void LoadToolTextures();

    Lucky::Scope<Lucky::CameraController> m_CameraController;
    glm::vec2 m_SquarePosition = { 0.0f, 0.0f };
    glm::vec2 m_SquareSize = { 1.0f, 1.0f };
    float m_SquareRotation = 0.0f;
    glm::vec4 m_SquareColor;
	Lucky::Ref<Lucky::Texture2D> m_Texture;
	Lucky::Ref<Lucky::Texture2D> m_SpriteSheet;
	Lucky::Ref<Lucky::Texture2D> m_NumberSheet;
	Lucky::Ref<Lucky::Texture2D> m_EmptySheet;
	Lucky::Ref<Lucky::SubTexture2D> m_EmptySprite;
	Lucky::ParticleProps m_Particle;
	Lucky::ParticleSystem m_ParticleSystem;
	static std::unordered_map<char, Lucky::Ref<Lucky::SubTexture2D>> s_TextureMap;
	static std::unordered_map<uint32_t, Lucky::Ref<Lucky::SubTexture2D>> s_SpriteSheetMap;
	static std::unordered_map<uint32_t, Lucky::Ref<Lucky::SubTexture2D>> s_NumberMap;
	uint32_t m_MapWidth, m_MapHeight;
};
