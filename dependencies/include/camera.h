#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float SPEED = 3.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float YAW = -90.0f;
const float PITCH = 0.0f;

class Camera {
public:

    Camera(glm::vec3 position);

    glm::mat4 getMatrix();
    
    float getZoom();

    void processKeyboardInput(CameraMovement direction, float deltaTime);

    void processMouseInput(float xoffset, float yoffset, bool constrainPitch = true);

    void processMouseScroll(float yoffset);

private:
    float movementSpeed = SPEED;
    float sensitivity = SENSITIVITY;
    float zoom = ZOOM;

    glm::vec3 cameraPos;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
   
    float Yaw = YAW;
    float Pitch = PITCH;

    void updateCamera();
};