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
