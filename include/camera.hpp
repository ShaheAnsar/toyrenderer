#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <common.hpp>
#include <uniformbuffer.hpp>

namespace Rend{
  struct CameraUBO{
    glm::vec4 position;
    glm::vec4 up;
    glm::vec4 dir;
  };
  struct DebugCamera{
    UniformBuffer<CameraUBO> ubo;
    float fov;
    float near_plane;
    float far_plane;
    float aspect_ratio;
    DebugCamera(float fov = 60.0f, float aspect_ratio = 16.0f/9.0f,
		float near_plane = 0.1f, float far_plane = 100000.f);
    void tick();
    glm::mat4 getViewM();
    glm::mat4 getPerspectiveM();
  };
}
