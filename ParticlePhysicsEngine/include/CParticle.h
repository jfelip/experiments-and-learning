#ifndef _CPARTICLE_H_
#define _CPARTICLE_H_


#include <CVector3.hpp>
#include <memory>

template <class T_Real=double, class T_UInt=unsigned int>
class CParticle
{
public:
	typedef std::shared_ptr<CParticle> Ptr;
	typedef const std::shared_ptr<CParticle> ConstPtr;

	CParticle(){}

	~CParticle(){}

	void setPosition(vec3::Vector3::ConstPtr &pos);

	void setVelocity(vec3::Vector3::ConstPtr &vel);

	void setInvMass(const T_Real &iM) { m_invmass = iM; }

	void setRadius(const T_Real &r) { m_radius = r; }

	vec3::Vector3::Ptr getPosition(vec3::Vector3::ConstPtr &pos){ return vec3::Vector3::Ptr(&m_position);}

	vec3::Vector3::Ptr getVelocity(vec3::Vector3::ConstPtr &pos){ return vec3::Vector3::Ptr(&m_velocity);}

	void getVelocity(vec3::Vector3::ConstPtr &vel);

	T_Real getInvMass() { return m_invmass; }

	T_Real getRadius() { return m_radius; }

	void clearForce(){m_extForce = 0;}

	void addForce(const vec3::Vector3<T_Real> & force){m_extForce += force;}

	void computePredictedPosition(T_Real timestep)
	{
		m_velocity = m_velocity + m_extForce*timestep;
		m_predPosition = m_position + m_velocity*timestep;
	}

protected:
	vec3::Vector3<T_Real> m_position;		///< Particle position x
	vec3::Vector3<T_Real> m_predPosition; 	///< Predicted particle position x*
	vec3::Vector3<T_Real> m_velocity;		///< Particle velocity
	vec3::Vector3<T_Real> m_extForce;		///< Particle external force
	T_Real m_invmass;						///< Mass inverse (1/m)
	T_UInt m_phase;							///< Particle group identifier
	T_Real m_radius;						///< Particle radius

};

#endif
