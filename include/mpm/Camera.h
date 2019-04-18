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

#ifndef MPM_CAMERA_H
#define MPM_CAMERA_H

#include <mpm/Global.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

MPM_NAMESPACE_BEGIN

class Camera {
public:
    Camera();
    ~Camera();

    Camera & perspective(float, float, float, float);
    Camera & orthographic(float, float, float, float, float, float);

    Camera & defaultView();
    Camera & move(float, float, float);
    Camera & rotate(float, float);

    Camera & update();

    Camera & setFieldOfView(float);
    Camera & setAspectRatio(float);
    Camera & setNearPlane(float);
    Camera & setFarPlane(float);
    Camera & setLeftPlane(float);
    Camera & setRightPlane(float);
    Camera & setBottomPlane(float);
    Camera & setTopPlane(float);

    float getFieldOfView() const;
    float getAspectRatio() const;
    float getNearPlane() const;
    float getFarPlane() const;
    float getLeftPlane() const;
    float getRightPlane() const;
    float getBottomPlane() const;
    float getTopPlane() const;

    const glm::mat4 & getProjectionMatrix() const;
    const glm::mat4 & getViewMatrix() const;
    const glm::mat4 & getViewProjectionMatrix() const;

private:
    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float leftPlane;
    float rightPlane;
    float bottomPlane;
    float topPlane;

    glm::vec3 translation;
    glm::vec2 rotation;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 viewProjectionMatrix;
};

MPM_NAMESPACE_END

#endif