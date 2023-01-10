#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <cmath>

using namespace std;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const GLint WIDTH = 1440;
const GLint HEIGHT = 1024;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

GLFWwindow* window;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

glm::mat4 projection;

bool init();
void display();
void processInput();
void initArrays(unsigned int &vao, GLsizei stride, vector<float>& positions);

pair<int, int> getGraphData();
pair<int, int> getSphereData();
pair<int, int> getTorusData();

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const string VERTEX_SHADER_SOURCE = R"glsl(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main(){
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)glsl";

const string FRAGMENT_SHADER_SOURCE = R"glsl(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 color;
    void main() {
        FragColor = color;
    }
)glsl";

int main() {
    if(!init()) return -1;

    display();
    glfwTerminate();

    return 0;
}

void display() {
    ShaderProgram shaderProgram = ShaderProgram::init(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
    shaderProgram.use();

    pair<int, int> graphic1Data = getGraphData();
    pair<int, int> torusData = getTorusData();
    pair<int, int> sphereData = getSphereData();
    
    projection = glm::perspective(glm::radians(camera.getZoom()), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", camera.getMatrix());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        
        //sphere solid
        model = glm::translate(model, glm::vec3(2.0f, 2.0f, 0.0f));
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec4("color", 255.0f / 255.0f, 230.0f / 255.0f, 0.0f / 255.0f, 1.0f);
        glBindVertexArray(sphereData.first);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sphereData.second);

        //sphere frame
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -2.0f, 0.0f));
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec4("color", 255.0f / 255.0f, 230.0f / 255.0f, 0.0f / 255.0f, 1.0);
        glBindVertexArray(sphereData.first);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sphereData.second);

        //torus solid
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 2.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec4("color", 178.0f / 255.0f, 112.0f / 255.0f, 225.0f / 255.0f, 1.0f);
        glBindVertexArray(torusData.first);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, torusData.second);

        //torus frame
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f));
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec4("color", 178.0f / 255.0f, 112.0f / 255.0f, 225.0f / 255.0f, 1.0f);
        glBindVertexArray(torusData.first);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, torusData.second);

        //graph frame
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -10.0f));
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec4("color", 143.0f / 255.0f, 210.0f / 255.0f, 33.0f / 255.0f, 1.0f);
        glBindVertexArray(graphic1Data.first);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, graphic1Data.second);

        //graph solid
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, -10.0f));
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec4("color", 143.0f / 255.0f, 210.0f / 255.0f, 33.0f / 255.0f, 1.0f);
        glBindVertexArray(graphic1Data.first);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, graphic1Data.second);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboardInput(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboardInput(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboardInput(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboardInput(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) projection = glm::ortho(-(WIDTH / 256.0f),  WIDTH / 256.0f,
                                                                              -(HEIGHT / 256.0f), (HEIGHT / 256.0f),
                                                                              -1000.0f, 1000.0f);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) projection = glm::perspective(glm::radians(camera.getZoom()), 
                                                                    (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
}

void initArrays(unsigned int &vao, GLsizei stride, vector<float>& positions) {
    unsigned int vbo;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

pair<int, int> getGraphData() {
    vector<float> vertices;
    
    float start = -2.0f;
    float end = 2.0f;
    float step = 0.1f;

    auto addToVertices = [](vector<float> &vertices, float x, float y) { 
        vertices.push_back(x);
        vertices.push_back(sin(y) * (x * x));
        vertices.push_back(y);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
    };

    for (float x = start; x < end;) {
        for (float y = start; y < end;) {
            addToVertices(vertices, x, y);
            addToVertices(vertices, x, y + step);
            addToVertices(vertices, x + step, y + step);
            addToVertices(vertices, x, y);
            addToVertices(vertices, x + step, y);
            addToVertices(vertices, x + step, y + step);
            y += step;
        }
        x += step;
    }

    unsigned int vao;
    initArrays(vao, 6 * sizeof(float), vertices);

    return {vao, pow((abs(start) + abs(end)) / step, 2) * 6};
}

pair<int, int> getSphereData() {
    int i, j;
    std::vector<float> vertices;
    std::vector<int> indices;

    int density = 40;
    int indicator = 0;
   
    for(i = 0; i <= density; i++) {
       double lat0 = glm::pi<double>() * (-0.5 + (double) (i - 1) / density);
       double z0  = sin(lat0);
       double zr0 =  cos(lat0);

       double lat1 = glm::pi<double>() * (-0.5 + (double) i / density);
       double z1 = sin(lat1);
       double zr1 = cos(lat1);

       for(j = 0; j <= density; j++) {
           double lng = 2 * glm::pi<double>() * (double) (j - 1) / density;
           double x = cos(lng);
           double y = sin(lng);

           vertices.push_back(x * zr0);
           vertices.push_back(y * zr0);
           vertices.push_back(z0);
           indices.push_back(indicator);
           indicator++;

           vertices.push_back(x * zr1);
           vertices.push_back(y * zr1);
           vertices.push_back(z1);
           indices.push_back(indicator);
           indicator++;
       }
       indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
   }

    unsigned int vao;
    initArrays(vao, 0, vertices);
    
    return {vao, indices.size()};
}

pair<int, int> getTorusData() {
    vector<float> vertices;
    double r = 0.15;
    double c = 0.3;
    int rSeg = 16;
    int cSeg = 36;
    const double TAU = 2.0 * glm::pi<double>();

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j <= cSeg; j++) {
            for (int k = 0; k <= 1; k++) {
                double s = (i + k) % rSeg + 0.5;
                double t = j % (cSeg + 1);

                double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
                double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
                double z = r * sin(s * TAU / rSeg);

                vertices.push_back(2 * x);
                vertices.push_back(2 * y);
                vertices.push_back(2 * z);
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
            }
        }
    }

    unsigned int vao;  
    initArrays(vao, 6 * sizeof(float), vertices);

    return {vao, vertices.size() / 6};
}

bool init() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(WIDTH, HEIGHT, "lab2", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return 0;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (glewInit() != GLEW_OK) {
        return 0;
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    return 1;
}


void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseInput(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}