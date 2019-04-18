// Copyright (c) 2019, Danilo Peixoto and Heitor Toledo. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <mpm/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

MPM_NAMESPACE_BEGIN

Camera::Camera() {}
Camera::~Camera() {}

Camera & Camera::perspective(
    float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    this->fieldOfView = fieldOfView;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    projectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);

    return *this;
}
Camera & Camera::orthographic(
    float leftPlane, float rightPlane, float bottomPlane, float topPlane,
    float nearPlane, float farPlane) {
    this->leftPlane = leftPlane;
    this->rightPlane = rightPlane;
    this->bottomPlane = bottomPlane;
    this->topPlane = topPlane;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    projectionMatrix = glm::ortho(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

    return *this;
}

Camera & Camera::defaultView() {
    translation.x = 0;
    translation.y = 0;
    translation.z = 0;

    rotation.x = 0;
    rotation.y = 0;

    return *this;
}
Camera & Camera::move(float dx, float dy, float dz) {
    translation.x += dx;
    translation.y += dy;
    translation.z += dz;

    return *this;
}
Camera & Camera::rotate(float ax, float ay) {
    rotation.x += ax;
    rotation.y += ay;

    return *this;
}

Camera & Camera::update() {
    static const glm::mat4 identity(1.0);
    static const glm::vec3 yAxis(0.0, 1.0, 0.0);

    glm::mat4 translationMatrix = glm::translate(identity, translation);
    glm::mat4 defaultViewMatrix = glm::lookAt(
        glm::vec3(28.0, 21.0, 28.0),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1.0, 0));
    glm::mat4 rotationYMatrix = glm::rotate(identity, rotation.x, yAxis);

    viewMatrix = translationMatrix * defaultViewMatrix * rotationYMatrix;

    glm::vec3 axis = glm::cross(yAxis, glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));
    glm::mat4 rotationAxis = glm::rotate(identity, rotation.y, axis);

    viewMatrix *= rotationAxis;
    viewProjectionMatrix = projectionMatrix * viewMatrix;

    return *this;
}

Camera & Camera::setFieldOfView(float fieldOfView) {
    this->fieldOfView = fieldOfView;
    return *this;
}
Camera & Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    return *this;
}
Camera & Camera::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    return *this;
}
Camera & Camera::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    return *this;
}
Camera & Camera::setLeftPlane(float leftPlane) {
    this->leftPlane = leftPlane;
    return *this;
}
Camera & Camera::setRightPlane(float rightPlane) {
    this->rightPlane = rightPlane;
    return *this;
}
Camera & Camera::setBottomPlane(float bottomPlane) {
    this->bottomPlane = bottomPlane;
    return *this;
}
Camera & Camera::setTopPlane(float topPlane) {
    this->topPlane = topPlane;
    return *this;
}

float Camera::getFieldOfView() const {
    return fieldOfView;
}
float Camera::getAspectRatio() const {
    return aspectRatio;
}
float Camera::getNearPlane() const {
    return nearPlane;
}
float Camera::getFarPlane() const {
    return farPlane;
}
float Camera::getLeftPlane() const {
    return leftPlane;
}
float Camera::getRightPlane() const {
    return bottomPlane;
}
float Camera::getBottomPlane() const {
    return bottomPlane;
}
float Camera::getTopPlane() const {
    return topPlane;
}

const glm::mat4 & Camera::getProjectionMatrix() const {
    return projectionMatrix;
}
const glm::mat4 & Camera::getViewMatrix() const {
    return viewMatrix;
}
const glm::mat4 & Camera::getViewProjectionMatrix() const {
    return viewProjectionMatrix;
}

MPM_NAMESPACE_END