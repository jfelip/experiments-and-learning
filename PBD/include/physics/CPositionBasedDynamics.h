//
// Created by labuser on 7/25/17.
//

#ifndef CPOSITIONBASEDDYNAMICS_H
#define CPOSITIONBASEDDYNAMICS_H

#include <memory>
#include <vector>
#include <CVector3.hpp>
#include <physics/CParticle.hpp>
#include <physics/CParticleSystem.h>


namespace PBD
{

template<typename T_real=double>
class CConstraint
{
public:
    typedef std::shared_ptr<CConstraint> Ptr;

    typedef const std::shared_ptr<CConstraint> ConstPtr;

public:
    CConstraint(){}

    ~CConstraint(){}

protected:

};


//TODO: Implement this
template<typename T_real=double>
void generateCollisionConstraints(
        std::vector< std::vector<CParticle> >&      particleSystems,
        std::vector< COpenGLGeometry::ConstPtr >&   geom,
        std::vector<CConstraint::ConstPtr>&         constraints
)
{

}

//TODO: Implement this
template<typename T_real=double>
void projectConstraints(
        std::vector< std::vector<CParticle> >&      particleSystems,
        std::vector<CConstraint::ConstPtr>&         constraints
)
{

}



template<typename T_real=double>
void positionBasedDynamicsStep(
        std::vector< std::vector<CParticle> >&      particleSystems,
        std::vector< COpenGLGeometry::ConstPtr >&   geom,
        const T_real&                               timeStep
)
{
    //Update velocity and position for each particle system
    for (auto p:particleSystems)
    {
        p.symplecticEulerUpdate(timeStep);
    }

    std::vector<CConstraint::ConstPtr> collConstraints;
    generateCollisionConstraints(particleSystems,geom,collConstraints);

    while(!timeout)
    {
        projectConstraints(particleSystems, collConstraints);
    }

    for (auto p:particleSystems)
    {
        p.updatePositionsWithPredPositions();
        p.updateVelocities();
    }

}


} //namespace PBD

#endif //CPOSITIONBASEDDYNAMICS_H
