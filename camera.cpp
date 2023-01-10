#include "camera.h"

Camera::Camera(glm::vec3 position) {
    cameraPos = position;
    updateCamera();
}

glm::mat4 Camera::getMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

float Camera::getZoom(){
    return zoom;
}

void Camera::processKeyboardInput(CameraMovement direction, float deltaTime) {
    float cameraSpeed = movementSpeed * deltaTime;
    if (direction == FORWARD)
        cameraPos += cameraFront * cameraSpeed;
    if (direction == BACKWARD)
        cameraPos -= cameraFront * cameraSpeed;
    if (direction == LEFT)
        cameraPos -= cameraRight * cameraSpeed;
    if (direction == RIGHT)
        cameraPos += cameraRight * cameraSpeed;
}

void Camera::processMouseInput(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCamera();
}

void Camera::processMouseScroll(float yoffset) {
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}

void Camera::updateCamera() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction.y = sin(glm::radians(Pitch));
    direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, globalUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}