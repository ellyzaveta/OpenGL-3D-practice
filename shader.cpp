#include "shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

using namespace std;

ShaderProgram
ShaderProgram::init(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int shaderProgramId = ShaderProgram::linkShaders(vertexShader, fragmentShader);
    return ShaderProgram(shaderProgramId);
}

ShaderProgram::ShaderProgram(const unsigned int &inputId) : shader(inputId) {}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shader);
}

void success(GLenum pname, unsigned int &object) 
{
    int  success;
    char infoLog[512];

    if(pname == GL_LINK_STATUS) 
    {
        glGetProgramiv(object, pname, &success);
        if (!success) 
        {
            glGetProgramInfoLog(object, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    } 
    else 
    {
        glGetShaderiv(object, pname, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

}

unsigned int ShaderProgram::compileShader(unsigned int type, const string &source) {
    unsigned int shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    success(GL_COMPILE_STATUS, shader);

    return shader;
}

unsigned int ShaderProgram::linkShaders(const string &vertexShader, const string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    success(GL_LINK_STATUS, program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}


void ShaderProgram::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(shader, name.c_str()), x, y, z, w);
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::use() {
    glUseProgram(shader);
}

unsigned int ShaderProgram::getShaderProgramId() {
    return shader;
}