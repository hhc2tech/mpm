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

#include <mpm/Viewer.h>
#include <mpm/TriangleMesh.h>
#include <mpm/MeshToParticle.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <sstream>

MPM_NAMESPACE_BEGIN

Viewer::Object & Viewer::Object::reset() {
    vao = -1;
    vbo = -1;
    program = -1;
    texture = -1;

    return *this;
}

Viewer & Viewer::initialize() {
    glEnable(GL_DEPTH_TEST);

    camera.perspective(M_PI / 4.0, width / (float)height, 0.001, 1000.0);
    camera.defaultView();

    TriangleMesh * mesh = TriangleMesh::loadMesh("res/meshes/bunny.obj");
    Material material(glm::vec3(0, 0, 0), 0.01, 1.0e5, 0.2);

    MeshToParticle particleGenerator(mesh, material, 0.1, 1.0, 1.0, 0);
    std::vector<Particle *> & particles = particleGenerator.getParticles();

    delete mesh;

    if (createShader("res/shaders/grid", false, gridObject)) {
        glUseProgram(gridObject.program);
        glUniform3f(glGetUniformLocation(gridObject.program, "color"), 0.25, 0.25, 0.25);
    }

    createShader("res/shaders/axes", false, axesObject);

    createAxes(1.0);
    createGrid(10);

    return *this;
}
Viewer & Viewer::render() {
    camera.update();

    clearBuffer();

    if (grid)
        renderGrid();

    renderAxes();

    return *this;
}

