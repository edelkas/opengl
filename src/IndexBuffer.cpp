#include "Renderer.h"
#include "IndexBuffer.h"

class IndexBuffer
{
private:
  unsigned int m_RendererID;
  unsigned int m_Count;
public:
  assert(sizeof(unsigned int) == sizeof(GLuint));

  IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count){
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
  }

  ~IndexBuffer(){
    GLCall(glDeleteBuffers(1, &m_RendererID));
  }

  void Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  }

  void Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }
};
