#pragma once

#include <string>
#include <vector>

#include <common.hpp>
#include <texture.hpp>


namespace Rend{
  enum class FBTextureType{
			   COLOR_ATTACHMENT,
			   DEPTH_ATTACHMENT,
			   STENCIL_ATTACHMENT,
			   DEPTH_STENCIL_ATTACHMENT
  };
  struct FBTexture{
    FBTextureType type;
    Rend::Texture tex;
    GLuint attachment_slot; // Only used for color attachments
  };
  struct FrameBuffer{
    std::string name; // Name for internal use purposes
    std::vector<FBTexture> textures;
    GLuint fbo;
    std::size_t* refc;
    FrameBuffer(const std::vector<FBTexture>& textures = {});
    FrameBuffer(const std::string& name,
		const std::vector<FBTexture>& textures = {});
    FrameBuffer(const FrameBuffer&);
    FrameBuffer& operator=(const FrameBuffer&);
    void attach_attachments();
    void bind();
    ~FrameBuffer();
  };
};
