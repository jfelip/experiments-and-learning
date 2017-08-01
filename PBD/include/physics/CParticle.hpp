#ifndef POSITIONBASEDDYNAMICS_CPARTICLE_H
#define POSITIONBASEDDYNAMICS_CPARTICLE_H

#include <memory>
#include <vector>
#include <CVector3.hpp>

namespace PBD {

    const static double minVel = 0.001;
    const static double velDamping = 0.99;


    template<typename T_real=double>
class CParticle
{
public:
    typedef std::shared_ptr< CParticle<T_real> > Ptr;

    typedef const std::shared_ptr< CParticle<T_real> > ConstPtr;

public:
    CParticle() = default;

    CParticle(const T_real& x, const T_real& y, const T_real& z, const T_real& mass, const T_real& size, const size_t& group=0):
            m_position( vec3::Vector3<T_real>(x,y,z) ),
            m_velocity( vec3::Vector3<T_real>(0,0,0) ),
            m_extForce( vec3::Vector3<T_real>(0,0,0) ),
            m_mass(mass),
            m_massInv(1/mass),
            m_size(size),
            m_group(group),
            m_collision(false)
    {

    }

    CParticle(const T_real& x,    const T_real& y,  const T_real& z,
              const T_real& vx,   const T_real& vy, const T_real& vz,
              const T_real& mass, const T_real& size,
              const size_t& group=0):
            m_position( vec3::Vector3<T_real>(x,y,z) ),
            m_velocity( vec3::Vector3<T_real>(vx,vy,vz) ),
            m_extForce( vec3::Vector3<T_real>(0,0,0) ),
            m_mass(mass),
            m_massInv(1/mass),
            m_size(size),
            m_group(group),
            m_collision(false)
    {

    }

    CParticle(const vec3::Vector3<T_real>& pos,
              const vec3::Vector3<T_real>& vel,
              const T_real& mass, const T_real& size,
              const size_t& group=0):
            m_position( pos ),
            m_velocity( vel ),
            m_mass(mass),
            m_massInv(1/mass),
            m_size(size),
            m_group(group),
            m_collision(false)
    {

    }

    ~CParticle() = default;

    CParticle(const CParticle &p) :
            m_position(p.m_position),
            m_predPosition(p.m_predPosition),
            m_velocity(p.m_velocity),
            m_extForce(p.m_extForce),
            m_mass(p.m_mass),
            m_massInv(p.m_massInv),
            m_size(p.m_size),
            m_group(p.m_group),
            m_collision(p.m_collision)
    { }

    CParticle(CParticle &&p) noexcept :
            m_position(std::move(p.m_position)),
            m_predPosition(std::move(p.m_predPosition)),
            m_velocity(std::move(p.m_velocity)),
            m_extForce(std::move(p.m_extForce)),
            m_mass(std::move(p.m_mass)),
            m_massInv(std::move(p.m_massInv)),
            m_size(std::move(p.m_size)),
            m_group(std::move(p.m_group)),
            m_collision(std::move(p.m_collision))
    { }

    CParticle &operator=(const CParticle &p)
    {
        m_position = p.m_position;
        m_predPosition = p.m_predPosition;
        m_velocity = p.m_velocity;
        m_extForce = p.m_extForce;
        m_mass = p.getMass();
        m_massInv = p.getMassInv();
        m_size = p.m_size;
        m_group = p.m_group;
        m_collision = p.m_collision;
        return *this;
    }

    CParticle &operator=(CParticle &&p) noexcept
    {
        m_position = std::move(p.m_position);
        m_predPosition = std::move(p.m_predPosition);
        m_velocity = std::move(p.m_velocity);
        m_extForce = std::move(p.m_extForce);
        m_mass = std::move(p.getMass());
        m_massInv = std::move(p.getMassInv());
        m_size = std::move(p.m_size);
        m_group = std::move(p.m_group);
        m_collision = std::move(p.m_collision);
        return *this;
    }

    void setMass(const T_real &m) {
        m_mass = m;
        m_massInv = 1 / m_mass;
    }

    T_real getMass( ) const { return m_mass; }

    T_real getMassInv( ) const { return m_massInv; }

    void clearExtForces() { m_extForce = vec3::Vector3<T_real>(0,0,0); }

    void symplecticEulerUpdate(T_real timeStep)
    {
        if (m_mass <= 0)
        {
            m_predPosition = m_position;
        }
        else
        {
            m_velocity     = m_velocity + m_extForce * timeStep * m_massInv * PBD::velDamping;
            if (m_velocity.norm() > PBD::minVel)
                m_predPosition = m_position + m_velocity * timeStep;
            else
                m_predPosition = m_position;
        }
    }

    void updatePositionsWithPredPositions(T_real timeStep)
    {
        if (m_mass <= 0)
        {
            m_position = m_position;
        }
        else
        {
            m_velocity = (m_predPosition - m_position) / timeStep;
            m_position = m_predPosition;
        }
    }

    vec3::Vector3<T_real> m_position;
    vec3::Vector3<T_real> m_predPosition;
    vec3::Vector3<T_real> m_velocity;
    vec3::Vector3<T_real> m_extForce;
    T_real m_size;
    size_t m_group;
    bool m_collision;

protected:
    T_real m_mass;
    T_real m_massInv;

};

}

#endif