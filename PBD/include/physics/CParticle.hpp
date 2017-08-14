#ifndef POSITIONBASEDDYNAMICS_CPARTICLE_H
#define POSITIONBASEDDYNAMICS_CPARTICLE_H

#include <memory>
#include <vector>
#include <CVector3.hpp>
#include <Eigen/Dense>

namespace PBD {

    //TODO:: Make these external parameters
    const static double minVel = 0.001;
    const static double minRotVel = 0.00001;
    const static double velDamping = 1.0;

template<typename T_real=double, typename T_vector=Eigen::Vector3d, typename T_quaternion=Eigen::Quaterniond>
class CParticle
{
public:
    typedef std::shared_ptr< CParticle<T_real> > Ptr;

    typedef const std::shared_ptr< CParticle<T_real> > ConstPtr;

public:
    CParticle() = default;

    CParticle(const T_real& x, const T_real& y, const T_real& z, const T_real& mass, const T_real& size, const size_t& group=0):
            m_position( T_vector(x,y,z) ),
            m_predPosition( T_vector(x,y,z) ),
            m_velocity( T_vector(0,0,0) ),
            m_angularVelocity( T_vector(0,0,0) ),
            m_extForce( T_vector(0,0,0) ),
            m_mass(mass),
            m_massInv(1/mass),
            m_size(size),
            m_group(group)
    {
        m_orientation.setIdentity();
        m_predOrientation.setIdentity();
    }

    CParticle(const T_real& x,    const T_real& y,  const T_real& z,
              const T_real& vx,   const T_real& vy, const T_real& vz,
              const T_real& mass, const T_real& size,
              const size_t& group=0):
            m_position( T_vector(x,y,z) ),
            m_predPosition( T_vector(x,y,z) ),
            m_velocity( T_vector(vx,vy,vz) ),
            m_angularVelocity( T_vector(0,0,0) ),
            m_extForce( T_vector(0,0,0) ),
            m_mass(mass),
            m_massInv(1/mass),
            m_size(size),
            m_group(group)
    {
        m_orientation.setIdentity();
        m_predOrientation.setIdentity();
    }

    CParticle(const T_vector& pos,
              const T_vector& vel,
              const T_real& mass, const T_real& size,
              const size_t& group=0):
            m_position( pos ),
            m_predPosition( T_vector(0,0,0) ),
            m_velocity( vel ),
            m_angularVelocity( T_vector(0,0,0) ),
            m_mass(mass),
            m_massInv(1/mass),
            m_size(size),
            m_group(group)
    {
        m_orientation.setIdentity();
        m_predOrientation.setIdentity();
    }

    ~CParticle() = default;

    CParticle(const CParticle &p) :
            m_position(p.m_position),
            m_orientation(p.m_orientation),
            m_predPosition(p.m_predPosition),
            m_velocity(p.m_velocity),
            m_angularVelocity(p.m_angularVelocity),
            m_extForce(p.m_extForce),
            m_mass(p.m_mass),
            m_massInv(p.m_massInv),
            m_size(p.m_size),
            m_group(p.m_group),
            m_predOrientation(p.m_predOrientation)
    {
        m_predPosition = m_position;
    }

    CParticle(CParticle &&p) noexcept :
            m_position(std::move(p.m_position)),
            m_orientation(std::move(p.m_orientation)),
            m_predPosition(std::move(p.m_predPosition)),
            m_velocity(std::move(p.m_velocity)),
            m_angularVelocity(std::move(p.m_angularVelocity)),
            m_extForce(std::move(p.m_extForce)),
            m_mass(std::move(p.m_mass)),
            m_massInv(std::move(p.m_massInv)),
            m_size(std::move(p.m_size)),
            m_group(std::move(p.m_group)),
            m_predOrientation(std::move(p.m_predOrientation))
    {
        m_predPosition = m_position;
    }

    CParticle &operator=(const CParticle &p)
    {
        m_position = p.m_position;
        m_orientation = p.m_orientation;
        m_predPosition = p.m_predPosition;
        m_velocity = p.m_velocity;
        m_angularVelocity = p.m_angularVelocity;
        m_extForce = p.m_extForce;
        m_mass = p.getMass();
        m_massInv = p.getMassInv();
        m_size = p.m_size;
        m_group = p.m_group;
        m_predOrientation = p.m_predOrientation;
        return *this;
    }

    CParticle &operator=(CParticle &&p) noexcept
    {
        m_position = std::move(p.m_position);
        m_orientation = std::move(p.m_orientation);
        m_predPosition = std::move(p.m_predPosition);
        m_velocity = std::move(p.m_velocity);
        m_angularVelocity = std::move(m_angularVelocity);
        m_extForce = std::move(p.m_extForce);
        m_mass = std::move(p.getMass());
        m_massInv = std::move(p.getMassInv());
        m_size = std::move(p.m_size);
        m_group = std::move(p.m_group);
        m_predOrientation = std::move(p.m_predOrientation);
        return *this;
    }

    void setMass(const T_real &m)
    {
        m_mass = m;
        m_massInv = 1 / m_mass;
    }

    T_real getMass( ) const { return m_mass; }

    T_real getMassInv( ) const { return m_massInv; }

    void clearExtForces() { m_extForce = T_vector(0,0,0); }

    void symplecticEulerUpdate(T_real timeStep)
    {
        if (m_mass <= 0)
        {
            m_predPosition = m_position;
            m_predOrientation = m_orientation;
        }
        else
        {
            T_vector vel = m_velocity + m_extForce * timeStep * m_massInv;
            m_predPosition = m_position + vel * timeStep;

            T_real wNorm = m_angularVelocity.norm();
            if (wNorm > minRotVel)
            {
                T_vector axis = m_angularVelocity / wNorm * sin( wNorm * timeStep / 2.0 );
                T_real   angle = cos ( wNorm * timeStep / 2 );
                T_quaternion delta;
                delta.x() = axis.x();
                delta.y() = axis.y();
                delta.z() = axis.z();
                delta.w() = angle;
                m_predOrientation = delta * m_orientation;
            }
            else
            {
                m_predOrientation = m_orientation;
            }

        }
    }

    void updateVelocity(T_real timeStep)
    {
        if (m_mass > 0)
        {
            T_quaternion quat = m_predOrientation*m_orientation.inverse();
            Eigen::AngleAxisd aa( quat );

            m_velocity = ((m_predPosition - m_position) / timeStep) * PBD::velDamping;
            m_angularVelocity = aa.axis() * aa.angle() / timeStep;
        }
    }

    void updatePositionsWithPredPositions()
    {
        if (m_mass > 0)
        {
            m_position = m_predPosition;
            m_orientation = m_predOrientation;
        }
    }

    T_vector m_position;
    T_vector m_predPosition;
    T_quaternion m_orientation;
    T_quaternion m_predOrientation;
    T_vector m_velocity;
    T_vector m_angularVelocity;
    T_vector m_extForce;
    T_real m_size;
    size_t m_group;

protected:
    T_real m_mass;
    T_real m_massInv;

};

}

#endif