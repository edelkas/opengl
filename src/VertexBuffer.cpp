#include "Renderer.h"
#include "VertexBuffer.h"

class VertexBuffer
{
private:
  unsigned int m_RendererID;
public:
  VertexBuffer(const void* data, unsigned int size){
    GLCall(glGenBuffers(1, &m_RendererID));                            // Create buffer, index is saved in pointer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));               // Bind buffer and establish type, mere array of data
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Save data in buffer
  }

  ~VertexBuffer(){
    GLCall(glDeleteBuffers(1, &m_RendererID));
  }

  void Bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  }

  void Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }
};
