#include <iostream>
#include <sstream>
#include <fstream>
#include "Renderer.h"
#include "Shader.h"

Shader::Shader(const std::string& filepath) : m_RendererID(0), m_FilePath(filepath) {
  ShaderSource src = ParseShader(filepath);                          // Read shader sources
  m_RendererID = CreateShader(src.vertexSource, src.fragmentSource); // Set up shader
}

Shader::~Shader() {
  GLCall(glDeleteProgram(m_RendererID));     // Delete shader from memory
}

// ------ Public methods ------

void Shader::Bind() const {
  GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
  GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value){
  GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

// ------ Private methods ------

ShaderSource Shader::ParseShader(const std::string& filepath){
  std::ifstream stream(filepath);                    // Input file stream
  std::string line;                                  // Store current line
  std::stringstream ss[2];                           // Store code for each shader

  enum class ShaderType{ NONE = -1, VERTEX = 0, FRAGMENT = 1 };
  ShaderType type = ShaderType::NONE;

  /* Store each line of code into the appropriate stream, either vertex or fragment */
  while(getline(stream, line)){                      // Loop through lines of stream
    if (line.find("#shader") != std::string::npos){  // Shader sections
      if (line.find("vertex") != std::string::npos){ // Vertex shader header
        type = ShaderType::VERTEX;
      } else {                                       // Fragment shader header
        type = ShaderType::FRAGMENT;
      }
    } else {                                         // Any other line is code
      if (type != ShaderType::NONE) ss[(int)type] << line << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };               // Implicitly create struct
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source){
  /* Compile shader */
  GLCall(unsigned int id = glCreateShader(type));    // Create shader and store id
  const char* src = &source[0];                      // Cast source to C string
  GLCall(glShaderSource(id, 1, &src, nullptr));      // Select source of shader
  GLCall(glCompileShader(id));                       // Compile shader

  /* Handle errors */
  // Retrieve compile status
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE){
    // Retrieve length of error message
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

    // Retrieve error message
    char *message = new char[length];
    GLCall(glGetShaderInfoLog(id, length, &length, message));

    // Print error message
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:\n" << message << std::endl;
    delete[] message;

    // Delete erroneous shader
    GLCall(glDeleteShader(id));
    return 0;
  }

  // Return
  return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  // TODO: Error handling: Check for compile errors.

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);                        // Delete intermediate object files
  glDetachShader(program, vs);               // Delete shader source from memory
  glDeleteShader(fs);
  glDetachShader(program, fs);

  return program;
}

int Shader::GetUniformLocation(const std::string& name) {
  if (m_UniformLocations.find(name) != m_UniformLocations.end()) return m_UniformLocations[name];
  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1) {                      // The uniform index is -1 if it was not found.
    std::cout << "Warning: Uniform " << name << " does not exist." << std::endl;
  }
  m_UniformLocations[name] = location;
  return location;
}