Viewer & Viewer::clearBuffer() {
    glClearDepth(1.0);
    glClearColor(0.35, 0.35, 0.35, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return *this;
}

bool Viewer::loadShader(const std::string & path, GLenum type, GLuint & id) const {
    std::ifstream file(path);

    if (!file.is_open())
        return false;

    std::stringstream buffer;
    std::string source;

    buffer << file.rdbuf();
    source = buffer.str();

    file.close();

    const char * code = source.c_str();
    GLuint shaderID = glCreateShader(type);

    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    int status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (!status)
        return false;

    id = shaderID;

    return true;
}
bool Viewer::createShader(
    const std::string & shader, bool geometry, Object & object) {
    GLuint geometryShader, vertexShader, fragmentShader;

    if (!loadShader(shader + ".vert", GL_VERTEX_SHADER, vertexShader) ||
        !loadShader(shader + ".frag", GL_FRAGMENT_SHADER, fragmentShader))
        return false;

    if (geometry && !loadShader(shader + ".geom", GL_GEOMETRY_SHADER, geometryShader))
        return false;

    object.program = glCreateProgram();

    glAttachShader(object.program, vertexShader);
    glAttachShader(object.program, fragmentShader);

    if (geometry)
        glAttachShader(object.program, geometryShader);

    glLinkProgram(object.program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (geometry)
        glDeleteShader(geometryShader);

    int status;
    glGetProgramiv(object.program, GL_LINK_STATUS, &status);

    return status;
}
Viewer & Viewer::deleteShader(Object & object) {
    glDeleteProgram(object.program);
    object.program = 0;

    return *this;
}

Viewer & Viewer::createAxes(float length) {
    float vertices[] = {
        0,      0,      0,      1.0, 0,   0,
        length, 0,      0,      1.0, 0,   0,
        0,      0,      0,      0,   1.0, 0,
        0,      length, 0,      0,   1.0, 0,
        0,      0,      0,      0,   0,   1.0,
        0,      0,      length, 0,   0,   1.0
    };

    glGenVertexArrays(1, &axesObject.vao);
    glBindVertexArray(axesObject.vao);

    glGenBuffers(1, &axesObject.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, axesObject.vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return *this;
}
Viewer & Viewer::deleteAxes() {
    glDeleteBuffers(1, &axesObject.vbo);
    glDeleteVertexArrays(1, &axesObject.vao);

    return *this;
}
Viewer & Viewer::renderAxes() {
    glDisable(GL_DEPTH_TEST);
    glViewport(-10, -10, 80, 80);

    glBindVertexArray(axesObject.vao);
    glUseProgram(axesObject.program);

    glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
    glm::mat4 viewMatrix = camera.getViewMatrix();

    viewMatrix[3][0] = 0;
    viewMatrix[3][1] = 0;
    viewMatrix[3][2] = 0;

    glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    glUniformMatrix4fv(
        glGetUniformLocation(axesObject.program, "modelViewProjection"),
        1,
        GL_FALSE,
        glm::value_ptr(viewProjectionMatrix));

    glLineWidth(2.0);
    glDrawArrays(GL_LINES, 0, 6);
    glLineWidth(1.0);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    return *this;
}

Viewer & Viewer::createGrid(size_t size) {
    std::vector<float> vertices;
    vertices.reserve(8 * (2 * size + 1));

    float right = size;
    float left = -right;

    for (float t = left; t <= right; t++) {
        if (std::abs(t) < MPM_EPS)
            continue;

        vertices.push_back(t);
        vertices.push_back(left);
        vertices.push_back(t);
        vertices.push_back(right);

        vertices.push_back(left);
        vertices.push_back(t);
        vertices.push_back(right);
        vertices.push_back(t);
    }

    vertices.push_back(0);
    vertices.push_back(left);
    vertices.push_back(0);
    vertices.push_back(right);

    vertices.push_back(left);
    vertices.push_back(0);
    vertices.push_back(right);
    vertices.push_back(0);

    glGenVertexArrays(1, &gridObject.vao);
    glBindVertexArray(gridObject.vao);

    glGenBuffers(1, &gridObject.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gridObject.vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(0);

    return *this;
}
Viewer & Viewer::deleteGrid() {
    glDeleteBuffers(1, &gridObject.vbo);
    glDeleteVertexArrays(1, &gridObject.vao);

    return *this;
}
Viewer & Viewer::renderGrid() {
    glBindVertexArray(gridObject.vao);
    glUseProgram(gridObject.program);

    glUniformMatrix4fv(
        glGetUniformLocation(gridObject.program, "modelViewProjection"),
        1,
        GL_FALSE,
        glm::value_ptr(camera.getViewProjectionMatrix()));

    int size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    size /= 2 * sizeof(float);

    int middle = size - 4;

    glLineWidth(1.0);
    glDrawArrays(GL_LINES, 0, middle);

    glLineWidth(2.0);
    glDrawArrays(GL_LINES, middle, size);

    glLineWidth(1.0);

    return *this;
}

void Viewer::resize(GLFWwindow * window, int width, int height) {
    Viewer * viewer = (Viewer *)glfwGetWindowUserPointer(window);
    Camera & camera = viewer->camera;

    viewer->width = width;
    viewer->height = height;

    glViewport(0, 0, width, height);

    camera.perspective(
        camera.getFieldOfView(),
        width / (float)height,
        camera.getNearPlane(),
        camera.getFarPlane());
}
void Viewer::keyboard(GLFWwindow * window, int key, int code, int action, int modifier) {
    Viewer * viewer = (Viewer *)glfwGetWindowUserPointer(window);
    Camera & camera = viewer->camera;

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_D:
            camera.defaultView();
            break;
        case GLFW_KEY_G:
            viewer->grid = !viewer->grid;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(viewer->window, GLFW_TRUE);
            break;
        default:
            break;
        }
    }
}
void Viewer::mouseButton(GLFWwindow * window, int button, int action, int modifier) {
    Viewer * viewer = (Viewer *)glfwGetWindowUserPointer(window);
    viewer->mouseState.button = action == GLFW_PRESS ? button : -1;
}
void Viewer::cursor(GLFWwindow * window, double x, double y) {
    Viewer * viewer = (Viewer *)glfwGetWindowUserPointer(window);
    Camera & camera = viewer->camera;

    double dx = x - viewer->mouseState.x;
    double dy = y - viewer->mouseState.y;

    viewer->mouseState.x = x;
    viewer->mouseState.y = y;

    switch (viewer->mouseState.button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        camera.rotate(0.005 * dx, 0.005 * dy);
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        camera.move(0.05 * dx, -0.05 * dy, 0);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        camera.move(0, 0, 0.5 * (dx + dy));
        break;
    default:
        break;
    }
}
void Viewer::scroll(GLFWwindow * window, double dx, double dy) {
    Viewer * viewer = (Viewer *)glfwGetWindowUserPointer(window);
    Camera & camera = viewer->camera;

    camera.move(0, 0, 5.0 * (dx + dy));
}

Viewer::Viewer() : grid(true) {}
Viewer::Viewer(const std::string & title, size_t width, size_t height) {
    this->title = title;

    this->width = width;
    this->height = height;

    this->grid = true;
}
Viewer::~Viewer() {
    close();
}

Viewer & Viewer::setTitle(const std::string & title) {
    this->title = title;
    return *this;
}
Viewer & Viewer::setHeight(size_t height) {
    this->height = height;
    return *this;
}
Viewer & Viewer::setWidth(size_t width) {
    this->width = width;
    return *this;
}
Viewer & Viewer::setGrid(bool enabled) {
    this->grid = enabled;
    return *this;
}

const std::string & Viewer::getTitle() const {
    return title;
}
size_t Viewer::getHeight() const {
    return height;
}
size_t Viewer::getWidth() const {
    return width;
}
bool Viewer::getGrid() const {
    return grid;
}

Viewer & Viewer::show() {
    mouseState.button = -1;
    mouseState.x = 0;
    mouseState.y = 0;

    window = nullptr;

    axesObject.reset();
    gridObject.reset();

    if (!glfwInit())
        return *this;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    if (window == nullptr)
        glfwTerminate();
    else {
        glfwSetWindowSizeCallback(window, resize);
        glfwSetKeyCallback(window, keyboard);
        glfwSetMouseButtonCallback(window, mouseButton);
        glfwSetCursorPosCallback(window, cursor);
        glfwSetScrollCallback(window, scroll);

        glfwSetWindowUserPointer(window, this);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        GLFWmonitor * monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);

        size_t x = (mode->width - width) / 2;
        size_t y = (mode->height - height) / 2;

        glfwSetWindowPos(window, x, y);

        glewExperimental = GL_TRUE;

        if (glewInit() == GLEW_OK) {
            initialize();

            while (!glfwWindowShouldClose(window)) {
                render();

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }

        close();
    }

    return *this;
}
Viewer & Viewer::close() {
    if (window != nullptr) {
        glfwDestroyWindow(window);
        glfwTerminate();

        deleteGrid();
        deleteAxes();

        deleteShader(gridObject);
        deleteShader(axesObject);
    }

    return *this;
}

MPM_NAMESPACE_END