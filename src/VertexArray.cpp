#include "Renderer.h"
#include "VertexArray.h"

VertexArray::VertexArray() {
  GLCall(glGenVertexArrays(1, &m_RendererID));
  GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
  GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout) const {
  Bind();                                        // Bind vao so that changes in the buffer are applied to it
  vb.Bind();
  const auto& attributes = layout.GetAttributes();
  unsigned int offset = 0;
  for (int i = 0; i < attributes.size(); i++) {
    const auto& attrib = attributes[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, attrib.count, attrib.type, attrib.norm, layout.GetStride(), (const void*)offset));
    offset += attrib.count * VertexAttribute::GetSize(attrib.type);
  }
}

void VertexArray::Bind() const {
  GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
  GLCall(glBindVertexArray(0));
}
