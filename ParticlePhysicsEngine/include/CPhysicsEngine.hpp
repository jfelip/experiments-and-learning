#ifndef _CPARTICLEPHYSICS_H_
#define _CPARTICLEPHYSICS_H_

#include <vector>
#include <memory>
#include <CParticle.h>

template <class T_Real=double>
class CParticlePhysicsEngine
{
public:
	typedef std::shared_ptr<CParticlePhysicsEngine> Ptr;
	typedef const std::shared_ptr<CParticlePhysicsEngine> ConstPtr;

	CParticlePhysicsEngine(){}

	~CParticlePhysicsEngine(){}

	void setTimeStep(const T_Real &timestep);

	T_Real getTimeStep() {return m_timestep;}

	void step();

protected:
	std::vector<CParticle::Ptr> m_particles;
	T_Real m_timestep = 0.01;
	vec3::Vector3<T_Real> m_gravity = {0,0,-9.81};

};


template <class T>
void CParticlePhysicsEngine<T>::step()
{

	for (auto p:m_particles)
	{
		p.clearForce();

		p.addForce(m_gravity);

		p.computePredictedPosition(m_timestep);

		p.applyMassScaling();
	}

	for (auto p:m_particles)
	{
		//find neighboring particles
		//find solid contacts
	}

	//While iter<SolverIterations
		//For each constraint group
			//solve all constraints delta_x, n
			//update x* = x* + delta_x/n

	//For all particles do
		//Update velocity
		//advect diffuse particles
		//apply internal forces
		//update positions

}

#endif
