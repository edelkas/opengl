#pragma once

class IndexBuffer
{
private:
  unsigned int m_RendererID; // OpenGL ID of the ibo
  unsigned int m_Count;      // Amount of indices in ibo
public:
  /* We could support unsigned shorts for simpler models */
  IndexBuffer(const unsigned int* data, unsigned int count);
  ~IndexBuffer();

  /* Additional methods: Setdata, Getdata, LockBuffer, UnlockBuffer... */
  void Bind() const;
  void Unbind() const;
  inline unsigned int GetCount() const { return m_Count; }
};
