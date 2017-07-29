
#ifndef POSITIONBASEDDYNAMICS_CPARTICLESYSTEM_H
#define POSITIONBASEDDYNAMICS_CPARTICLESYSTEM_H

#include <physics/CParticle.hpp>
#include <physics/CConstraint.hpp>
#include <memory>
#include <vector>
#include <CVector3.hpp>

namespace PBD {

    template<typename T_real=double>
    class CParticleSystem {
    public:
        typedef std::shared_ptr< CParticleSystem<T_real> > Ptr;

        typedef const std::shared_ptr< CParticleSystem<T_real> > ConstPtr;

    public:
        CParticleSystem() = default;

        ~CParticleSystem() = default;

        CParticleSystem( const CParticleSystem& p ): m_particles( p.m_particles )
        { }

        CParticleSystem( CParticleSystem&& p ) noexcept : m_particles( std::move(p.m_particles) )
        { }

        CParticleSystem &operator=(const CParticleSystem &p)
        {
            m_particles = p.m_particles;
            return *this;
        }

        CParticleSystem &operator=(CParticleSystem &&p) noexcept
        {
            m_particles = std::move(p.m_particles);
            return *this;
        }

        void getBoundingBox(vec3::Vector3<T_real>& min, vec3::Vector3<T_real>& max)
        {
            for (auto it=m_particles.begin(); it<m_particles.end(); ++it)
            {
                for (uint i=0; i<3; ++i)
                {
                    min[i] = std::min(min[i] , (*it)->m_position(i));
                    max[i] = std::max(max[i] , (*it)->m_position(i));
                }
            }
        }

        vec3::Vector3<T_real> getCentroid()
        {
            vec3::Vector3<T_real> centroid(0,0,0);
            for (auto it=m_particles.begin(); it<m_particles.end(); ++it)
            {
                for (uint i = 0; i < 3; ++i)
                {
                    centroid[i] += (*it)->m_position[i];
                }
            }
            centroid /= m_particles.size();
            return centroid;
        }

        vec3::Vector3<T_real> getVelocity()
        {
            vec3::Vector3<T_real> vel(0,0,0);
            for (auto it=m_particles.begin(); it<m_particles.end(); ++it)
            {
                for (uint i = 0; i < 3; ++i)
                {
                    vel[i] += (*it)->m_velocity[i];
                }
            }
            vel /= m_particles.size();
            return vel;
        }

        std::vector< PBD::CParticle<>* > m_particles;        ///< Current state of the particle system.
    };

}
#endif //POSITIONBASEDDYNAMICS_CPARTICLESYSTEM_H
