//
// Created by labuser on 7/25/17.
//

#ifndef CPOSITIONBASEDDYNAMICS_H
#define CPOSITIONBASEDDYNAMICS_H

#include <memory>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <CVector3.hpp>
#include <physics/CParticle.hpp>
#include <physics/CParticleSystem.h>
#include <physics/CConstraint.hpp>
#include <physics/CWorld.h>
#include <sstream>

#include <Common.h>
#define _HCD_ENABLE_SAFE_CHECKS_
#include <Octree.hpp>
#include <PointSetNode.hpp>



namespace PBD
{


template<typename T_real=double>
void addParticleSystemInternalConstraints(
        PBD::CWorld* pWorld,
        const size_t& partIdxIni,
        const size_t& partIdxEnd
)
{

    //USING SHAPE MATCHING
    std::vector< CParticle<>* > particles;
    for (size_t i=partIdxIni; i<partIdxEnd ; ++i) {
        particles.push_back(pWorld->m_particles[i].get());
    }
    pWorld->m_shapeMatchingConstraints.emplace_back( PBD::CShapeMatchingConstraint<>::Ptr(
            new PBD::CShapeMatchingConstraint<>(particles)
    ));

    //USING DISTANCE CONSTRAINTS
//    for (size_t i=partIdxIni; i<partIdxEnd ; ++i)
//    {
//        for (size_t j=i; j<partIdxEnd; ++j)
//        {
//            CParticle<>* p1 = pWorld->m_particles[i].get();
//            CParticle<>* p2 = pWorld->m_particles[j].get();
//            T_real distance = p1->m_size + p1->m_size + PBD::constraintEpsilon;
//
//            if( (p1->m_position - p2->m_position).norm() <= distance )
//
//                pWorld->m_permanentConstraints.emplace_back(
//                        CConstantDistanceConstraint<>::Ptr(
//                                new CConstantDistanceConstraint<T_real>(p1,p2)
//                        )
//                );
//        }
//    }



}



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

    size_t partIdxEnd = pWorld->m_particles.size();

    //Add internal distance constraints to mantain structure
    if (partWeigth != 0)
    {
        addParticleSystemInternalConstraints(pWorld, partIdxIni, partIdxEnd);
    }

}

template<typename T_real=double>
void createParticleSystemSolidSphere(
        const vec3::Vector3<T_real>& pos,
        const T_real& radius,
        PBD::CWorld* pWorld,
        T_real partSize=T_real(0.05),
        T_real partWeigth=T_real(0.01),
        size_t partGroup=0)
{
    T_real epsilon = 0.001;

    size_t partIdxIni = pWorld->m_particles.size();

    for(double i=-radius; i<radius; i+=partSize+epsilon )
    {
        for(double j=-radius; j<radius; j+=partSize+epsilon )
        {
            for (double k =-radius; k < radius; k+= partSize+epsilon)
            {
                if (std::sqrt(i*i+j*j+k*k) <= radius)
                pWorld->m_particles.emplace_back( PBD::CParticle<>::Ptr(
                        new PBD::CParticle<T_real>(i+pos(0),j+pos(1),k+pos(2),partWeigth,partSize,partGroup)));
            }
        }
    }


    size_t partIdxEnd = pWorld->m_particles.size();

    //Add internal distance constraints to mantain structure
    if (partWeigth != 0)
    {
        addParticleSystemInternalConstraints(pWorld, partIdxIni, partIdxEnd);
    }

}



template<typename T_real=double>
void createParticleSystemFromASCIIXYZPointCloud(
        const vec3::Vector3<T_real>& pos,
        const std::string& filename,
        PBD::CWorld* pWorld,
        T_real partSize=T_real(0.05),
        T_real partWeigth=T_real(0.01),
        size_t partGroup=0,
        T_real scale = T_real(1.0))
{
    std::ifstream infile(filename);
    if ( !infile.is_open() )
    {
        return;
    }

    std::cout << "Creating particle system from file: " << filename << std::endl;

    size_t partIdxIni = pWorld->m_particles.size();
    std::string line;
    HCD::TOctree< T_real , HCD::TPointSetNode< T_real > , T_real > Octree;
    while ( std::getline(infile, line)  )
    {
        std::stringstream lineStream;
        lineStream << line;
        T_real x,y,z;
        lineStream >> x;
        lineStream >> y;
        lineStream >> z;
        //TODO: Solid voxelization. Add internal voxels to the tree.
        //_GENERIC_DEBUG_("Add particle [" + std::to_string(x) + " , "+ std::to_string(y) + " , "+ std::to_string(z) + "] ");
        //Construct and octree with the particle size
        HCD::TVector3D<T_real> point (x * scale,y * scale,z * scale);
        Octree.Insert(point, partSize);
        //Octree.Insert(x,y,z, partSize);   //TODO: Tell David this does not work in the HCD version I have
    }

    std::deque< HCD::TOctreeNode< T_real , HCD::TPointSetNode< T_real > >* > Nodes = Octree.GetNodes();

    _GENERIC_DEBUG_("Octree nodes loaded: " + std::to_string(Nodes.size()));


    for (auto pOctreeNode : Nodes)
    {
        if (pOctreeNode->IsOuter())
        {
            T_real x = pOctreeNode->GetCenter().m_X;
            T_real y = pOctreeNode->GetCenter().m_Y;
            T_real z = pOctreeNode->GetCenter().m_Z;

            pWorld->m_particles.emplace_back(
                    PBD::CParticle<>::Ptr(
                            new PBD::CParticle<T_real>(x+pos(0),y+pos(1),z+pos(2),partWeigth,partSize,partGroup)
                    )
            );
        }
    }

    size_t partIdxEnd = pWorld->m_particles.size();

    std::cout << "Loaded " << partIdxEnd -partIdxIni << " particles" << std::endl;

    //Add internal distance constraints to mantain structure
    if (partWeigth != 0)
    {
        addParticleSystemInternalConstraints(pWorld, partIdxIni, partIdxEnd);
    }

    std::cout << "Created constraints." << std::endl;

}



} //namespace PBD

#endif //CPOSITIONBASEDDYNAMICS_H
