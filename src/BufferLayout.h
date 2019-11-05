#pragma once
#include <vector>
#include "Renderer.h"

struct VertexAttribute {
  unsigned int type;   // GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_CHAR,...
  unsigned int count;
  unsigned char norm;  // 1 or 0, normalized or not

  static unsigned int GetSize(unsigned int type){
    switch(type){
      case GL_FLOAT:         return sizeof(float);
      case GL_UNSIGNED_INT:  return sizeof(unsigned int);
      case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
    }
    assert(false);     // Invalid type
    return 0;
  }
};

class BufferLayout
{
private:
  std::vector<VertexAttribute> m_Attributes; // Vector of attributes of vertex
  unsigned int m_Stride;                     // Total size of vertex in bytes
public:
  BufferLayout() : m_Stride(0) { }

  // Add attribute to the vector. Overloaded template. 3 types implemented.
  // To support more types, this needs to be extended (or generalized).
  template<typename T> void Push(unsigned int count) {
    //std::cout << "Error: unsupported type " << typeid(T).name() << std::endl;
    assert(false);
  }

  inline const std::vector<VertexAttribute>& GetAttributes() const { return m_Attributes; }
  inline unsigned int GetStride() const { return m_Stride; }
};

// NOTA: Para corregir fallo de redefinicion, hacer estas especializaciones inline.
template<> void BufferLayout::Push<float>(unsigned int count) {
  m_Attributes.push_back({ GL_FLOAT, count, GL_FALSE });
  m_Stride += count * VertexAttribute::GetSize(GL_FLOAT);
}
template<> void BufferLayout::Push<unsigned int>(unsigned int count) {
  m_Attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
  m_Stride += count * VertexAttribute::GetSize(GL_UNSIGNED_INT);
}
template<> void BufferLayout::Push<unsigned char>(unsigned int count) {
  m_Attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
  m_Stride += count * VertexAttribute::GetSize(GL_UNSIGNED_BYTE);
}
