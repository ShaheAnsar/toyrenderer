#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <vector>
#include <string>

#define HEIGHT 800
#define WIDTH 600

std::vector<float> tri1 = {
};

GLFWwindow* win = nullptr;




int main(void) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  win = glfwCreateWindow(WIDTH, HEIGHT, "OGL", nullptr, nullptr);
  if(!win) {
    std::cerr << "Unable to initialize window";
    std::exit(-1);
  }
  glfwMakeContextCurrent(win);
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Unable to initialize glad!";
    std::exit(-1);
  }
  glViewport(0, 0, 800, 600);
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  while(!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();
    glfwSwapBuffers(win);
  }
  return 0;
}
