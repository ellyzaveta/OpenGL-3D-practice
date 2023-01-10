#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
public:

    ~ShaderProgram();

    static unsigned int compileShader(unsigned int type, const std::string &source);

    static unsigned int linkShaders(const std::string &vertexShader, const std::string &fragmentShader);

    static ShaderProgram
    init(const std::string &vertexShader, const std::string &fragmentShader);

    void use();

    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    unsigned int getShaderProgramId();

private:
    unsigned int shader;

    explicit ShaderProgram(const unsigned int &inputId);
};