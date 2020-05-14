#include "../include/shader.hpp"
#include <vector>
#include <string>
#include <utility>
#include <filesystem>
#include <fstream>

#include <logger.hpp>

extern logger::logger mlog;

using namespace Rend;

Shader::Shader(std::string fn, GLenum shader_type, const GLchar* entrypoint, std::pair<std::vector<GLuint>, std::vector<GLuint>> specialization_constants) : shader_type(shader_type) {
  unsigned long size = std::filesystem::file_size(std::filesystem::path(fn));
  std::fstream binary_stream(fn, std::ios_base::binary | std::ios_base::in);
  std::vector<char> binary(size);
  binary_stream.read(binary.data(), size);
  shader_id = glCreateShader(shader_type);
  glShaderBinary(1, &shader_id, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size());
  glSpecializeShader(shader_id, entrypoint, specialization_constants.first.size(), specialization_constants.first.data(), specialization_constants.second.data());
  int success = 0;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if(!success) {
    mlog << std::pair(logger::pri::ERR, "Unable to specialize shader!");
  }
  else {
    mlog << "Shader " << shader_id << " Specialized";
  }
}

ShaderProgram::ShaderProgram() : shader_created(false){

}

void ShaderProgram::create_program(std::vector<Shader> shaders) {
  program_id = glCreateProgram();
  for (const auto &i : shaders) {
    glAttachShader(program_id, i.shader_id);
  }
  glLinkProgram(program_id);
  int success = 0;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    GLint buflen = 0;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &buflen);
    char* buf = new char[buflen];
    glGetProgramInfoLog(program_id, buflen, &buflen, buf);
    mlog << std::make_pair(logger::pri::ERR,
			   "Unable to link program " + std::to_string(program_id)
			   + "\n[OPENGL LOG]\n" + buf);
  } else {
    mlog << "Successfully linked program " + std::to_string(program_id);
    shader_created = true;
  }
}


ShaderProgram::ShaderProgram(std::vector<Shader> shaders) {
  create_program(shaders);
}

void ShaderProgram::use_program() {
  mlog << "Using program " + std::to_string(program_id);
  glUseProgram(program_id);
}

ShaderProgram::~ShaderProgram() {
  if(shader_created)
    glDeleteProgram(program_id);
}
