#pragma once

class VertexBuffer
{
private:
  unsigned int m_RendererID; // OpenGL ID of the vbo
public:
  VertexBuffer(const void* data, unsigned int size);
  ~VertexBuffer();

  /* Additional methods: Setdata, Getdata, LockBuffer, UnlockBuffer... */
  void Bind() const;
  void Unbind() const;
};
