
#ifndef POSITIONBASEDDYNAMICS_CPARTICLESYSTEM_H
#define POSITIONBASEDDYNAMICS_CPARTICLESYSTEM_H

#include <physics/CParticle.hpp>
#include <memory>
#include <vector>
#include <CVector3.hpp>

namespace PBD {


    template<typename T_real=double>
    class CParticleSystem {
    public:
        typedef std::shared_ptr<CParticleSystem> Ptr;

        typedef const std::shared_ptr<CParticleSystem> ConstPtr;

    public:
        CParticleSystem() {}

        ~CParticleSystem() {}

        std::vector<CParticle> &getParticles() { return m_particles; }

        void setParticles(std::vector<CParticle> &p) { m_particles = p; }

        T_real getMass() { return m_mass; }

        T_real getMassInv() { return m_massInv; }

        void setMass(T_real m) {
            m_mass = m;
            m_massInv = 1 / m_mass;
        }

        T_real getSize() { return m_size; }

        void setSize(T_real s) { m_size = s; }

        //TODO: Implement this
        void symplecticEulerUpdate(T_real timeStep);

        //TODO: Implement this
        void updatePositionsWithPredPositions();

        //TODO: Implement this
        void updateVelocities();


    protected:
        std::vector<CParticle> m_particles;        ///< Current state of the particle system.
        std::vector<CParticle> m_initialParticles; ///< This particle configuration is the resting position for the particle system.
        T_real m_size;
        T_real m_mass;
        T_real m_massInv;
        vec3::Vector3 m_extForce;
    };

}
#endif //POSITIONBASEDDYNAMICS_CPARTICLESYSTEM_H
