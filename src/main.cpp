#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>

#define TINY_OBJ_IMPLEMENTATION
#include <../include/tiny_obj_loader.h>
#include <../include/glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <../include/stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <logger.hpp>
#include <shader.hpp>
#include <engine2D.hpp>

#define HEIGHT 600
#define WIDTH 800

GLFWwindow *win = nullptr;

std::vector<float> tri1 = {
			   0.0f, 0.0f, 0.0f,
			   1.0f, 0.0f, 0.0f,
			   
			   0.5f, 0.0f, 0.0f,
			   0.0f, 1.0f, 0.0f,
			   
			   0.5f, 0.5f, 0.0f,
			   0.0f, 0.0f, 1.0f,

			   0.0f, 0.5f, 0.0f,
			   1.0f, 1.0f, 0.0f,
			   
};

std::vector<uint8_t> trie1 = {
			      0, 1, 2,
			      0, 2, 3
};

std::vector<float> tri2 = {
			   0.0f, 0.0f, 0.0f,
			   1.0f, 0.0f, 0.0f,
			   
			   -0.5f, 0.0f, 0.0f,
			   0.0f, 1.0f, 0.0f,
			   
			   -0.5f, -0.5f, 0.0f,
			   0.0f, 0.0f, 1.0f,

			   0.0f, -0.5f, 0.0f,
			   1.0f, 1.0f, 0.0f,
};

std::vector<uint8_t> trie2 = {
			      0, 1, 2,
			      0, 2, 3
};


std::vector<float> cube = {
			   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			   0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

struct uniform_buffer_3d {
  glm::mat4 mvp;
  glm::mat4 model;
  glm::vec4 texcoord_offset;
};

struct point_light {
  glm::vec4 position;
};

logger::logger mlog(std::cout);

typedef void  (APIENTRY *DEBUGCALLBACK)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void  APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			     GLsizei length, const GLchar* message, const void* userParam)
{
  mlog << std::make_pair<logger::pri, std::string>( logger::pri::DEB, std::string("<From OpenGL> ") + message );
}

int main(void) {
  stbi_set_flip_vertically_on_load(true);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  win = glfwCreateWindow(WIDTH, HEIGHT, "OGL", nullptr, nullptr);
  if (!win) {
    std::cerr << "Unable to initialize window";
    std::exit(-1);
  }
  mlog << "Initialized Window" << "Initializing GLAD";
  glfwMakeContextCurrent(win);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Unable to initialize glad!";
    std::exit(-1);
  }
  mlog << "Initialized GLAD";
  glDebugMessageCallback(debugCallback, nullptr);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEBUG_OUTPUT);
  glViewport(0, 0, WIDTH, HEIGHT);
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  GLuint uniform_buffer;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenBuffers(1, &uniform_buffer);
  GLuint vao2, vbo2, ebo2;
  glGenBuffers(1, &vbo2);
  glGenVertexArrays(1, &vao2);
  glGenBuffers(1, &ebo2);
  mlog << "Generated VAO, VBO and EBO";
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);
  glBufferData(GL_ARRAY_BUFFER, tri1.size()*sizeof(float), tri1.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, trie1.size()*sizeof(uint8_t), trie1.data(), GL_STATIC_DRAW);
  glm::vec4 offset_v4(-0.2f, -0.2f, 0.0f, 1.0f);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), &offset_v4, GL_STREAM_DRAW);
  mlog << "Uploaded data to the VBO and EBO";
  Rend::Shader vshader("./shaders/triangle_v.spv", GL_VERTEX_SHADER);
  Rend::Shader fshader("./shaders/triangle_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram triprog({vshader, fshader});
  triprog.use_program();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float)*3));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(vao2);
  Rend::Shader vshader2("./shaders/triangle2_v.spv", GL_VERTEX_SHADER);
  Rend::Shader fshader2("./shaders/triangle2_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram triprog2({vshader2, fshader2});
  triprog2.use_program();
  glBindBuffer(GL_ARRAY_BUFFER, vbo2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
  glBufferData(GL_ARRAY_BUFFER, tri2.size()*sizeof(float), tri2.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, trie2.size()*sizeof(*trie2.data()), trie2.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float)*3));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(vao);
  triprog.use_program();


  GLuint vbo3d, ebo3d, ubo3d, vao3d;
  glGenVertexArrays(1, &vao3d);
  glGenBuffers(1, &vbo3d);
  glGenBuffers(1, &ebo3d);
  glGenBuffers(1, &ubo3d);
  glBindVertexArray(vao3d);
  glBindBuffer(GL_ARRAY_BUFFER, vbo3d);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo3d);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo3d);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cube.size(), cube.data(), GL_STREAM_DRAW);
  uniform_buffer_3d a = {
			 glm::perspective(glm::radians(90.0f), 4.0f/3.0f, 0.1f, 100.f) * glm::lookAt(glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)),
			 glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)),
			 glm::vec4(0.0f),
  };
  glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_3d), &a, GL_STREAM_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(sizeof(float)*3));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);
  int tw = 0, th = 0, tn = 0;
  unsigned char* td = stbi_load("assets/woodfloor.png", &tw, &th, &tn, 0);
  if(!td) mlog << std::pair<logger::pri, std::string>{logger::pri::ERR, "Unable to load in texture"};
  GLuint image_format = GL_RGB;
  if(tn > 3) image_format = GL_RGBA;
  GLuint tex;
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, image_format, GL_UNSIGNED_BYTE, td);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(td);

  Rend::Shader shader3dv("shaders/3d_basic_v.spv", GL_VERTEX_SHADER);
  Rend::Shader shader3df("shaders/3d_basic_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram program3d({shader3dv, shader3df});
  program3d.use_program();
  glUniform1i(1, 0);
  // Engine2D Test
  engine2D::GameObject go(glm::vec2(0.0f));
  auto r_attr = std::make_shared<engine2D::Renderable>( glm::vec2(0.5f, 0.5f), glm::vec2(0.4f, 0.4f));
  Rend::Shader e2d_v("shaders/engine2d_basic_v.spv", GL_VERTEX_SHADER);
  Rend::Shader e2d_f("shaders/engine2d_basic_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram program2d({e2d_v, e2d_f});
  program2d.use_program();
  r_attr->set_shaderprog(program2d);
  r_attr->add_color(glm::vec4{1.0f, 1.0f, 0.0f, 1.0f});
  r_attr->submit_data();
  go.add_attribute(r_attr);

  float time = glfwGetTime();
  float dt = 0;
  while (!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    go.tick();
    ////glDrawArrays(GL_TRIANGLES, 0, 3);
    ////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
    //glDrawArrays(GL_TRIANGLES, 0, cube.size()/5);
    glfwPollEvents();
    glfwSwapBuffers(win);

    dt = glfwGetTime() - time;
    time += dt;
  }
  return 0;
}
