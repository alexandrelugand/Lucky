#include "LuckyPch.h"
#include "CameraController.h"

#include "OrthographicCameraController.h"
#include "PerspectiveCameraController.h"

namespace Lucky
{
	Scope<CameraController> CameraController::Create(CameraType type, const Camera::Settings &settings)
	{
		switch (type)
		{
		case CameraType::Orthographic:
			return CreateScope<OrthographicCameraController>(settings);

		case CameraType::Perspective:
			return CreateScope<PerspectiveCameraController>(settings);
		}

		LK_CORE_ASSERT(false, "Unknown camera type!");
		return nullptr;
	}

} // namespace Lucky
