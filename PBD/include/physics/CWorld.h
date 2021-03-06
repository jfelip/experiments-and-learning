//
// Created by labuser on 7/28/17.
//

#ifndef PBD_CWORLD_H
#define PBD_CWORLD_H

#include <chrono>
#include <iostream>
#include <physics/CParticle.hpp>
#include <physics/CParticleSystem.h>
#include <physics/CConstraint.hpp>


//TODO: HIGH Approximate shock propagation to increase convergence of rigid stacks
//TODO: HIGH Static and dynamic friction forces
//TODO: HIGH Constraint elasticity
//TODO: HIGH Constraint plasticity
//TODO: HIGH Constraint breaking


namespace PBD
{

class CWorld
{
public:
    CWorld() = default;
    ~CWorld() = default;

    bool collision(CParticle<>* p1, CParticle<>* p2);
    void getIJFromIdx(size_t idx, const std::vector<size_t> &layout, size_t &i, size_t &j);

    void step(const double & timeStep, const double & timeout);
    void applyGravity();
    void symplecticEulerUpdate(double timeStep);
    void createCollisionConstraints();
    void clearExternalForces();
    bool gaussSeidelSolver();
    void updatePositionsWithPredPositions();
    void updateVelocities(double timeStep);



    std::vector<PBD::CParticle<>::Ptr>        m_particles;
    std::vector<PBD::CParticleSystem<>::Ptr>  m_particleSystems;
    std::vector<PBD::CConstraint<>::Ptr>      m_constraints;
    std::vector<PBD::CConstraint<>::Ptr>      m_permanentConstraints;
    std::vector<PBD::CShapeMatchingConstraint<>::Ptr>      m_shapeMatchingConstraints;
    Eigen::Vector3d m_gravity;
};

bool CWorld::collision(CParticle<>* p1, CParticle<>* p2)
{
    if (p1->m_group == p2->m_group) return false;

    double distance = (p1->m_predPosition- p2->m_predPosition).norm();
    double partSize = (p1->m_size+p2->m_size)*0.5;
    return distance <= partSize;
}

void CWorld::getIJFromIdx(size_t idx, const std::vector<size_t> &layout, size_t &i, size_t &j)
{
    size_t lidx = 0;
    while(idx > layout[lidx])
    {
        idx -= layout[lidx];
        lidx++;
    }

    i=lidx;
    j=idx;
}

void CWorld::createCollisionConstraints()
{
    //TODO: Broad phase (use particle system bounding boxes)


    //Narrow phase (detect collisions between particle systems that might be in contact)
    //Create non-penetration constraints among each penetrating point-pair
    std::vector< size_t > numParticles(m_particles.size());

    for (uint i=0; i<m_particles.size(); ++i)
    {
        for(uint j=i+1; j<m_particles.size(); ++j)
        {
            //Obtain the pointer to the indexed particles
            CParticle<>* p1 = m_particles[i].get();
            CParticle<>* p2 = m_particles[j].get();

            //Create a non-penetration constraint if the particles are in contact
            if (collision(p1,p2))
            {
                m_constraints.emplace_back( CConstraint<>::Ptr( new CNoPenetrationConstraint<>(p1,p2) ) );
            }
        }
    }
}

void CWorld::step(const double & timeStep, const double & timeout)
{
    // TIMING VARIABLES
    auto start = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    double elapsed_seconds = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()) / double(1000.0);

    // SOLVE CONTACTS FIRST TO PRE-STABILIZE
    m_constraints.clear();
    createCollisionConstraints();
    bool constraintsOK = true;
    uint i = 0;
    uint maxIter = 5;
    do
    {

        for (auto it = m_constraints.begin(); it < m_constraints.end(); ++it)
        {
            (*it)->project();
            constraintsOK = constraintsOK && (*it)->isPredSatisfied();
        }
        ++i;
    } while(elapsed_seconds < timeout && !constraintsOK && i<maxIter);
    updatePositionsWithPredPositions();

    // ADD GRAVITY AND PREDICT NEW POSITIONS
    applyGravity();
    symplecticEulerUpdate(timeStep);
    clearExternalForces();

    // SOLVE CONTACT CONSTRAINTS
    m_constraints.clear();
    createCollisionConstraints();
    constraintsOK = true;
    i = 0;
    maxIter = 5;
    do
    {

        for (auto it = m_constraints.begin(); it < m_constraints.end(); ++it)
        {
            (*it)->project();
            constraintsOK = constraintsOK && (*it)->isPredSatisfied();
        }
        ++i;
    } while(elapsed_seconds < timeout && !constraintsOK && i<maxIter);

    // SOLVE SHAPE-MATCHING CONSTRAINTS
    i = 0;
    maxIter = 5000;
    do
    {
        constraintsOK = true;
        for( auto it = m_shapeMatchingConstraints.begin(); it<m_shapeMatchingConstraints.end(); ++it)
        {
            constraintsOK = (*it)->project();
        }

        elapsed = std::chrono::high_resolution_clock::now() - start;
        elapsed_seconds = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()) / double(1000.0);
        ++i;
    } while(elapsed_seconds < timeout && !constraintsOK && i<maxIter);

    updateVelocities(timeStep);
    updatePositionsWithPredPositions();
}

void CWorld::clearExternalForces()
{
    for( auto it = m_particles.begin(); it<m_particles.end(); ++it)
    {
        (*it)->clearExtForces();
    }
}

void CWorld::symplecticEulerUpdate( double timeStep )
{
    for( auto it = m_particles.begin(); it<m_particles.end(); ++it)
    {
        (*it)->symplecticEulerUpdate(timeStep);
    }
}

void CWorld::updatePositionsWithPredPositions( )
{
    for( auto it = m_particles.begin(); it<m_particles.end(); ++it)
    {
        (*it)->updatePositionsWithPredPositions();
    }
}

void CWorld::updateVelocities( double timeStep )
{
    for( auto it = m_particles.begin(); it<m_particles.end(); ++it)
    {
        (*it)->updateVelocity(timeStep);
    }
}

void CWorld::applyGravity()
{
    for( auto it = m_particles.begin(); it<m_particles.end(); ++it)
    {
        (*it)->m_extForce += m_gravity * ( (*it)->getMass() );
    }
}


}

#endif //PBD_CWORLD_H
