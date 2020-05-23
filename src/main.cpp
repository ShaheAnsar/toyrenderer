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
#include <chrono>
#include <thread>

#include <unistd.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <../include/tiny_obj_loader.h>
#undef TINYOBJLOADER_IMPLEMENTATION
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
#include <common.hpp>
#include <shader.hpp>
#include <engine2D.hpp>
#include <texture.hpp>
#include <mesh.hpp>
#include <meshinstance.hpp>
#include <primitivegen.hpp>
#include <uniformbuffer.hpp>
#include <camera.hpp>



#define WIDTH 1280
#define HEIGHT 720

GLFWwindow *win = nullptr;
std::fstream glog("g.log", std::ios::out | std::ios::trunc);
std::fstream flog("f.log", flog.trunc | flog.out);


std::vector<float> quad{
			//Pos, UV
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
};

std::vector<float> quad2{
			 -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

std::vector<float> skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

const engine_globals eGlobals = {
			   WIDTH, HEIGHT
};




std::string to_string(const glm::vec3& a) {
  return std::string("<") + std::to_string(a.x) + "," + std::to_string(a.y) + "," + std::to_string(a.z) + ">"; 
}

std::string to_string(const glm::vec4& a) {
  return std::string("<") + std::to_string(a.x) + "," + std::to_string(a.y) + "," + std::to_string(a.z) +
    "," + std::to_string(a.w) + ">"; 
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
  flog.rdbuf()->pubsetbuf(0,0);
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
  glfwSwapInterval(1);
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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LEQUAL);
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

  Rend::UniformBuffer<engine_gl_ubo> engineGlUBO;
  engineGlUBO.buffer.time = 0;
  engineGlUBO.buffer.dt = 0;
  uniform_buffer_3d sponza_ubo_data = {
				       glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f,
							10000.0f)*
				       glm::lookAt(glm::vec3{1000.0f, 1000.0f, 1000.0f},
						   glm::vec3{0.0f, 0.0f, 0.0f},
						   glm::vec3{0.0f, 1.0f, 0.0f}),
				       glm::mat4(1.0f),
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
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
		    m.ambient_t = std::make_optional(Rend::Texture("assets/sponza/" + tex_name, GL_RGBA));
		  }
		  else {
		    m.ambient_t = std::nullopt;
		  }
		  if(diffuse_exists) {
		    std::string tex_name = m_t.diffuse_texname;
		    std::replace(tex_name.begin(), tex_name.end(), '\\', '/');
		    m.diffuse_t = std::make_optional(Rend::Texture("assets/sponza/" + tex_name, GL_RGBA));
		  }
		  else {
		    m.diffuse_t = std::nullopt;
		  }

		  if(specular_exists) {
		    std::string tex_name = m_t.specular_texname;
		    std::replace(tex_name.begin(), tex_name.end(), '\\', '/');
		    m.specular_t = std::make_optional(Rend::Texture("assets/sponza/" + tex_name, GL_RGBA));
		  }
		  else {
		    m.specular_t = std::nullopt;
		  }

		};
  Rend::Mesh<basic_mat, void (*)(basic_mat&, tinyobj::material_t&)>
    sponza_test("assets/sponza/sponza.obj", "assets/sponza/", fn_mat);
  flog << "Regarding Sponza:\n"
       << "Materials: " << sponza_test.mats.size() << std::endl;
  for(const auto& i : sponza_test.mats) {
    flog << "Material Info =>\n"
	 << "Ambient Color: " << to_string(i.mat_ubo.ambient_c) << "\n" 
	 << "Diffuse Color: " << to_string(i.mat_ubo.diffuse_c) << "\n"
	 << "Specular Color: " << to_string(i.mat_ubo.specular_c) << "\n"
	 << "Ambient Texture: " << std::to_string(i.ambient_t.has_value()) << "\n"
	 << "Diffuse Texture: " << std::to_string(i.diffuse_t.has_value()) << "\n"
	 << "Specular Texture: " << std::to_string(i.specular_t.has_value()) << std::endl;
  }
  for(const auto&[mat_id, i, len] : sponza_test.mat_tuples) {
    flog << "------\n"
	 << "MAT ID: " << mat_id << "\n"
	 << "I: " << i << "\n"
	 << "LEN: " << len << "\n"
	 << "------" << std::endl;
  }
  Rend::MeshInstance<basic_mat, void(*)(basic_mat&, tinyobj::material_t&)> sponza_inst;
  sponza_inst.mesh_ptr = &sponza_test;
  Rend::Mesh<basic_mat, void (*)(basic_mat&, tinyobj::material_t&)>
    buddha("assets/buddha/buddha.obj", "assets/buddha/", fn_mat);
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
  Rend::UniformBuffer buddha_ubo(uniform_buffer_3d{
    });
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
  
  #define SPONZA_FB 0
  #define BUDDHA_FB 1
  GLuint fbs[2];
  glGenFramebuffers(2, fbs);
  flog << "Framebuffers: " << fbs[0] << ", " << fbs[1] << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER,fbs[SPONZA_FB]);
  GLuint fb_texs[4];
  glGenTextures(4, fb_texs);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fb_texs[2*SPONZA_FB]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, fb_texs[2*SPONZA_FB + 1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, fb_texs[2*BUDDHA_FB]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, fb_texs[2*BUDDHA_FB + 1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  GLuint buddha_normal_tex;
  glGenTextures(1, &buddha_normal_tex);
  glBindTexture(GL_TEXTURE_2D, buddha_normal_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE , nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_texs[2*SPONZA_FB],0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb_texs[2*SPONZA_FB + 1],0);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    glog << "[ERROR] Sponza framebuffer incomplete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, fbs[BUDDHA_FB]);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_texs[2*BUDDHA_FB],0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, buddha_normal_tex, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb_texs[2*BUDDHA_FB + 1],0);
  std::vector<GLenum> draw_buffers = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(draw_buffers.size(), draw_buffers.data());
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    glog << "[ERROR] Buddha framebuffer incomplete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLuint quad_vao;
  GLuint quad_vbo;
  glGenVertexArrays(1, &quad_vao);
  glGenBuffers(1, &quad_vbo);
  glBindVertexArray(quad_vao);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(float),
	       quad.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)( sizeof(float)*2 ));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
  Rend::Shader quad_v("shaders/quad_sponza_buddha_v.spv", GL_VERTEX_SHADER);
  Rend::Shader quad_f("shaders/quad_sponza_buddha_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram quad_prog({quad_v, quad_f});

  GLuint skybox;
  glGenTextures(1, &skybox);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
  std::string skybox_paths[6] = {
				 "assets/skybox/""right"".jpg",
				 "assets/skybox/""left"".jpg",
				 "assets/skybox/""top"".jpg",
				 "assets/skybox/""bottom"".jpg",
				 "assets/skybox/""front"".jpg",
				 "assets/skybox/""back"".jpg"
  };
  stbi_set_flip_vertically_on_load(false);
  for(std::size_t i = 0; i < 6; i++) {
    int tw = 0, th = 0, tn = 0;
    unsigned char* data = stbi_load(skybox_paths[i].c_str(), &tw, &th, &tn, 0);
    if(!data) {
      flog << "Unable to load in texture: " << skybox_paths[i] << std::endl;
    }
    GLenum imageformat = GL_RGB;
    if(tn > 3)
      imageformat = GL_RGBA;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, tw, th, 0, imageformat,
		 GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
  stbi_set_flip_vertically_on_load(true);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  GLuint skybox_vao;
  GLuint skybox_vbo;
  glGenVertexArrays(1, &skybox_vao);
  glGenBuffers(1, &skybox_vbo);
  glBindVertexArray(skybox_vao);
  glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
  glBufferData(GL_ARRAY_BUFFER, (skyboxVertices.size() * sizeof(float)), skyboxVertices.data(),
	       GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  Rend::Shader skybox_vert("shaders/skybox_v.spv", GL_VERTEX_SHADER);
  Rend::Shader skybox_frag("shaders/skybox_f.spv", GL_FRAGMENT_SHADER);
  Rend::ShaderProgram skybox_prog({skybox_vert, skybox_frag});
  GLuint skybox_ubo;
  glGenBuffers(1, &skybox_ubo);
  uniform_buffer_3d skybox_ubo_data;
  skybox_ubo_data = {
		     glm::mat4(1.0f),
		     glm::mat4(1.0f),
		     glm::mat4(1.0f),
		     glm::vec4(0.0f)
  };
  

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(win, true);
  ImGui_ImplOpenGL3_Init("#version 150");

  

  Rend::DebugCamera mainDCamera;

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
  dlight.color = glm::vec4{0.0f};
  dlight.direction_intensity = glm::vec4{0.0f};
  GLuint dlight_ubo;
  glGenBuffers(1, &dlight_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, dlight_ubo);
  std::pair<double, double> mouse_posp;
  std::pair<double, double> mouse_posc;
  float mouse_speed = 0.5f;
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
    ImGui::InputFloat("Camera FOV", &mainDCamera.fov);
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
    ImGui::InputFloat3("Sponza Position", &sponza_inst.position[0]);
    ImGui::InputFloat3("Sponza Scale", &sponza_inst.scale[0]);
    ImGui::InputFloat4("Sponza Rotation [u, vhat]", &sponza_inst.rotation[0]);
    bool reload_buddha_shaders = ImGui::Button("Reload Buddha Shader");
    if(reload_buddha_shaders) {
      buddha_prog.reload();
    }
    if(ImGui::Button("Reload Sponza Buddha FB Shader")) {
      quad_prog.reload();
    }

    perspectiveM = mainDCamera.getPerspectiveM();
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
      vertical_angle = std::clamp(vertical_angle, glm::radians(-70.0f), glm::radians(70.0f));
    }
    mainDCamera.ubo.buffer.dir = glm::vec4{std::cos(vertical_angle)*std::sin(horizontal_angle),
					   std::sin(vertical_angle),
					   -std::cos(vertical_angle)*std::cos(horizontal_angle), 0.0f};

    if(Q_KEY_pressed == GLFW_PRESS) {
      mainDCamera.ubo.buffer.position += movement_speed * dt * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    }
    if(E_KEY_pressed == GLFW_PRESS) {
      mainDCamera.ubo.buffer.position -= movement_speed * dt * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    }
    if(W_KEY_pressed == GLFW_PRESS) {
      mainDCamera.ubo.buffer.position += movement_speed * dt * mainDCamera.ubo.buffer.dir;
    }
    if(S_KEY_pressed == GLFW_PRESS) {
      mainDCamera.ubo.buffer.position -= movement_speed * dt * mainDCamera.ubo.buffer.dir;
    }
    if(A_KEY_pressed == GLFW_PRESS) {
      mainDCamera.ubo.buffer.position -= movement_speed * dt *
	glm::vec4( glm::cross(glm::vec3( mainDCamera.ubo.buffer.dir ),
			      glm::vec3{0.0f, 1.0f, 0.0f}), 1.0f );
      //stub
    }
    if(D_KEY_pressed == GLFW_PRESS) {
      //mainDCamera.ubo.buffer.position camera_pos += movement_speed * dt * glm::cross(camera_dir, glm::vec3{0.0f, 1.0f, 0.0f});
      mainDCamera.ubo.buffer.position += movement_speed * dt *
	glm::vec4( glm::cross(glm::vec3( mainDCamera.ubo.buffer.dir ),
			      glm::vec3{0.0f, 1.0f, 0.0f}), 1.0f );

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
    ImGui::Text("Camera Position: <%f,%f,%f>", mainDCamera.ubo.buffer.position.x,
		mainDCamera.ubo.buffer.position.y,
		mainDCamera.ubo.buffer.position.z);
    ImGui::Text("Frametime: %f ms", dt*1000);
    ImGui::Text("FPS: %f", 1/dt);
    ImGui::EndChild();
    ImGui::End();
    glm::mat4 viewM = mainDCamera.getViewM();
    buddha_modelM = glm::translate(glm::mat4(1.0f), buddha_pos);
    buddha_modelM *= glm::scale(glm::mat4(1.0f), buddha_scale);
    //sponza_ubo_data.mvp = perspectiveM * viewM;
    uniform_buffer_3d& buddha_ubo_data = buddha_ubo.buffer;
    buddha_ubo_data.mvp = perspectiveM * viewM * buddha_modelM;
    buddha_ubo_data.model = buddha_modelM;
    buddha_ubo_data.normalM = glm::transpose(glm::inverse(buddha_modelM));
    buddha_ubo_data.texcoord_offset = glm::vec4(0.0f);
    glm::mat4 viewM_skybox = glm::mat4(glm::mat3(viewM));
    skybox_ubo_data.mvp = perspectiveM * viewM_skybox;
    skybox_ubo_data.normalM = viewM_skybox;
    glBindBuffer(GL_UNIFORM_BUFFER, sponza_ubo);
    //glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_3d), &sponza_ubo_data, GL_STREAM_DRAW);
    //camera_ubo_data = glm::vec4(camera_pos, 1.0f);
    //glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    //glBufferData(GL_UNIFORM_BUFFER, sizeof(camera_ubo_data), &camera_ubo_data, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, debug_ubo_name);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(debug_ubo), &debug_ubo_1, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, dlight_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dlight), &dlight, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, sponza_light_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(sponza_light), &sponza_light, GL_STREAM_DRAW);
    buddha_ubo.upload();
    glBindBuffer(GL_UNIFORM_BUFFER, buddha_outline_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), &buddha_outline_col, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, skybox_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_3d), &skybox_ubo_data,
		 GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    engineGlUBO.upload();
    engineGlUBO.bind(20);
    mainDCamera.tick();
    mainDCamera.ubo.bind(2);
    sponza_inst.tick(perspectiveM, viewM);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbs[SPONZA_FB]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbs[BUDDHA_FB]);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, fbs[SPONZA_FB]);
    sponza_prog.use_program();
    sponza_inst.ubo.bind(5);
    glBindVertexArray(sponza_test.vao);
    for(auto&[mat_i,i,len] : sponza_test.mat_tuples) {
      if(sponza_test.mats[mat_i].ambient_t.has_value())
	sponza_test.mats[mat_i].ambient_t.value().bind(0);
      if(sponza_test.mats[mat_i].diffuse_t.has_value())
	sponza_test.mats[mat_i].diffuse_t.value().bind(1);
      if(sponza_test.mats[mat_i].specular_t.has_value())
	sponza_test.mats[mat_i].specular_t.value().bind(2);
      cel_shade.bind(3);
      glBindBufferBase(GL_UNIFORM_BUFFER, 1, sponza_test.mats[mat_i].ubo);
      glDrawArrays(GL_TRIANGLES, 3*i, len*3);
    }
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
    glBindVertexArray(skybox_vao);
    glBindBufferBase(GL_UNIFORM_BUFFER, 5, skybox_ubo);
    skybox_prog.use_program();
    glDrawArrays(GL_TRIANGLES, 0, skyboxVertices.size() / 3);

    glBindFramebuffer(GL_FRAMEBUFFER, fbs[BUDDHA_FB]);
    //glBindBufferBase(GL_UNIFORM_BUFFER, 5, buddha_ubo);
    buddha_ubo.bind(5);
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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb_texs[2*SPONZA_FB]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fb_texs[2*SPONZA_FB + 1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fb_texs[2*BUDDHA_FB]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, fb_texs[2*BUDDHA_FB + 1]);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, buddha_normal_tex);
    glBindVertexArray(quad_vao);
    quad_prog.use_program();
    glDrawArrays(GL_TRIANGLES, 0, quad.size()/4);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwPollEvents();
    glfwSwapBuffers(win);

    dt = glfwGetTime() - time;
    time += dt;
    engineGlUBO.buffer.dt = dt;
    engineGlUBO.buffer.time = time;
    
    //if(dt < 0.0166) {
    //  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>( std::floor(16.6 - 1000*dt) )));
    //}
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
  glog.close();
  return 0;
}
