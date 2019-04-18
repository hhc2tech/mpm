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

#include <mpm/MeshToParticle.h>

#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/PointScatter.h>

#include <limits>

MPM_NAMESPACE_BEGIN

MeshDataAdapter::MeshDataAdapter(TriangleMesh * mesh, float voxelSize) {
    this->mesh = mesh;
    transform = openvdb::math::Transform::createLinearTransform(voxelSize);

    for (size_t i = 0; i < mesh->getVertexCount(); i++) {
        glm::vec3 vertex = mesh->getVertex(i);
        openvdb::Vec3d transformedPoint(vertex.x, vertex.y, vertex.z);

        transformedPoint = transform->worldToIndex(transformedPoint);

        vertex.x = transformedPoint.x();
        vertex.y = transformedPoint.y();
        vertex.z = transformedPoint.z();

        mesh->setVertex(i, vertex);
    }
}

size_t MeshDataAdapter::pointCount() const {
    return mesh->getVertexCount();
}
size_t MeshDataAdapter::polygonCount() const {
    return mesh->getTriangleCount();
}
size_t MeshDataAdapter::vertexCount(size_t index) const {
    return 3;
}

void MeshDataAdapter::getIndexSpacePoint(
    size_t polygonIndex, size_t vertexIndex, openvdb::math::Vec3d & position) const {
    const std::vector<size_t> & vertexIndices = mesh->getVertexIndices();
    size_t index = vertexIndices[polygonIndex * 3 + vertexIndex];
    const glm::vec3 & vertex = mesh->getVertex(index);

    position[0] = vertex.x;
    position[1] = vertex.y;
    position[2] = vertex.z;
}

TriangleMesh * MeshDataAdapter::getMesh() {
    return mesh;
}
const openvdb::math::Transform & MeshDataAdapter::getTransform() const {
    return *transform.get();
}

Material::Material(
    const glm::vec3 & velocity, float mass, float young, float poisson) {
    this->velocity = velocity;
    this->mass = mass;

    float d = 1.0 / (1.0 + poisson);

    lambda = young * poisson / (1.0 - 2.0 * poisson) * d;
    mu = 0.5 * young * d;
}

const glm::vec3 & Material::getVelocity() const {
    return velocity;
}
float Material::getMass() const {
    return mass;
}
float Material::getLambda() const {
    return lambda;
}
float Material::getMu() const {
    return mu;
}

Particle::Particle() {}
Particle::Particle(const Material & material) {
    apply(material);
}
Particle & Particle::apply(const Material & material) {
    velocity = material.getVelocity();
    mass = material.getMass();
    lambda = material.getLambda();
    mu = material.getMu();

    return *this;
}

MeshToParticle::MeshToParticle(
    TriangleMesh * mesh, const Material & material,
    float voxelSize, float density, float spread, size_t seed) {
    MeshDataAdapter meshDataAdapter(mesh, voxelSize);

    openvdb::FloatGrid::Ptr grid = openvdb::tools::meshToVolume<openvdb::FloatGrid>
        (meshDataAdapter, meshDataAdapter.getTransform(), 1.0, std::numeric_limits<float>::max());

    RandomGenerator randomGenerator(seed);

    openvdb::tools::DenseUniformPointScatter<MeshToParticle, RandomGenerator>
        denseUniformPointScatter(*this, density * voxelSize, randomGenerator, spread);

    denseUniformPointScatter(*grid.get());
}
MeshToParticle::~MeshToParticle() {
    for (Particle * particle : particles) {
        if (particle)
            delete particle;
    }
}

ParticlePointerArray & MeshToParticle::getParticles() {
    return particles;
}

void MeshToParticle::add(const openvdb::Vec3d & point) {
    Particle * particle = new Particle();

    particle->position.x = point.x();
    particle->position.y = point.y();
    particle->position.z = point.z();

    particles.push_back(particle);
}

MPM_NAMESPACE_END