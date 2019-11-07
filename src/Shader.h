#pragma once
#include <string>
#include <unordered_map>

struct ShaderSource{
  std::string vertexSource;
  std::string fragmentSource;
};

class Shader
{
private:
  unsigned int m_RendererID;
  std::string m_FilePath;
  std::unordered_map<std::string, int> m_UniformLocations;
public:
  Shader(const std::string& filepath);
  ~Shader();
  void Bind();
  void Unbind();
  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
  ShaderSource ParseShader(const std::string& filepath);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
  int GetUniformLocation(const std::string& name);
};
