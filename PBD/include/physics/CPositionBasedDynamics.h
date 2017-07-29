//
// Created by labuser on 7/25/17.
//

#ifndef CPOSITIONBASEDDYNAMICS_H
#define CPOSITIONBASEDDYNAMICS_H

#include <memory>
#include <vector>
#include <chrono>
#include <iostream>
#include <CVector3.hpp>
#include <physics/CParticle.hpp>
#include <physics/CParticleSystem.h>
#include <physics/CConstraint.hpp>
#include <physics/CWorld.h>


namespace PBD
{

//void getIJFromIdx(size_t idx, const std::vector<size_t> &layout, size_t &i, size_t &j)
//{
//    size_t lidx = 0;
//    while(idx > layout[lidx])
//    {
//        idx -= layout[lidx];
//        lidx++;
//    }
//
//    i=lidx;
//    j=idx;
//}
//
//bool collision(CParticle<>* p1, CParticle<>* p2)
//{
////    vec3::Vector3<> pos1 = p1->getPosition();
////    vec3::Vector3<> pos2 = p2->getPosition();
////    vec3::Vector3<> diff = pos1 - pos2;
//    //if (p1->getGroup() == p2->getGroup()) return false;
//
//    double distance = (p1->getPosition() - p2->getPosition()).norm();
//    double partSize = (p1->getSize()+p2->getSize())*0.5;
//    return distance+0.00001 <= partSize;
//}
//
//
//template<typename T_real=double>
//void generateInternalConstraints(
//        std::vector< CParticleSystem<>::Ptr > &   particleSystems,
//        std::vector< CConstraint<>::Ptr >    &   constraints
//)
//{
//    for (const auto & pS:particleSystems)
//    {
//        for (const auto & iConst:pS->m_internalConstraints)
//        {
//            constraints.push_back(iConst);
//        }
//    }
//}
//
//
//template<typename T_real=double>
//void generateCollisionConstraints(
//        std::vector< CParticleSystem<>::Ptr > &   particleSystems,
//        std::vector< CConstraint<>::Ptr >    &   constraints
//)
//{
//
//    //TODO: Broad phase (use particle system bounding boxes)
//
//
//    //Narrow phase (detect collisions between particle systems that might be in contact)
//    //Create non-penetration constraints among each penetrating point-pair
//    std::vector< size_t > numParticles(particleSystems.size());
//    size_t totalParticles = 0;
//    for (uint i=0; i<particleSystems.size(); ++i)
//    {
//        numParticles[i] = particleSystems[i]->m_particles.size();
//        totalParticles += numParticles[i];
//    }
//
//    for (uint i=0; i<totalParticles; ++i)
//    {
//        for(uint j=i+1; j<totalParticles; ++j)
//        {
//            size_t p1_i,p1_j;
//            size_t p2_i,p2_j;
//
//            //Derreference particle number i to its actual particleSystem k and position l inside the particle system
//            getIJFromIdx(i,numParticles,p1_i,p1_j);
//            getIJFromIdx(j,numParticles,p2_i,p2_j);
//
//            //Obtain the pointer to the indexed particles
//            CParticle<>* p1 = &(particleSystems[p1_i]->m_particles[p1_j]);
//            CParticle<>* p2 = &(particleSystems[p2_i]->m_particles[p2_j]);
//
//            //Create a non-penetration constraint if the particles are in contact
//            if (collision(p1,p2))
//            {
//                p1->setCollision(true);
//                p2->setCollision(true);
//                constraints.emplace_back( CConstraint<>::Ptr( new CNoPenetrationConstraint<T_real>(p1,p2) ) );
//            }
//        }
//    }
//
//}
//
//template<typename T_real=double>
//bool projectConstraints( std::vector< CConstraint<>::Ptr > & constraints)
//{
//    bool res = true;
//
//    for (auto &C:constraints)
//    {
//            res = res && C->project();
//    }
//    return res;
//}
//
//
//
//template<typename T_real=double>
//void positionBasedDynamicsStep(
//        std::vector< CParticleSystem<>::Ptr >   &   particleSystems,
//        const T_real                            &   timeStep,
//        const T_real                            &   timeout
//)
//{
//    auto start = std::chrono::high_resolution_clock::now();
//    auto elapsed = std::chrono::high_resolution_clock::now() - start;
//    T_real elapsed_seconds = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()) / T_real(1000.0);
//
//    //Update velocity and position for each particle system
//    for (uint p=0; p<particleSystems.size(); ++p)
//    {
//        particleSystems[p]->symplecticEulerUpdate(timeStep);
//        particleSystems[p]->clearExtForces();
//    }
//
//    std::vector< CConstraint<>::Ptr > constraints;
//
//    generateCollisionConstraints(particleSystems , constraints);
//    //generateInternalConstraints (particleSystems , constraints);
//
//    std::cout << "Generated " << constraints.size() << " collision constraints" << std::endl;
//    for (uint p=0; p<particleSystems.size(); ++p)
//        std::cout << "Generated " << particleSystems[p]->m_internalConstraints.size() << " internal constraints" << std::endl;
//
//
//    bool constraintsOK = false;
//    while(elapsed_seconds < timeout && !constraintsOK)
//    {
//        constraintsOK = projectConstraints(constraints);
//        for (uint p=0; p<particleSystems.size(); ++p)
//        {
//            constraintsOK = constraintsOK && projectConstraints(particleSystems[p]->m_internalConstraints);
//        }
//        elapsed = std::chrono::high_resolution_clock::now() - start;
//        elapsed_seconds = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()) / T_real(1000.0);
//    }
//
//    for (uint p=0; p<particleSystems.size(); ++p)
//    {
//        particleSystems[p]->updatePositionsWithPredPositions(timeStep);
//    }
//}

template<typename T_real=double>
void createParticleSystemSolidCube(
        const vec3::Vector3<T_real>& pos,
        const vec3::Vector3<T_real>& dim,
        PBD::CWorld* pWorld,
        T_real partSize=T_real(0.05),
        T_real partWeigth=T_real(0.01),
        size_t partGroup=0)
{
    T_real epsilon = 0.001;

    size_t partIdxIni = pWorld->m_particles.size();

    for(double i=0; i<dim(0); i+=partSize+epsilon )
    {
        for(double j=0; j<dim(1); j+=partSize+epsilon )
        {
            for (double k = 0; k < dim(2); k+= partSize+epsilon)
            {
                pWorld->m_particles.emplace_back( PBD::CParticle<>::Ptr(
                        new PBD::CParticle<T_real>(i+pos(0),j+pos(1),k+pos(2),partWeigth,partSize,partGroup)));
            }
        }
    }

    size_t partIdxEnd = pWorld->m_particles.size()-1;

    //Add distance constraints pairwise
    if (partWeigth != 0)
    {
        for (uint i=partIdxIni; i<partIdxEnd ; ++i)
        {
            for (uint j=i+1; j<partIdxEnd; ++j)
            {
                CParticle<>* p1 = pWorld->m_particles[i].get();
                CParticle<>* p2 = pWorld->m_particles[j].get();

                pWorld->m_permanentConstraints.emplace_back(
                        CConstantDistanceConstraint<>::Ptr( new CConstantDistanceConstraint<T_real>(p1,p2)
                        )
                );
            }
        }
    }

}

} //namespace PBD

#endif //CPOSITIONBASEDDYNAMICS_H
