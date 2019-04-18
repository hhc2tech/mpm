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

#ifndef MPM_VIEWER_H
#define MPM_VIEWER_H

#include <mpm/Global.h>
#include <mpm/Camera.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

MPM_NAMESPACE_BEGIN

class Viewer {
private:
    struct MouseState {
        int button;
        double x, y;
    } mouseState;

    struct Object {
        GLuint vao, vbo, program, texture;
        Object & reset();
    };

    Object gridObject;
    Object axesObject;

    std::string title;
    size_t width;
    size_t height;
    bool grid;

    GLFWwindow * window;
    Camera camera;

    Viewer & initialize();
    Viewer & render();

    Viewer & clearBuffer();

    bool loadShader(const std::string &, GLenum, GLuint &) const;
    bool createShader(const std::string &, bool, Object &);
    Viewer & deleteShader(Object &);

    Viewer & createAxes(float);
    Viewer & deleteAxes();
    Viewer & renderAxes();

    Viewer & createGrid(size_t);
    Viewer & deleteGrid();
    Viewer & renderGrid();

    static void resize(GLFWwindow *, int, int);
    static void keyboard(GLFWwindow *, int, int, int, int);
    static void mouseButton(GLFWwindow *, int, int, int);
    static void cursor(GLFWwindow *, double, double);
    static void scroll(GLFWwindow *, double, double);

public:
    Viewer();
    Viewer(const std::string &, size_t, size_t);
    ~Viewer();

    Viewer & setTitle(const std::string &);
    Viewer & setHeight(size_t);
    Viewer & setWidth(size_t);
    Viewer & setGrid(bool);

    const std::string & getTitle() const;
    size_t getHeight() const;
    size_t getWidth() const;
    bool getGrid() const;

    Viewer & show();
    Viewer & close();
};

MPM_NAMESPACE_END

#endif