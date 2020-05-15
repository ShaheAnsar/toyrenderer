#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <thread>
#include <chrono>
#include <memory>
#include <fstream>
#include <optional>

#include <unistd.h>

#undef TINYOBJLOADER_IMPLEMENTATION
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
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include <logger.hpp>
#include <shader.hpp>
#include <engine2D.hpp>
#include <texture.hpp>
#include <mesh.hpp>

#define HEIGHT 720
#define WIDTH 1280

GLFWwindow *win = nullptr;
std::fstream glog("g.log", std::ios::out | std::ios::trunc);
std::fstream flog("f.log", flog.trunc | flog.out);

struct uniform_buffer_3d {
  glm::mat4 mvp;
  glm::mat4 model;
  glm::vec4 texcoord_offset;
  glm::mat4 normalM;
};

struct point_light {
  glm::vec4 position_intensity;
  glm::vec4 color;
  glm::vec4 falloff_constants;
};

struct directional_light {
  glm::vec4 direction_intensity;
  glm::vec4 color;
};
struct debug_ubo {
  glm::vec4 adsn;
};


struct basic_mat_ubo {
  glm::vec4 ambient_c;
  glm::vec4 diffuse_c;
  glm::vec4 specular_c;
};

struct basic_mat {
  GLuint ubo;
  basic_mat_ubo mat_ubo;
  std::optional<Rend::Texture> ambient_t;
  std::optional<Rend::Texture> diffuse_t;
  std::optional<Rend::Texture> specular_t;
};

std::string to_string(const glm::vec3& a) {
  return std::string("<") + std::to_string(a.x) + "," + std::to_string(a.y) + "," + std::to_string(a.z) + ">"; 
}


logger::logger mlog(std::cout);

