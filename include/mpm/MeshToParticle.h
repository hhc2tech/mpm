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

#ifndef MPM_MESH_TO_PARTICLE_H
#define MPM_MESH_TO_PARTICLE_H

#include <mpm/Global.h>
#include <mpm/TriangleMesh.h>

#include <glm/vec3.hpp>

#include <openvdb/openvdb.h>

#include <vector>
#include <random>

MPM_NAMESPACE_BEGIN

using RandomGenerator = std::mersenne_twister_engine<uint32_t, 32, 351, 175, 19,
    0xccab8ee7, 11, 0xffffffff, 7, 0x31b6ab00, 15, 0xffe50000, 17, 1812433253>;

class MeshDataAdapter {
public:
    MeshDataAdapter(TriangleMesh *, float);

    size_t pointCount() const;
    size_t polygonCount() const;
    size_t vertexCount(size_t) const;

    void getIndexSpacePoint(size_t, size_t, openvdb::math::Vec3d &) const;

    TriangleMesh * getMesh();
    const openvdb::math::Transform & getTransform() const;

private:
    TriangleMesh * mesh;
    openvdb::math::Transform::Ptr transform;
};

class Material {
public:
    Material(const glm::vec3 &, float, float, float);

    const glm::vec3 & getVelocity() const;
    float getMass() const;
    float getLambda() const;
    float getMu() const;

private:
    glm::vec3 velocity;
    float mass;
    float lambda;
    float mu;
};

class Particle {
public:
    Particle();
    Particle(const Material &);

    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    float volume;
    float lambda;
    float mu;

    Particle & apply(const Material &);
};

typedef std::vector<Particle *> ParticlePointerArray;

class MeshToParticle {
public:
    MeshToParticle(TriangleMesh *, const Material &, float, float, float, size_t);
    ~MeshToParticle();

    ParticlePointerArray & getParticles();
    void add(const openvdb::Vec3d &);

private:
    ParticlePointerArray particles;
};

MPM_NAMESPACE_END

#endif