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

#define TINYOBJLOADER_IMPLEMENTATION
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

#define HEIGHT 720
#define WIDTH 1280

GLFWwindow *win = nullptr;
std::fstream glog("g.log", std::ios::out | std::ios::trunc);
std::fstream flog("f.log", flog.trunc | flog.out);

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

struct shape_mat_assoc {
  int material_id;
  std::size_t i; // Face index from which this association applies
  std::size_t length; // The length for which this association will be true
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
  mlog << "UBO3d : " << ubo3d;
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cube.size(), cube.data(), GL_STREAM_DRAW);
  uniform_buffer_3d a = {
			 glm::perspective(glm::radians(90.0f), 4.0f/3.0f, 0.1f, 100.f) *
			     glm::lookAt(glm::vec3{2.0f, 2.0f, 2.0f}, glm::vec3{0.0f, 0.0f, 0.0f},
					 glm::vec3{0.0f, 1.0f, 0.0f})
			     *glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)),
			 glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)),
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
  Rend::Texture t("./assets/woodfloor.png");
  t.bind(3);
  glBindVertexArray(vao3d);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo3d);
  program3d.use_program();
  char cwd[1024];
  getcwd(cwd, 1024);
  std::string cwd_str(cwd);
  //chdir("assets/sponza");
  tinyobj::attrib_t attrib_sponza;
  std::vector<tinyobj::shape_t> shapes_sponza;
  std::vector<tinyobj::material_t> materials_sponza;
  std::string warn_sponza;
  std::string err_sponza;
  if(!tinyobj::LoadObj(&attrib_sponza, &shapes_sponza,
		       &materials_sponza, &warn_sponza, &err_sponza, "assets/sponza/sponza.obj", "assets/sponza", true)) {
    mlog << std::make_pair<logger::pri, std::string>( logger::pri::ERR,
						      std::string(err_sponza.c_str()));
  }
  else if(!warn_sponza.empty()) {
    mlog << std::make_pair<logger::pri, std::string>(logger::pri::WARN,
						     std::string(warn_sponza.c_str()));
  }
  else if(!err_sponza.empty()) {
    mlog << std::make_pair<logger::pri, std::string>(logger::pri::ERR,
						     std::string(err_sponza.c_str()));
  }
  //chdir(cwd_str.c_str());
  flog << "Material Info" << std::endl;
  for(auto& i : materials_sponza) {
    flog  << "-----------------\n"
	  <<"Material Name: " << i.name << "\n"
	  <<"Ambient Color: <" << i.ambient[0] << "," << i.ambient[1] << "," << i.ambient[2] << ">\n"
	  <<"Diffuse Color: <" << i.diffuse[0] << "," << i.diffuse[1] << "," << i.diffuse[2] << ">\n"
	  <<"Specular Color: <" << i.specular[0] << "," << i.specular[1] << "," << i.specular[2] << ">\n"
	  <<"Ambient Texture: " << i.ambient_texname << "\n"
	  <<"Diffuse Texture: " << i.diffuse_texname << "\n"
	  <<"Specular Texture: " << i.specular_texname << std::endl;
  }
  mlog << "[MODEL] Shapes: " + std::to_string(shapes_sponza.size());
  mlog << "[MODEL] materials: " + std::to_string(materials_sponza.size());
  std::vector<GLuint> sponza_vbos(shapes_sponza.size());
  std::vector<GLuint> sponza_vaos(shapes_sponza.size());
  std::vector<std::vector<shape_mat_assoc>> sponza_vbo_materials;
  std::vector<basic_mat> processed_materials_sponza;
  glGenBuffers(shapes_sponza.size(), sponza_vbos.data());
  glGenVertexArrays(shapes_sponza.size(), sponza_vaos.data());
  for(std::size_t i = 0; i < sponza_vbos.size(); i++) {
    glBindVertexArray(sponza_vaos[i]);
    glBindBuffer(GL_ARRAY_BUFFER, sponza_vbos[i]);
    std::vector<float> vbo_data;
    for(std::size_t mesh_i = 0; mesh_i < shapes_sponza[i].mesh.indices.size(); mesh_i++) {
      tinyobj::index_t idx = shapes_sponza[i].mesh.indices[mesh_i];
      // Vertex Position
      vbo_data.push_back(attrib_sponza.vertices[idx.vertex_index*3 + 0]);
      vbo_data.push_back(attrib_sponza.vertices[idx.vertex_index*3 + 1]);
      vbo_data.push_back(attrib_sponza.vertices[idx.vertex_index*3 + 2]);
      // Vertex Normal
      vbo_data.push_back(attrib_sponza.normals[idx.normal_index*3 + 0]);
      vbo_data.push_back(attrib_sponza.normals[idx.normal_index*3 + 1]);
      vbo_data.push_back(attrib_sponza.normals[idx.normal_index*3 + 2]);
      // Vertex Texture Coordinates
      vbo_data.push_back(attrib_sponza.texcoords[idx.texcoord_index*2 + 0]);
      vbo_data.push_back(attrib_sponza.texcoords[idx.texcoord_index*2 + 1]);
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8*shapes_sponza[i].mesh.indices.size(),
		 vbo_data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    sponza_vbo_materials.push_back({});
    sponza_vbo_materials[i].push_back(shape_mat_assoc{shapes_sponza[i].mesh.material_ids[0], 0, 1});
   for(std::size_t face_i = 1; face_i < shapes_sponza[i].mesh.num_face_vertices.size(); face_i++) {
      if(shapes_sponza[i].mesh.material_ids[face_i] == sponza_vbo_materials[i].back().material_id) {
	sponza_vbo_materials[i].back().length++;
      }
      else {
	sponza_vbo_materials[i].push_back(shape_mat_assoc{shapes_sponza[i].mesh.material_ids[face_i], face_i, 1});
      }
    }
  }
  for(std::size_t i = 0; i < materials_sponza.size(); i++) {
    processed_materials_sponza.push_back({});
    std::string ambi_name = materials_sponza[i].ambient_texname;
    std::replace(ambi_name.begin(), ambi_name.end(), '\\', '/');
    ambi_name = "assets/sponza/" + ambi_name;
    std::string diff_name = materials_sponza[i].diffuse_texname;
    std::replace(diff_name.begin(), diff_name.end(), '\\', '/');
    diff_name = "assets/sponza/" + diff_name;
    std::string spec_name = materials_sponza[i].specular_texname;
    std::replace(spec_name.begin(), spec_name.end(), '\\', '/');
    spec_name = "assets/sponza/" + spec_name;
    flog << "Texture Name: " << ambi_name;
    flog << "Texture Name: " << diff_name;
    flog << "Texture Name: " << spec_name << std::endl;
    processed_materials_sponza.back().ambient_t = materials_sponza[i].ambient_texname.empty() ? std::nullopt :
      std::make_optional( Rend::Texture(ambi_name) );
    processed_materials_sponza.back().diffuse_t = materials_sponza[i].diffuse_texname.empty() ? std::nullopt :
      std::make_optional( Rend::Texture(diff_name) );
    processed_materials_sponza.back().specular_t = materials_sponza[i].specular_texname.empty() ? std::nullopt :
      std::make_optional( Rend::Texture(spec_name) );
    processed_materials_sponza.back().mat_ubo.ambient_c = glm::vec4{materials_sponza[i].ambient[0],
								    materials_sponza[i].ambient[1],
								    materials_sponza[i].ambient[2],
								    processed_materials_sponza.back().ambient_t.
								    has_value() ? 1.0f : 0.0f};
    processed_materials_sponza.back().mat_ubo.diffuse_c = glm::vec4{materials_sponza[i].diffuse[0],
								    materials_sponza[i].diffuse[1],
								    materials_sponza[i].diffuse[2],
								    processed_materials_sponza.back().diffuse_t.
								    has_value() ? 1.0f : 0.0f};
    processed_materials_sponza.back().mat_ubo.specular_c = glm::vec4{materials_sponza[i].specular[0],
								     materials_sponza[i].specular[1],
								     materials_sponza[i].specular[2],
								     processed_materials_sponza.back().specular_t.
								     has_value() ? 1.0f : 0.0f};
    if(processed_materials_sponza.back().ambient_t.has_value())
      processed_materials_sponza.back().ambient_t.value().bind(0);
    if(processed_materials_sponza.back().diffuse_t.has_value())
      processed_materials_sponza.back().diffuse_t.value().bind(1);
    if(processed_materials_sponza.back().specular_t.has_value())
      processed_materials_sponza.back().specular_t.value().bind(2);
    glGenBuffers(1, &processed_materials_sponza.back().ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, processed_materials_sponza.back().ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof( basic_mat_ubo ), &processed_materials_sponza.back().mat_ubo, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
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
			      glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}
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
    sponza_ubo_data.mvp = perspectiveM * viewM;
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
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    //go.tick();
    ////glDrawArrays(GL_TRIANGLES, 0, 3);
    ////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
    //glDrawArrays(GL_TRIANGLES, 0, cube.size()/5);
    for (std::size_t i = 0; i < sponza_vaos.size(); i++) {
      glBindVertexArray(sponza_vaos[i]);
      std::size_t offset = 0;
      for(std::size_t assoc_i = 0; assoc_i < sponza_vbo_materials[i].size(); assoc_i++) {
	std::size_t len = sponza_vbo_materials[i][assoc_i].length;
	int mat_id = sponza_vbo_materials[i][assoc_i].material_id;
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, processed_materials_sponza[mat_id].ubo );

	if(processed_materials_sponza[mat_id].ambient_t.has_value())
	  processed_materials_sponza[mat_id].ambient_t.value().bind(0);
	if(processed_materials_sponza[mat_id].diffuse_t.has_value())
	  processed_materials_sponza[mat_id].diffuse_t.value().bind(1);
	if(processed_materials_sponza[mat_id].specular_t.has_value())
	  processed_materials_sponza[mat_id].specular_t.value().bind(2);

	glDrawArrays(GL_TRIANGLES, offset, len*3);
	offset += 3*len;
      }
    }
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

    // a.texcoord_offset = glm::vec4(std::sin(time), std::cos(time), 0.0f,
    // 0.0f); glBufferData(GL_UNIFORM_BUFFER, sizeof(a), &a, GL_STREAM_DRAW);
  }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    flog.close();
    //glfwDestroyWindow(win);
    //glfwTerminate();
    return 0;
}
