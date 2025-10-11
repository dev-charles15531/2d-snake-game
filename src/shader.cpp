#include "../include/shader.hpp"
#include "../include/glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::ifstream vsfile(vertexPath);
  std::ifstream fsfile(fragmentPath);
  if (!vsfile || !fsfile) {
    throw std::runtime_error("Failed to open shader file");
  }

  std::stringstream vsCodeStream, fsCodeStream;
  vsCodeStream << vsfile.rdbuf();
  fsCodeStream << fsfile.rdbuf();

  GLint success;
  GLuint vertexShader, fragmentShader;

  std::string vertexShaderCode = vsCodeStream.str();
  const char *vertexShaderSource = vertexShaderCode.c_str();

  std::string fragmentShaderCode = fsCodeStream.str();
  const char *fragmentShaderSource = fragmentShaderCode.c_str();

  char infoLog[512];
  // ################## Vertex Shader ##################
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // ################## Fragment Shader ##################
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // ################## Shader Program ##################
  Shader::ID = glCreateProgram();
  glAttachShader(Shader::ID, vertexShader);
  glAttachShader(Shader::ID, fragmentShader);

  glLinkProgram(Shader::ID);
  glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(Shader::ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() { glUseProgram(Shader::ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(Shader::ID, name.c_str()), value);
}
