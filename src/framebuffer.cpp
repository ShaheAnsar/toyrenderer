#include <framebuffer.hpp>

#include <fstream>

extern std::fstream flog;
extern std::fstream glog;


using namespace Rend;



FrameBuffer::FrameBuffer(const std::vector<FBTexture>& textures) :
  textures(textures), name("nil"), refc(nullptr)
{
  refc = new std::size_t;
  if(!refc)
    flog << "Framebuffer refc allocation error" << std::endl;
  *refc = 1;
  glGenFramebuffers(1, &fbo);
}

FrameBuffer::FrameBuffer(const std::string& name,
			 const std::vector<FBTexture>& textures) :
  textures(textures), name(name), refc(nullptr)
{
  refc = new std::size_t;
  if (!refc)
    flog << "Framebuffer refc allocation error" << std::endl;
  *refc = 1;
  glGenFramebuffers(1, &fbo);
}

FrameBuffer::FrameBuffer(const FrameBuffer& f):
  name(f.name.c_str()), textures(f.textures), fbo(f.fbo), refc(f.refc)
{
  (*refc)++;
}

FrameBuffer& FrameBuffer::operator=(const FrameBuffer& f) {
  if(*refc <= 1) {
    delete refc;
    refc = nullptr;
    glDeleteFramebuffers(1, &fbo);
    flog << "Deleting FrameBuffer " << name << std::endl;
  } else {
    (*refc)--;
  }

  name = f.name.c_str();
  textures = f.textures;
  fbo = f.fbo;
  refc = f.refc;
  (*refc)++;
  return *this;
}

FrameBuffer::~FrameBuffer() {
  if(*refc <= 1) {
    delete refc;
    refc = nullptr;
    glDeleteFramebuffers(1, &fbo);
  } else {
    (*refc)--;
  }
}

void FrameBuffer::attach_attachments() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  std::vector<FBTexture> color_attachments;
  for(const auto& i : textures) {
    GLenum attachment_type = 0;
    switch(i.type){
    case FBTextureType::COLOR_ATTACHMENT:
      attachment_type = GL_COLOR_ATTACHMENT0 + i.attachment_slot;
      color_attachments.push_back(i);
      break;
    case FBTextureType::DEPTH_ATTACHMENT:
      attachment_type = GL_DEPTH_ATTACHMENT;
      break;
    case FBTextureType::STENCIL_ATTACHMENT:
      attachment_type = GL_STENCIL_ATTACHMENT;
      break;
    case FBTextureType::DEPTH_STENCIL_ATTACHMENT:
      attachment_type = GL_DEPTH_STENCIL_ATTACHMENT;
      break;
    default:
      flog << "Shouldn't happen: " << __FILE__ << ":" << __LINE__ << std::endl;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, GL_TEXTURE_2D, i.tex.tex_id, 0);
  }
  std::sort(color_attachments.begin(), color_attachments.end(),
	    [](const FBTexture& a, const FBTexture& b) {
	      return a.attachment_slot < b.attachment_slot;
	    });
  std::vector<GLenum> tex_ids;
  for(const auto& i : color_attachments) {
    tex_ids.push_back(GL_COLOR_ATTACHMENT0 + i.attachment_slot);
  }
  glDrawBuffers(tex_ids.size(),
		tex_ids.data());
}

void FrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

bool FrameBuffer::is_complete() {
  bind();
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
