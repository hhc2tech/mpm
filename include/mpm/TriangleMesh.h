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

#ifndef MPM_TRIANGLE_MESH_H
#define MPM_TRIANGLE_MESH_H

#include <mpm/Global.h>

#include <vector>
#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

MPM_NAMESPACE_BEGIN

class TriangleMesh {
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<size_t> vertexIndices;
    std::vector<size_t> normalIndices;
    std::vector<size_t> textureIndices;

public:
    static TriangleMesh * loadMesh(const std::string &);

    TriangleMesh();
    TriangleMesh(const TriangleMesh &);
    TriangleMesh(size_t, size_t, size_t = 0, size_t = 0);
    TriangleMesh(
        const std::vector<glm::vec3> &,
        const std::vector<size_t> &);
    TriangleMesh(
        const std::vector<glm::vec3> &,
        const std::vector<glm::vec3> &,
        const std::vector<size_t> &,
        const std::vector<size_t> &);
    TriangleMesh(
        const std::vector<glm::vec3> &,
        const std::vector<glm::vec3> &,
        const std::vector<glm::vec2> &,
        const std::vector<size_t> &,
        const std::vector<size_t> &,
        const std::vector<size_t> &);
    ~TriangleMesh();

    TriangleMesh & setVertices(const std::vector<glm::vec3> &);
    TriangleMesh & setNormals(const std::vector<glm::vec3> &);
    TriangleMesh & setTextureCoordinates(const std::vector<glm::vec2> &);
    TriangleMesh & setVertexIndices(const std::vector<size_t> &);
    TriangleMesh & setNormalIndices(const std::vector<size_t> &);
    TriangleMesh & setTextureIndices(const std::vector<size_t> &);
    TriangleMesh & setVertex(size_t, const glm::vec3 &);
    TriangleMesh & setNormal(size_t, const glm::vec3 &);
    TriangleMesh & setTextureCoordinates(size_t, const glm::vec2 &);
    TriangleMesh & setVertexIndices(size_t, size_t, size_t, size_t);
    TriangleMesh & setNormalIndices(size_t, size_t, size_t, size_t);
    TriangleMesh & setTextureIndices(size_t, size_t, size_t, size_t);
    const std::vector<glm::vec3> & getVertices() const;
    const std::vector<glm::vec3> & getNormals() const;
    const std::vector<glm::vec2> & getTextureCoordinates() const;
    const std::vector<size_t> & getVertexIndices() const;
    const std::vector<size_t> & getNormalIndices() const;
    const std::vector<size_t> & getTextureIndices() const;
    const glm::vec3 & getVertex(size_t i) const;
    const glm::vec3 & getNormal(size_t i) const;
    const glm::vec2 & getTextureCoordinates(size_t i) const;
    void getVertexIndices(size_t, size_t &, size_t &, size_t &) const;
    void getNormalIndices(size_t, size_t &, size_t &, size_t &) const;
    void getTextureIndices(size_t, size_t &, size_t &, size_t &) const;
    size_t getVertexCount() const;
    size_t getNormalCount() const;
    size_t getTextureCoordinateCount() const;
    size_t getTriangleCount() const;
    bool hasNormals() const;
    bool hasTextureCoordinates() const;

    TriangleMesh & create(const TriangleMesh &);
    TriangleMesh & create(size_t, size_t, size_t = 0, size_t = 0);
    TriangleMesh & create(
        const std::vector<glm::vec3> &,
        const std::vector<size_t> &);
    TriangleMesh & create(
        const std::vector<glm::vec3> &,
        const std::vector<glm::vec3> &,
        const std::vector<size_t> &,
        const std::vector<size_t> &);
    TriangleMesh & create(
        const std::vector<glm::vec3> &,
        const std::vector<glm::vec3> &,
        const std::vector<glm::vec2> &,
        const std::vector<size_t> &,
        const std::vector<size_t> &,
        const std::vector<size_t> &);

    TriangleMesh & transform(const glm::mat4 &);
};

MPM_NAMESPACE_END

#endif