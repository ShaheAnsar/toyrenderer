#include <algorithm>
#include <iostream>


#include <engine2D.hpp>
#include "../include/logger.hpp"

extern logger::logger mlog;

#define POSITION 0 // Location of this variable in the shader
#define UV 1 // Location of this variable in the shader
#define COLOR 2 // Location of this variable in the shader


/*
 * The uniform buffer for renderable consists of
 * - vec4 offset_scale - This is normally the position (for x,y) of the center of the quad. (z,w) are scaling constants
 * - vec4 angle - The x component is a rotation angle
 */
#define UNIFORM_BUFFER_RENDERABLE 0

struct uniform_buffer_renderable_data {
  glm::vec4 offset_scale;
  glm::vec4 angle;
};


using namespace engine2D;


// Stuff for Quad

void Quad::calculate() {
  for(int i = 0; i < 4; i++) {
    vertices[i] = vertices_c[i] + position;
  }
}


Quad::Quad(glm::vec2 ll, glm::vec2 dim) : scale(1.0f) {
  vertices[0] = ll;
  vertices[1] = vertices[0] + glm::vec2{0, dim.x};
  vertices[2] = vertices[1] + glm::vec2{dim.y, 0};
  vertices[3] = vertices[0] + glm::vec2{dim.y, 0};
  position = glm::vec2(0.0f);
  for(int i = 0; i < 4; i++) {
    position += vertices[i];
  }
  position /= 4;
  for(int i = 0; i < 4; i++) {
    vertices_c[i] = vertices[i] - position;
  }
  for(int i = 0; i < 4; i++) {
    std::cerr << "[DEBUG] - FROM QUAD" << vertices_c[i].x << ", " <<  vertices_c[i].y << std::endl;
  }
}


void Quad::translate(glm::vec2 offset) {
  position += offset;
  calculate();
}

void Quad::rotate(float angle) {
  angle += angle;
  calculate();
}

void Quad::fscale(glm::vec2 s) {
  scale = s;
}


// Stuff for Renderable

Renderable::Renderable(glm::vec2 dim, glm::vec2 ll) : quad(ll, dim), color(glm::vec4(1.0f)) {
  pri = AttrPri::LAST;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenBuffers(1, &ubo);
}

void Renderable::add_tex(Texture t) {
  
}


void Renderable::add_color(glm::vec4 c){
  color = c;
}

void Renderable::set_shaderprog(Rend::ShaderProgram p) {
  shader_prog = p;
}

void Renderable::submit_data() {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_BUFFER_RENDERABLE, ubo);
  std::vector<float> array_buffer_data;
  for(int i = 0; i < 4; i++) {
    array_buffer_data.push_back(quad.vertices_c[i].x);
    array_buffer_data.push_back(quad.vertices_c[i].y);
    switch(i % 4) {
    case 0:
      array_buffer_data.push_back(0.0f);
      array_buffer_data.push_back(0.0f);
      break;
      
    case 1:
      array_buffer_data.push_back(1.0f);
      array_buffer_data.push_back(0.0f);
      break;
 
    case 2:
      array_buffer_data.push_back(1.0f);
      array_buffer_data.push_back(1.0f);
      break;
    case 3:
      array_buffer_data.push_back(0.0f);
      array_buffer_data.push_back(1.0f);
      break;
 
    }
    array_buffer_data.push_back(color.r);
    array_buffer_data.push_back(color.g);
    array_buffer_data.push_back(color.b);
    array_buffer_data.push_back(color.a);
  }
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*array_buffer_data.size(), array_buffer_data.data(), GL_STREAM_DRAW);
  uint8_t element_indices[] = {
			     0, 1, 2,
			     0, 2, 3
  };
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint8_t)*sizeof(element_indices), element_indices, GL_STREAM_DRAW);
  uniform_buffer_renderable_data data {
				       glm::vec4{quad.position, quad.scale},
				       glm::vec4{quad.angle, 0.0f, 0.0f, 0.0f}
  };
  glBufferData(GL_UNIFORM_BUFFER, sizeof(data), &data, GL_STREAM_DRAW);
  glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(float)*((2 + 2 + 4)), (void*)0);
  glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, sizeof(float)*((2 + 2 + 4)), (void*)(sizeof(float)*2));
  glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(float)*((2 + 2 + 4)), (void*)(sizeof(float)*4));
  glEnableVertexAttribArray(POSITION);
  glEnableVertexAttribArray(UV);
  glEnableVertexAttribArray(COLOR);
}


void Renderable::tick() {
  shader_prog.use_program();
  glBindVertexArray(vao);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
}

// GameObject stuff


GameObject::GameObject(glm::vec2 position) : position(position), attributes(0) {
} 


void GameObject::add_attribute(std::shared_ptr<Attribute> attr) {
  attributes.push_back(attr);
  std::sort(attributes.begin(), attributes.end(), [](std::shared_ptr<Attribute>& l, std::shared_ptr<Attribute>& r)
						  {
						    return l->pri < r->pri;
						  });
}

void GameObject::tick() {
  for(const auto& attr : attributes) {
    attr->tick();
  }
}
