#include "Shader.h"

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <string>

#include "GLCore/OpenGLDebug.h"
#include "Util/Log.h"

Shader::Shader(const std::string &vertexFilePath,
               const std::string &fragmentFilePath)
{
  std::string vertexSource = ReadFile(vertexFilePath);
  std::string fragmentSource = ReadFile(fragmentFilePath);
  m_RendererID = CreateShader(vertexSource, fragmentSource);
}

Shader::~Shader() { glDeleteProgram(m_RendererID); }

std::string Shader::ReadFile(const std::string &filepath)
{
  std::ifstream stream(filepath);
  if (!stream.is_open())
  {
    LOG_ERROR("Failed to open shader file: {}", filepath);
    return {};
  }

  std::stringstream ss;
  std::string line;
  while (getline(stream, line))
  {
    ss << line << '\n';
  }

  return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string &source)
{
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src,
                 nullptr); // legnth = nullptr to read the whole source code
  glCompileShader(id);

  // Error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    LOG_ERROR("Failed to compile {} shader",
              type == GL_VERTEX_SHADER ? "vertex" : "fragment");
    LOG_ERROR("{}", message);
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader,
                                  const std::string &fragmentShader)
{
  if (vertexShader.empty() || fragmentShader.empty())
  {
    return 0;
  }

  // NOTE: using unsigned int instead of GL_UINT makes it more generic and helps
  // with multiple APIs (Vulkan, DirectX)
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  if (vs == 0 || fs == 0)
  {
    glDeleteProgram(program);
    return 0;
  }

  // Link shaders into one program
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  int linkStatus;
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  if (linkStatus == GL_FALSE)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetProgramInfoLog(program, length, &length, message);
    LOG_ERROR("Failed to link shader program");
    LOG_ERROR("{}", message);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
    return 0;
  }

  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

void Shader::Bind() const
{
  if (m_RendererID == 0)
    return;
  glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
  if (m_RendererID == 0)
    return;
  glUseProgram(0);
}

void Shader::SetUniform1i(const std::string &name, int value)
{
  if (m_RendererID == 0)
    return;
  glUniform1i(GetUniformLocation(m_RendererID, name), value);
}

void Shader::SetUniform1iv(const std::string &name, int count,
                           const int *values)
{
  if (m_RendererID == 0)
    return;
  glUniform1iv(GetUniformLocation(m_RendererID, name), count, values);
}

void Shader::SetUniform1f(const std::string &name, float value)
{
  if (m_RendererID == 0)
    return;
  glUniform1f(GetUniformLocation(m_RendererID, name), value);
}

void Shader::SetUniform2f(const std::string &name, const glm::vec2 &value)
{
  if (m_RendererID == 0)
    return;
  glUniform2f(GetUniformLocation(m_RendererID, name), value.x, value.y);
}

void Shader::SetUniform3f(const std::string &name, const glm::vec3 &value)
{
  if (m_RendererID == 0)
    return;
  glUniform3f(GetUniformLocation(m_RendererID, name), value.x, value.y,
              value.z);
}

void Shader::SetUniform4f(const std::string &name, const glm::vec4 &value)
{
  if (m_RendererID == 0)
    return;
  glUniform4f(GetUniformLocation(m_RendererID, name), value.x, value.y, value.z,
              value.w);
}

void Shader::SetUniformMat3f(const std::string &name, const glm::mat3 &matrix)
{
  if (m_RendererID == 0)
    return;

  glUniformMatrix3fv(GetUniformLocation(m_RendererID, name), 1, GL_FALSE,
                     &matrix[0][0]);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
  if (m_RendererID == 0)
    return;
  glUniformMatrix4fv(GetUniformLocation(m_RendererID, name), 1, GL_FALSE,
                     &matrix[0][0]);
}

int Shader::GetUniformLocation(unsigned int rendererID,
                               const std::string &name)
{
  if (rendererID == 0)
    return -1;

  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  int location = glGetUniformLocation(rendererID, name.c_str());
  if (location == -1)
    LOG_WARN("Uniform '{}' does not exist", name);
  m_UniformLocationCache[name] = location;
  return location;
}
