#include "LuckyPch.h"
#include "CameraController.h"

#include "OrthographicCameraController.h"
#include "PerspectiveCameraController.h"

namespace Lucky
{
	Scope<CameraController> CameraController::Create(CameraType type, const CameraSettings &settings)
	{
		switch (type)
		{
		case CameraType::Orthographic:
			return std::make_unique<OrthographicCameraController>(settings);

		case CameraType::Perspective:
			return std::make_unique<PerspectiveCameraController>(settings);
		}

		LK_CORE_ASSERT(false, "Unknown camera type!");
		return nullptr;
	}

} // namespace Lucky