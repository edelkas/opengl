#include "Texture.h"
#include "external/stb_image/stb_image.h"

Texture::Texture(const std::string& filepath)
  : m_FilePath(filepath), m_LocalBuffer(nullptr), m_Height(0), m_Width(0), m_BPP(0)
{
  /**
   * Load image.
   *
   * - Flip because OpenGL expects origin to be at the bottom.
   * - stbi_load will also set the member variables whose address we specify.
   * - The last parameter is the number of desired channels (4: RGBA).
   */
  stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

  /**
   * Generate and bind texture
   */
  GLCall(glGenTextures(1, &m_RendererID));            // Create texture, index is saved in pointer
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); // Bind texture and establish type

  /**
   * Configure basic parameters of the texture
   *
   * - Minification filter: What to do when the texture is minimized.
   * - Maximization filter: What to do when the texture is maximized.
   * - Horizontal wrapping: What to do when the texture is done horizontally.
   * - Vertical wrapping:   What to do when the texture is done vertically.
   */
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // Linear interpolation
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_FILTER, GL_LINEAR)); // Linear interpolation
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));      // Clamping (instead of tiling)
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));      // Clamping (instead of tiling)

  /**
   * Provide OpenGL with the texture data.
   *
   * - Texture type.
   * - Texture levels.
   * - Internal format in which OpenGL will save the texture data.
   * - Width of the image in pixels.
   * - Height of the image in pixels.
   * - Borders of the texture.
   * - External format of the data we are providing.
   * - Format of each of the channels in said data.
   * - Pointer to start of data (can be nullptr to only allocate).
   */
  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

  /**
   * Unbind texture and free memory
   */
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture(){
  GLCall(glDeleteTextures(1, m_RendererID));
}

Texture::Bind(unsigned int slot = 0){
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));        // Select slot to bind texture
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

Texture::Unbind(){
 GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
