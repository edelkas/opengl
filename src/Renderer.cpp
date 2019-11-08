#include <iostream>
#include "Renderer.h"

void GLClearErrors(){
  while(glGetError() != GL_NO_ERROR);
}

bool GLCheckErrors(const char* function, const char* file, int line){
  bool error = false;
  while(GLenum error = glGetError()){
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
    error = true;
  }
  return error;
}

void Renderer::Clear() const {
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const DrawElement& elem) const {
  elem.shader.Bind();
  elem.va.Bind();
  elem.ib.Bind();
  GLCall(glDrawElements(GL_TRIANGLES, elem.ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
