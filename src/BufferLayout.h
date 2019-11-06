#pragma once
#include <vector>
#include "Renderer.h"

struct VertexAttribute {
  unsigned int type;   // GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_CHAR,...
  unsigned int count;
  unsigned char norm;  // 1 or 0, normalized or not

  static unsigned int GetType(const char* type){
    if (type == "float")         return GL_FLOAT;
    if (type == "unsigned int")  return GL_UNSIGNED_INT;
    if (type == "unsigned char") return GL_UNSIGNED_BYTE;
    assert(false);     // Invalid type
  }

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

  // Add attribute to the vector.
  void Push(const char* type, unsigned int count) {
    unsigned int attributeType = VertexAttribute::GetType(type);
    m_Attributes.push_back({ attributeType, count, GL_FALSE });
    m_Stride += count * VertexAttribute::GetSize(attributeType);
  }

  inline const std::vector<VertexAttribute>& GetAttributes() const { return m_Attributes; }
  inline unsigned int GetStride() const { return m_Stride; }
};
