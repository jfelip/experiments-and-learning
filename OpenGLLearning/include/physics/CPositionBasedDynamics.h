//
// Created by labuser on 7/25/17.
//

#ifndef CPOSITIONBASEDDYNAMICS_H
#define CPOSITIONBASEDDYNAMICS_H

#include <CVector3.hpp>


namespace PBD
{

template<typename T_real=double>
class CParticle
{
public:
    typedef std::shared_ptr<CParticle> Ptr;

    typedef const std::shared_ptr<CParticle> ConstPtr;

public:
    CParticle(){}

    ~CParticle(){}

    vec3::Vector3& getPosition(){return m_position;}

    vec3::Vector3& getPredPosition(){return m_predPosition;}

    vec3::Vector3& getVelocity(){return m_velocity;}

    T_real getMass(){return m_mass;}

    T_real getMassInv(){return m_massInv;}

    void setMass(T_real m){m_mass = m; m_massInv = 1 / m_mass;}

    T_real getSize(){return m_size;}

    void setSize(T_real s){m_size = s;}

protected:
    vec3::Vector3 m_position;
    vec3::Vector3 m_predPosition;
    vec3::Vector3 m_velocity;
    T_real m_mass;
    T_real m_massInv;
    T_real m_size;
};

template<typename T_real=double>
class CParticleSystem
{
public:
    typedef std::shared_ptr<CParticleSystem> Ptr;

    typedef const std::shared_ptr<CParticleSystem> ConstPtr;

public:
    CParticleSystem(){}

    ~CParticleSystem(){}

    std::vector< CParticle >& getParticles(){ return m_particles; }

    void setParticles( std::vector< CParticle >& p) { m_particles = p; }

    T_real getMass(){return m_mass;}

    T_real getMassInv(){return m_massInv;}

    void setMass(T_real m){m_mass = m; m_massInv = 1 / m_mass;}

    T_real getSize(){return m_size;}

    void setSize(T_real s){m_size = s;}

    //TODO: Implement this
    void symplecticEulerUpdate(T_real timeStep);

    //TODO: Implement this
    void updatePositionsWithPredPositions();

    //TODO: Implement this
    void updateVelocities();


protected:
    std::vector< CParticle > m_particles;        ///< Current state of the particle system.
    std::vector< CParticle > m_initialParticles; ///< This particle configuration is the resting position for the particle system.
    T_real m_size;
    T_real m_mass;
    T_real m_massInv;
};

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
std::vector<CConstraint::ConstPtr>& generateCollisionConstraints(
        std::vector< std::vector<CParticle> >&      particleSystems,
        std::vector< COpenGLGeometry::ConstPtr >&   geom
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

    std::vector<CConstraint::ConstPtr> collConstraints = generateCollisionConstraints(particleSystems,geom);

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
