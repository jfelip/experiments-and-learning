
#include <physics/CPositionBasedDynamics.h>
#include <iostream>
#include <physics/CWorld.h>
typedef double T_real;
typedef vec3::Vector3<T_real> Vector3;



void PBDCreateObjects( PBD::CWorld* pWorld );

int main( int argc, char** argv)
{
    T_real simStep = 0.005;
    T_real simTimeSeconds = 5.0;

    PBD::CWorld PBDWorld;
    PBDWorld.m_gravity = Vector3(0,0,-9.81);

    std::cout<<"Creating objects"<<std::endl;
    PBDCreateObjects(&PBDWorld);

    std::cout<<"Simulation started"<<std::endl;
    for (uint i=0; i<simTimeSeconds/simStep; ++i)
    {
        PBDWorld.step(simStep,0.1);
        std::cout<< "t="<< i*simStep << " particles: "<< PBDWorld.m_particles.size() << std::endl;
    }
    std::cout<<"DONE!"<<std::endl;
}


void PBDCreateObjects( PBD::CWorld* pWorld )
{

    std::cout<<"Creating objects"<<std::endl;

    //Create a cube 5x5x0.05 m size with 5cm particles centered at 0,0,0 (This will be the floor)
    PBD::createParticleSystemSolidCube<T_real>(Vector3(-1.5,-1.5,0) , Vector3(3,3,0.1), pWorld, 0.1, 0, 0);

    //Create cube objects                         position              dimensions             partSize partWeight groupId
    PBD::createParticleSystemSolidCube<T_real>(Vector3(0.5,0.5,3) , Vector3(0.2,0.2,0.2), pWorld, 0.1, 0.01, 1);
    PBD::createParticleSystemSolidCube<T_real>(Vector3(0.5,0.5,4) , Vector3(0.2,0.2,0.2), pWorld, 0.1, 0.02, 2);

//    //Hang one object with a distance constraint
//    PBD::createParticleSystemSolidCube<T_real>(Vector3(0.5,0.5,4.5) , Vector3(0.1,0.1,0.1), pWorld, 0.1, 0, 3);   //Object to hang from
//    PBD::CConstantDistanceConstraint<>::Ptr hangConstraint (
//            new PBD::CConstantDistanceConstraint<>(
//                    &(psObject2->m_particles[0]),
//                    &(psFixedObject->m_particles[0]))
//    );
//    psFixedObject->m_internalConstraints.emplace_back(hangConstraint);

}