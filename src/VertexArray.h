#pragma once
#include "VertexBuffer.h"
#include "BufferLayout.h"

class VertexArray
{
private:
  unsigned int m_RendererID;
public:
  VertexArray();
  ~VertexArray();

  void AddBuffer(const VertexBuffer& vb, const BufferLayout& layout) const;
  void Bind() const;
  void Unbind() const;
};
