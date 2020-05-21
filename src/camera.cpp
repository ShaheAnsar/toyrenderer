
#include <camera.hpp>


#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>


using namespace Rend;

DebugCamera::DebugCamera(float fov, float aspect_ratio,
		float near_plane, float far_plane) :
  ubo(), fov(fov), near_plane(near_plane), far_plane(far_plane), aspect_ratio(aspect_ratio)
{
  ubo.buffer.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  ubo.buffer.dir = glm::vec4(1.0f,0.0f, 0.0f, 0.0f);
  ubo.buffer.up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}


void DebugCamera::tick() {
  ubo.upload();
}


glm::mat4 DebugCamera::getViewM() {
  return glm::lookAt(glm::vec3( ubo.buffer.position ),
		     glm::vec3( ubo.buffer.position + ubo.buffer.dir ),
		     glm::vec3( ubo.buffer.up ));
}

glm::mat4 DebugCamera::getPerspectiveM() {
  return glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
}
