#include "LuckyPch.h"
#include "Renderer.h"

#include "RenderCommand.h"
#include "Renderer2D.h"

namespace Lucky
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		LK_PROFILE_FUNCTION();
		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const Scope<CameraController>& cameraController)
	{
		s_SceneData->ViewProjectionMatrix = cameraController->GetCamera().GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
} // namespace Lucky