typedef void  (APIENTRY *DEBUGCALLBACK)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void  APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			     GLsizei length, const GLchar* message, const void* userParam)
{
  //mlog << std::make_pair<logger::pri, std::string>( logger::pri::DEB, std::string("<From OpenGL> ") + message );
  glog << std::string("<From OpenGL> ") + message << std::endl;

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
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if(!glfwRawMouseMotionSupported())
    mlog << std::make_pair<logger::pri, std::string>(logger::pri::ERR,
						     std::string("Raw Mouse Motion Not Supported!"));
  glfwSetInputMode(win, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  
  mlog << "Initialized Window" << "Initializing GLAD";
  glfwMakeContextCurrent(win);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Unable to initialize glad!";
    std::exit(-1);
  }
  mlog << "Initialized GLAD";
  glDebugMessageCallback(debugCallback, nullptr);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEBUG_OUTPUT);
  glViewport(0, 0, WIDTH, HEIGHT);
 int tw = 0, th = 0, tn = 0;
  unsigned char* td = stbi_load("assets/woodfloor.png", &tw, &th, &tn, 0);
  if(!td) mlog << std::pair<logger::pri, std::string>{logger::pri::ERR, "Unable to load in texture"};
  GLuint image_format = GL_RGB; if(tn > 3) image_format = GL_RGBA;
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
  Rend::Texture t("./assets/woodfloor.png");
  t.bind(3);
  program3d.use_program();
  uniform_buffer_3d sponza_ubo_data = {
				       glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f,
							10000.0f)*
				       glm::lookAt(glm::vec3{1000.0f, 1000.0f, 1000.0f},
						   glm::vec3{0.0f, 0.0f, 0.0f},
						   glm::vec3{0.0f, 1.0f, 0.0f}),
				       glm::mat4(1.0f),
				       glm::vec4(0.0f)
  };
  point_light sponza_light = {
			      glm::vec4{0.0f, 20.0f, 0.0f, 10000.0f},
			      glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
			      glm::vec4{0.0000001f, 0.00001f, 0.3f, 0.0f}
  };

  Rend::Shader sponza_v("./shaders/sponza_v.spv", GL_VERTEX_SHADER);
  Rend::Shader sponza_f("./shaders/sponza_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram sponza_prog({sponza_v, sponza_f});
  sponza_prog.use_program();

  GLuint sponza_ubo;
  GLuint sponza_light_ubo;
  glGenBuffers(1, &sponza_ubo);
  glGenBuffers(1, &sponza_light_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, sponza_ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_3d), &sponza_ubo_data, GL_STREAM_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, sponza_light_ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(point_light), &sponza_light, GL_STREAM_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, sponza_light_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 5, sponza_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, sponza_ubo);

  auto fn_mat = [](basic_mat& m, tinyobj::material_t& m_t) -> void {
		  bool ambient_exists = !m_t.ambient_texname.empty();
		  bool diffuse_exists = !m_t.diffuse_texname.empty();
		  bool specular_exists = !m_t.specular_texname.empty();
		  float ambient_existsf = ambient_exists ? 1.0f : 0.0f;
		  float diffuse_existsf = diffuse_exists ? 1.0f : 0.0f;
		  float specular_existsf = specular_exists ? 1.0f : 0.0f;
		  m.mat_ubo.ambient_c = {m_t.ambient[0],m_t.ambient[1],m_t.ambient[2], ambient_existsf};
		  m.mat_ubo.diffuse_c = {m_t.diffuse[0],m_t.diffuse[1],m_t.diffuse[2], diffuse_existsf};
		  m.mat_ubo.specular_c = {m_t.specular[0],m_t.specular[1],m_t.specular[2], specular_existsf};
		  glGenBuffers(1, &m.ubo);
		  glBindBuffer(GL_UNIFORM_BUFFER, m.ubo);
		  glBufferData(GL_UNIFORM_BUFFER, sizeof(m.mat_ubo), &m.mat_ubo, GL_STREAM_DRAW);
		  glBindBuffer(GL_UNIFORM_BUFFER, 0);
		  if(ambient_exists) {
		    std::string tex_name = m_t.ambient_texname;
		    std::replace(tex_name.begin(), tex_name.end(), '\\', '/');
		    m.ambient_t = std::make_optional(Rend::Texture("assets/sponza/" + tex_name));
		  }
		  else {
		    m.ambient_t = std::nullopt;
		  }
		  if(diffuse_exists) {
		    std::string tex_name = m_t.diffuse_texname;
		    std::replace(tex_name.begin(), tex_name.end(), '\\', '/');
		    m.diffuse_t = std::make_optional(Rend::Texture("assets/sponza/" + tex_name));
		  }
		  else {
		    m.diffuse_t = std::nullopt;
		  }

		  if(specular_exists) {
		    std::string tex_name = m_t.specular_texname;
		    std::replace(tex_name.begin(), tex_name.end(), '\\', '/');
		    m.specular_t = std::make_optional(Rend::Texture("assets/sponza/" + tex_name));
		  }
		  else {
		    m.specular_t = std::nullopt;
		  }

		};
  auto fn_null = []() -> void {};
  Rend::Mesh<basic_mat, void (*)(basic_mat&, tinyobj::material_t&), void(*)()>
    sponza_test("assets/sponza/sponza.obj", "assets/sponza/", fn_null, fn_mat);
  Rend::Mesh<basic_mat, void (*)(basic_mat&, tinyobj::material_t&), void(*)()>
    buddha("assets/buddha/buddha.obj", "assets/buddha/", fn_null, fn_mat);
  Rend::Texture cel_shade("assets/LUTS/cel_shade.png");
  cel_shade.bind(0);
  cel_shade.set_param(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  cel_shade.set_param(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  cel_shade.set_param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  cel_shade.set_param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  Rend::Shader buddha_v("shaders/buddha_v.spv", GL_VERTEX_SHADER);
  Rend::Shader buddha_f("shaders/buddha_f.spv", GL_FRAGMENT_SHADER);
  Rend::Shader buddha_outline_f("shaders/outline_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram buddha_outline_prog({buddha_v, buddha_outline_f});
  Rend::ShaderProgram buddha_prog( {buddha_v, buddha_f} );
  glBindVertexArray(buddha.vao);
  glBindBuffer(GL_ARRAY_BUFFER, buddha.vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float)*3));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  uniform_buffer_3d buddha_ubo_data{
			       
  };
  GLuint buddha_ubo;
  glGenBuffers(1, &buddha_ubo);
  sponza_prog.use_program();
  glBindVertexArray(sponza_test.vao);
  glBindBuffer(GL_ARRAY_BUFFER, sponza_test.vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(sizeof(float)*3));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(sizeof(float)*6));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(win, true);
  ImGui_ImplOpenGL3_Init("#version 150");

  

  float time = glfwGetTime();
  float dt = 0;
  glm::mat4 perspectiveM = glm::perspective(glm::radians(60.0f), 16.0f/9.0f, 0.1f,
					    10000.0f);
  glm::mat4 buddha_modelM = glm::mat4(1.0f);
  glm::vec3 buddha_pos = glm::vec3(0.0f, 100.0f, 0.0f);
  glm::vec3 buddha_scale = glm::vec3(100.0f);
  glm::vec4 buddha_outline_col = glm::vec4(1.0f);
  GLuint buddha_outline_ubo;
  glGenBuffers(1, &buddha_outline_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 6, buddha_outline_ubo);
  glm::vec3 camera_pos{136.f, 150.f, -8.f};
  glm::vec4 camera_ubo_data {camera_pos, 1.0f};
  GLuint camera_ubo;
  glGenBuffers(1, &camera_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, camera_ubo);
  GLuint debug_ubo_name;
  debug_ubo debug_ubo_1;
  glGenBuffers(1, &debug_ubo_name);
  glBindBufferBase(GL_UNIFORM_BUFFER, 3, debug_ubo_name);
  directional_light dlight;
  GLuint dlight_ubo;
  glGenBuffers(1, &dlight_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, dlight_ubo);
  std::pair<double, double> mouse_posp;
  std::pair<double, double> mouse_posc;
  float mouse_speed = 3.0f;
  float horizontal_angle = 0.f;
  float vertical_angle = 0.f;
  float camera_fov = 60.0f;
  bool init = true;
  bool raw_motion_disabled = false;
  float movement_speed = 200.0f;
  int E_KEY_pressed = GLFW_RELEASE;
  int Q_KEY_pressed = GLFW_RELEASE;
  int W_KEY_pressed = GLFW_RELEASE;
  int A_KEY_pressed = GLFW_RELEASE;
  int S_KEY_pressed = GLFW_RELEASE;
  int D_KEY_pressed = GLFW_RELEASE;
  int R_KEY_pressed = GLFW_RELEASE;
  float R_KEY_timer = 0.0f;
  glBindVertexArray(sponza_test.vao);
  while (!glfwWindowShouldClose(win)) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Debug");
    ImGui::InputFloat("Movement Speed", &movement_speed);
    ImGui::InputFloat("Camera FOV", &camera_fov);
    ImGui::InputFloat("Ambient Scaler", &debug_ubo_1.adsn[0]);
    ImGui::InputFloat("Diffuse Scaler", &debug_ubo_1.adsn[1]);
    ImGui::InputFloat("Specular Scaler", &debug_ubo_1.adsn[2]);
    ImGui::InputFloat3("Point Light Falloff Constants[Q,L,C]", &sponza_light.falloff_constants[0]);
    ImGui::ColorEdit3("Directional Light color", &dlight.color.r);
    ImGui::InputFloat3("Directional Light direction", &dlight.direction_intensity.x);
    ImGui::InputFloat("Directional Light Intensity", &dlight.direction_intensity.w);
    ImGui::InputFloat3("Buddha Position", &buddha_pos[0]);
    ImGui::InputFloat3("Buddha Scale", &buddha_scale[0]);
    ImGui::ColorEdit3("Buddha Outline Color", &buddha_outline_col[0]);
      
    perspectiveM = glm::perspective(glm::radians(camera_fov), 16.0f/9.0f, 0.1f, 10000.f);
    if (init) {
      glfwGetCursorPos(win, &mouse_posp.first, &mouse_posp.second);
      init = false;
    }
    glfwGetCursorPos(win, &mouse_posc.first, &mouse_posc.second);
    Q_KEY_pressed = glfwGetKey(win, GLFW_KEY_Q);
    E_KEY_pressed = glfwGetKey(win, GLFW_KEY_E);
    W_KEY_pressed = glfwGetKey(win, GLFW_KEY_W);
    A_KEY_pressed = glfwGetKey(win, GLFW_KEY_A);
    S_KEY_pressed = glfwGetKey(win, GLFW_KEY_S);
    D_KEY_pressed = glfwGetKey(win, GLFW_KEY_D);
    R_KEY_pressed = glfwGetKey(win, GLFW_KEY_R);
    if(!raw_motion_disabled) {
      horizontal_angle += mouse_speed * dt * (mouse_posc.first - mouse_posp.first);
      vertical_angle += mouse_speed * dt * (mouse_posp.second - mouse_posc.second);
    }
    glm::vec3 camera_dir{std::cos(vertical_angle)*std::sin(horizontal_angle), std::sin(vertical_angle),
			 -std::cos(vertical_angle)*std::cos(horizontal_angle)};

    if(Q_KEY_pressed == GLFW_PRESS) {
      camera_pos += movement_speed * dt * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if(E_KEY_pressed == GLFW_PRESS) {
      camera_pos -= movement_speed * dt * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if(W_KEY_pressed == GLFW_PRESS) {
      camera_pos += movement_speed * dt * camera_dir;
    }
    if(S_KEY_pressed == GLFW_PRESS) {
      camera_pos -= movement_speed * dt * camera_dir;
    }
    if(A_KEY_pressed == GLFW_PRESS) {
      camera_pos -= movement_speed * dt * glm::cross(camera_dir, glm::vec3{0.0f, 1.0f, 0.0f});
      //stub
    }
    if(D_KEY_pressed == GLFW_PRESS) {
      camera_pos += movement_speed * dt * glm::cross(camera_dir, glm::vec3{0.0f, 1.0f, 0.0f});
      //stub
    }
    if(R_KEY_pressed == GLFW_PRESS) {
#define R_KEY_TIMER_THRESHOLD 0.5
      if(R_KEY_timer > R_KEY_TIMER_THRESHOLD)
      {
	R_KEY_timer = 0.0f;
	if (raw_motion_disabled) {
	  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	  raw_motion_disabled = false;
	} else {
	  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	  raw_motion_disabled = true;
        }
      }
    }

    ImGui::Text("Debug Output");
    ImGui::BeginChild("Scrolling");
    ImGui::Text("Horizontal Angle: %f", horizontal_angle);
    ImGui::Text("Vertical Angle: %f", vertical_angle);
    ImGui::Text("Camera Position: <%f,%f,%f>", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::Text("Frametime: %f ms", dt*1000);
    ImGui::Text("FPS: %f", 1/dt);
    ImGui::EndChild();
    ImGui::End();
    glBindBuffer(GL_UNIFORM_BUFFER, sponza_ubo);
    glm::mat4 viewM = glm::lookAt(camera_pos,
				  camera_pos + camera_dir,
				  glm::vec3(0.0f, 1.0f, 0.0f));
    buddha_modelM = glm::translate(glm::mat4(1.0f), buddha_pos);
    buddha_modelM *= glm::scale(glm::mat4(1.0f), buddha_scale);
    sponza_ubo_data.mvp = perspectiveM * viewM;
    buddha_ubo_data.mvp = perspectiveM * viewM * buddha_modelM;
    buddha_ubo_data.model = buddha_modelM;
    buddha_ubo_data.normalM = glm::transpose(glm::inverse(buddha_modelM));
    buddha_ubo_data.texcoord_offset = glm::vec4(0.0f);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_3d), &sponza_ubo_data, GL_STREAM_DRAW);
    camera_ubo_data = glm::vec4(camera_pos, 1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(camera_ubo_data), &camera_ubo_data, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, debug_ubo_name);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(debug_ubo), &debug_ubo_1, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, dlight_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dlight), &dlight, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, sponza_light_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(sponza_light), &sponza_light, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, buddha_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_3d), &buddha_ubo_data, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, buddha_outline_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), &buddha_outline_col, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    sponza_prog.use_program();
    glBindBufferBase(GL_UNIFORM_BUFFER, 5, sponza_ubo);
    glBindVertexArray(sponza_test.vao);
    for(auto&[mat_i,i,len] : sponza_test.mat_tuples) {
      if(sponza_test.mats[mat_i].ambient_t.has_value())
	sponza_test.mats[mat_i].ambient_t.value().bind(0);
      if(sponza_test.mats[mat_i].diffuse_t.has_value())
	sponza_test.mats[mat_i].diffuse_t.value().bind(1);
      if(sponza_test.mats[mat_i].specular_t.has_value())
	sponza_test.mats[mat_i].specular_t.value().bind(2);
      cel_shade.bind(6);
      glBindBufferBase(GL_UNIFORM_BUFFER, 1, sponza_test.mats[mat_i].ubo);
      glDrawArrays(GL_TRIANGLES, 3*i, len*3);
    }
    glStencilMask(0xff);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glBindBufferBase(GL_UNIFORM_BUFFER, 5, buddha_ubo);
    glBindVertexArray(buddha.vao);
    buddha_prog.use_program();
    for(auto&[mat_i,i,len] : buddha.mat_tuples) {
      if(buddha.mats.empty()) goto buddha_matskip;
      if(buddha.mats[mat_i].ambient_t.has_value())
      	buddha.mats[mat_i].ambient_t.value().bind(0);
      if(buddha.mats[mat_i].diffuse_t.has_value())
      	buddha.mats[mat_i].diffuse_t.value().bind(1);
      if(buddha.mats[mat_i].specular_t.has_value())
      	buddha.mats[mat_i].specular_t.value().bind(2);
      glBindBufferBase(GL_UNIFORM_BUFFER, 1, buddha.mats[mat_i].ubo);
    buddha_matskip:
      cel_shade.bind(0);
      glDrawArrays(GL_TRIANGLES, 3*i, len*3);
    }
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    buddha_modelM = glm::translate(glm::mat4(1.0f), buddha_pos);
    buddha_modelM *= glm::scale(glm::mat4(1.0f), buddha_scale * 1.05f);
    buddha_ubo_data.mvp = perspectiveM * viewM * buddha_modelM;
    buddha_ubo_data.model = buddha_modelM;
    buddha_ubo_data.normalM = glm::mat4(1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, buddha_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_3d),
		 &buddha_ubo_data, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, buddha_outline_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buddha_outline_col),
		 &buddha_outline_col, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    buddha_outline_prog.use_program();
    for(auto&[mat_i,i,len] : buddha.mat_tuples) {
      if(buddha.mats.empty()) goto buddha_matskip2;
      if(buddha.mats[mat_i].ambient_t.has_value())
      	buddha.mats[mat_i].ambient_t.value().bind(0);
      if(buddha.mats[mat_i].diffuse_t.has_value())
      	buddha.mats[mat_i].diffuse_t.value().bind(1);
      if(buddha.mats[mat_i].specular_t.has_value())
      	buddha.mats[mat_i].specular_t.value().bind(2);
      glBindBufferBase(GL_UNIFORM_BUFFER, 1, buddha.mats[mat_i].ubo);
    buddha_matskip2:
      cel_shade.bind(0);
      glDrawArrays(GL_TRIANGLES, 3*i, len*3);
    }
    glStencilFunc(GL_ALWAYS, 0x00, 0x00);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwPollEvents();
    glfwSwapBuffers(win);

    dt = glfwGetTime() - time;
    time += dt;
    R_KEY_timer += dt;
    mouse_posp = mouse_posc;

    E_KEY_pressed = GLFW_RELEASE;
    Q_KEY_pressed = GLFW_RELEASE;
    W_KEY_pressed = GLFW_RELEASE;
    A_KEY_pressed = GLFW_RELEASE;
    S_KEY_pressed = GLFW_RELEASE;
    D_KEY_pressed = GLFW_RELEASE;

  }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    flog.close();
    return 0;
}
