#include "ipch.h"
#include "EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace IlluminoEngine
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_Projection(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
	}

	void EditorCamera::OnUpdate(Timestep timestep)
	{
		OPTICK_EVENT();

		float cosYaw = glm::cos(glm::radians(m_Yaw));
		float sinYaw = glm::sin(glm::radians(m_Yaw));
		float cosPitch = glm::cos(glm::radians(m_Pitch));
		float sinPitch = glm::sin(glm::radians(m_Pitch));

		m_Forward.x = cosYaw * cosPitch;
		m_Forward.y = sinPitch;
		m_Forward.z = sinYaw * cosPitch;

		m_Forward = glm::normalize(m_Forward);
		m_Right = glm::normalize(glm::cross(m_Forward, {0, 1, 0}));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

		m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	void EditorCamera::SetViewportSize(float width, float height)
	{
		OPTICK_EVENT();

		m_AspectRatio = width / height;
		m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	}
}
