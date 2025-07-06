#include "../include/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

// Kamerayı oluşturur, başlangıç ayarlarını yapılır
Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
    : position(position), front(front), worldUp(up), yaw(-90.0f), pitch(0.0f),
      movementSpeed(2.5f), mouseSensitivity(0.1f), fov(45.0f),
      cameraDistance(2.0f), cameraHeight(1.5f) {
    updateCameraVectors();
}

Camera::~Camera() {
    
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Camera::setFront(const glm::vec3& front) {
    this->front = glm::normalize(front);
}

void Camera::setUp(const glm::vec3& up) {
    this->worldUp = glm::normalize(up);
    updateCameraVectors();
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getFront() const {
    return front;
}

glm::vec3 Camera::getUp() const {
    return up;
}

// Kameranın baktığı yerden bir görünüm olusturma
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio, float fov, float nearPlane, float farPlane) const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::processMouseScroll(float yoffset) {
    fov -= yoffset;
    fov = std::clamp(fov, 1.0f, 45.0f);
}

void Camera::moveForward(float distance) {
    position += front * distance;
}

void Camera::moveBackward(float distance) {
    position -= front * distance;
}

void Camera::moveLeft(float distance) {
    position -= right * distance;
}

void Camera::moveRight(float distance) {
    position += right * distance;
}

void Camera::moveUp(float distance) {
    position += worldUp * distance;
}

void Camera::moveDown(float distance) {
    position -= worldUp * distance;
}

// Kamera yönlerini güncellemek icin
void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::setFOV(float fov) {
    this->fov = fov;
}

float Camera::getFOV() const {
    return fov;
}

void Camera::setCameraDistance(float distance) {
    this->cameraDistance = distance;
}

float Camera::getCameraDistance() const {
    return cameraDistance;
}

void Camera::setCameraHeight(float height) {
    this->cameraHeight = height;
}

float Camera::getCameraHeight() const {
    return cameraHeight;
}

// Kameranın robotu takip etmesi saglanır
void Camera::updateCameraWithRobot(const glm::vec3& robotPosition, float robotRotation) {
    float offsetX = sin(glm::radians(robotRotation)) * cameraDistance;
    float offsetZ = cos(glm::radians(robotRotation)) * cameraDistance;
    glm::vec3 newCameraPos = glm::vec3(
        robotPosition.x - offsetX,
        robotPosition.y + cameraHeight,
        robotPosition.z - offsetZ
    );
    setPosition(newCameraPos);
    glm::vec3 newCameraFront = glm::normalize(robotPosition - newCameraPos);
    setFront(newCameraFront);
}
