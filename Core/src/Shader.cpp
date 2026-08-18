#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string &filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
  ShaderProgramSource source = ParseShader(filepath);
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
  GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{
  // NOTE: using modern C++ here BUT old-school C is faster (better for Game Engines)
  std::ifstream stream(filepath);
  if (!stream.is_open())
  {
    std::cout << "Failed to open shader file: " << filepath << std::endl;
    return {};
  }

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  // check if we are reading vertex or fragment shader code
  // append each line to the corresponding stringstream ([0]: vertex, [1]: fragment)
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
      {
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
        type = ShaderType::FRAGMENT;
      }
    }
    else
    {
      ss[(int)type] << line << '\n';
    }
  }

  return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
  GLCall(unsigned int id = glCreateShader(type));
  const char *src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr)); // legnth = nullptr to read the whole source code
  GLCall(glCompileShader(id));

  // Error handling
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char *message = (char *)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  if (vertexShader.empty() || fragmentShader.empty())
  {
    return 0;
  }

  // NOTE: using unsigned int instead of GL_UINT makes it more generic and helps with multiple APIs (Vulkan, DirectX)
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  if (vs == 0 || fs == 0)
  {
    GLCall(glDeleteProgram(program));
    return 0;
  }

  // Link shaders into one program
  glAttachShader(program, vs); // FIXME: this throws an error for some reason when wrapped with GLCall()
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));

  int linkStatus;
  GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));
  if (linkStatus == GL_FALSE)
  {
    int length;
    GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
    char *message = (char *)alloca(length * sizeof(char));
    GLCall(glGetProgramInfoLog(program, length, &length, message));
    std::cout << "Failed to link shader program!" << std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    GLCall(glDeleteProgram(program));
    return 0;
  }

  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

void Shader::Bind() const
{
  if (m_RendererID == 0)
    return;
  GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
  if (m_RendererID == 0)
    return;
  GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
  if (m_RendererID == 0)
    return;
  GLCall(glUniform1i(GetUniformLocation(m_RendererID, name), value));
}

void Shader::SetUniform1f(const std::string &name, float value)
{
  if (m_RendererID == 0)
    return;
  GLCall(glUniform1f(GetUniformLocation(m_RendererID, name), value));
}

void Shader::SetUniform2f(const std::string &name, const glm::vec2 &value)
{
  if (m_RendererID == 0)
    return;
  GLCall(glUniform2f(GetUniformLocation(m_RendererID, name), value.x, value.y));
}

void Shader::SetUniform3f(const std::string &name, const glm::vec3 &value)
{
  if (m_RendererID == 0)
    return;
  GLCall(glUniform3f(GetUniformLocation(m_RendererID, name), value.x, value.y, value.z));
}

void Shader::SetUniform4f(const std::string &name, const glm::vec4 &value)
{
  if (m_RendererID == 0)
    return;
  GLCall(glUniform4f(GetUniformLocation(m_RendererID, name), value.x, value.y, value.z, value.w));
}

void Shader::SetUniformMat3f(const std::string &name, const glm::mat3 &matrix)
{
  if (m_RendererID == 0)
    return;

  GLCall(glUniformMatrix4fv(GetUniformLocation(m_RendererID, name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
  if (m_RendererID == 0)
    return;
  GLCall(glUniformMatrix4fv(GetUniformLocation(m_RendererID, name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(unsigned int rendererID, const std::string &name)
{
  if (rendererID == 0)
    return -1;

  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
  if (location == -1)
    std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
  m_UniformLocationCache[name] = location;
  return location;
}
