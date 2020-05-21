#ifndef MY_UNIFORM_BUFFER_HPP
#define MY_UNIFORM_BUFFER_HPP

#include <glad/glad.h>

namespace Rend{
  template<typename BufferType>
  struct UniformBuffer{
    BufferType buffer;
    GLuint buf_id;
    inline void upload(const BufferType& buf) {
      glBindBuffer(GL_UNIFORM_BUFFER, buf_id);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(BufferType), &buf, GL_STREAM_DRAW);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    inline void upload() {
      upload(buffer);
    }

    UniformBuffer(){
      glGenBuffers(1, &buf_id);
    }
    UniformBuffer(const BufferType& buf){
      buffer = buf;
      glGenBuffers(1, &buf_id);
    }
    ~UniformBuffer() {
      glDeleteBuffers(1, &buf_id);
    }
    void bind(GLuint slot) {
      glBindBufferBase(GL_UNIFORM_BUFFER, slot, buf_id);
    }
  };
}


#endif
