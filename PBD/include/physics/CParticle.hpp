#ifndef POSITIONBASEDDYNAMICS_CPARTICLE_H
#define POSITIONBASEDDYNAMICS_CPARTICLE_H

#include <memory>
#include <vector>
#include <CVector3.hpp>

namespace PBD {

template<typename T_real=double>
class CParticle
{
public:
    typedef std::shared_ptr<CParticle> Ptr;

    typedef const std::shared_ptr<CParticle> ConstPtr;

public:
    CParticle() = default;

    ~CParticle() = default;

    CParticle(const CParticle &p) :
            m_position(p.m_position),
            m_predPosition(p.m_predPosition),
            m_velocity(p.m_velocity),
            m_extForce(p.m_extForce),
            m_mass(p.m_mass),
            m_massInv(p.m_massInv),
            m_size(p.m_size) {
    }

    CParticle(CParticle &&p) noexcept :
            m_position(std::move(p.m_position)),
            m_predPosition(std::move(p.m_predPosition)),
            m_velocity(std::move(p.m_velocity)),
            m_extForce(std::move(p.m_extForce)),
            m_mass(std::move(p.m_mass)),
            m_massInv(std::move(p.m_massInv)),
            m_size(std::move(p.m_size)) {
    }

    CParticle &operator=(const CParticle &p) {
        m_position = p.getPosition();
        m_predPosition = p.getPredPosition();
        m_velocity = p.getVelocity();
        m_extForce = p.getExtForce();
        m_mass = p.getMass();
        m_massInv = p.getMassInv();
        m_size = p.getSize();
        return *this;
    }

    CParticle &operator=(CParticle &&p) noexcept {
        m_position = std::move(p.getPosition());
        m_predPosition = std::move(p.getPredPosition());
        m_velocity = std::move(p.getVelocity());
        m_extForce = std::move(p.getExtForce());
        m_mass = std::move(p.getMass());
        m_massInv = std::move(p.getMassInv());
        m_size = std::move(p.getSize());
        return *this;
    }

    vec3::Vector3 &getPosition() const { return m_position; }

    vec3::Vector3 &getPredPosition() const { return m_predPosition; }

    vec3::Vector3 &getVelocity() const { return m_velocity; }

    vec3::Vector3 &getExtForce() const { return m_extForce; }

    T_real getMass() const { return m_mass; }

    T_real getMassInv() const { return m_massInv; }

    T_real getSize() const { return m_size; }

    void setMass(const T_real &m) {
        m_mass = m;
        m_massInv = 1 / m_mass;
    }

    void setSize(const T_real &s) { m_size = s; }

    //TODO: Implement this
    void symplecticEulerUpdate(T_real timeStep);

    //TODO: Implement this
    void updatePositionsWithPredPositions();

    //TODO: Implement this
    void updateVelocities();


protected:
    vec3::Vector3 m_position;
    vec3::Vector3 m_predPosition;
    vec3::Vector3 m_velocity;
    vec3::Vector3 m_extForce;
    T_real m_mass;
    T_real m_massInv;
    T_real m_size;
};

}