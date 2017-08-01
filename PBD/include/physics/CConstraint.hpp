#ifndef POSITIONBASEDDYNAMICS_CCONSTRAINT_H
#define POSITIONBASEDDYNAMICS_CCONSTRAINT_H

#include <memory>
#include <vector>
#include <CVector3.hpp>
#include <physics/CParticle.hpp>

namespace PBD {

    const static double constraintEpsilon = 0.00001;

template<typename T_real=double>
class CConstraint
{
public:
    typedef std::shared_ptr< CConstraint<T_real> > Ptr;

    typedef const std::shared_ptr< CConstraint<T_real> > ConstPtr;

public:
    CConstraint() = default;

    virtual ~CConstraint() = default;

    CConstraint(const CConstraint& C): m_particles(C.m_particles)
    {}

    CConstraint(CConstraint&& C)noexcept : m_particles(std::move(C.m_particles))
    {}

    CConstraint & operator= (const CConstraint& C)
    {
        m_particles = C.m_particles;
        return *this;
    }

    CConstraint & operator= (CConstraint&& C) noexcept
    {
        m_particles = std::move(C.m_particles);
        return *this;
    }

    virtual bool isSatisfied()=0;// { return true; };

    virtual bool isPredSatisfied()=0;// { return true; };

    virtual bool project()=0;// { return true; };

    std::vector< PBD::CParticle<>* > m_particles;
    T_real m_epsilon;

};


    template<typename T_real=double>
    class CNoPenetrationConstraint: public CConstraint<T_real>
    {
    public:
        typedef std::shared_ptr< CNoPenetrationConstraint > Ptr;

        typedef const std::shared_ptr< CNoPenetrationConstraint > ConstPtr;


        CNoPenetrationConstraint(PBD::CParticle<>* p1, PBD::CParticle<>* p2)
        {
            CConstraint<T_real>::m_particles.push_back(p1);
            CConstraint<T_real>::m_particles.push_back(p2);
            CConstraint<T_real>::m_epsilon = PBD::constraintEpsilon;
        }

        bool isSatisfied()
        {
            //if (CConstraint<T_real>::m_particles.size()!=2) return true;
            return (CConstraint<T_real>::m_particles[0]->m_position -
                    CConstraint<T_real>::m_particles[1]->m_position).norm() >
                   (CConstraint<T_real>::m_particles[0]->m_size + CConstraint<T_real>::m_particles[1]->m_size) * 0.5;
        }

        bool isPredSatisfied()
        {
            //if (CConstraint<T_real>::m_particles.size()!=2) return true;
            return (CConstraint<T_real>::m_particles[0]->m_predPosition -
                    CConstraint<T_real>::m_particles[1]->m_predPosition).norm() >
                   (CConstraint<T_real>::m_particles[0]->m_size + CConstraint<T_real>::m_particles[1]->m_size) * 0.5;
        }

        bool project()
        {
            vec3::Vector3<T_real> posAdjustmentDir =
                    CConstraint<T_real>::m_particles[0]->m_predPosition -
                    CConstraint<T_real>::m_particles[1]->m_predPosition;

            T_real err = posAdjustmentDir.norm() -
                    (CConstraint<T_real>::m_particles[0]->m_size + CConstraint<T_real>::m_particles[1]->m_size)*0.5;

            if (err < CConstraint<T_real>::m_epsilon)
            {
                err -= CConstraint<T_real>::m_epsilon;
                posAdjustmentDir.normalize();
                if (CConstraint<T_real>::m_particles[0]->getMass()>0 && CConstraint<T_real>::m_particles[1]->getMass()>0)
                {
                    CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 0.5;
                    CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 0.5;
                }
                else if (CConstraint<T_real>::m_particles[0]->getMass()>0)
                    CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 1.0;
                else if (CConstraint<T_real>::m_particles[1]->getMass()>0)
                    CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 1.0;
            }

            return isPredSatisfied();
        }

    };


    template<typename T_real=double>
    class CConstantDistanceConstraint: public CConstraint<T_real>
    {
    public:
        typedef std::shared_ptr< CConstantDistanceConstraint > Ptr;

        typedef const std::shared_ptr< CConstantDistanceConstraint > ConstPtr;


        CConstantDistanceConstraint(PBD::CParticle<>* p1, PBD::CParticle<>* p2)
        {
            CConstraint<T_real>::m_particles.push_back(p1);
            CConstraint<T_real>::m_particles.push_back(p2);
            m_targetDistance2 = (p1->m_position - p2->m_position).squared_norm();
            m_targetDistance = std::sqrt(m_targetDistance2);
            m_targetDistanceTolerance = 0.01;
            m_targetDistanceTolerance2 = m_targetDistanceTolerance * m_targetDistanceTolerance;
            CConstraint<T_real>::m_epsilon = PBD::constraintEpsilon;
            m_damping = 0.95;
        }

        bool isSatisfied()
        {
            T_real err = (CConstraint<T_real>::m_particles[0]->m_position -
                          CConstraint<T_real>::m_particles[1]->m_position).squared_norm() - m_targetDistance2;

            return -m_targetDistanceTolerance2 < err && err < m_targetDistanceTolerance2;
        }

        bool isPredSatisfied()
        {
            T_real err = (CConstraint<T_real>::m_particles[0]->m_predPosition -
                          CConstraint<T_real>::m_particles[1]->m_predPosition).squared_norm() - m_targetDistance2;

            return m_targetDistanceTolerance2 < err && err < m_targetDistanceTolerance2;
        }

        bool project()
        {
            vec3::Vector3<T_real> posAdjustmentDir =
                    CConstraint<T_real>::m_particles[0]->m_predPosition -
                    CConstraint<T_real>::m_particles[1]->m_predPosition;

            T_real err = posAdjustmentDir.norm() - m_targetDistance;
            if (err > m_targetDistanceTolerance && !std::isinf(err) && !std::isnan(err))
            {
                err = err  - m_targetDistanceTolerance + CConstraint<T_real>::m_epsilon;
            }
            else if (err < -m_targetDistanceTolerance && !std::isinf(err) && !std::isnan(err))
            {
                err = err + m_targetDistanceTolerance - CConstraint<T_real>::m_epsilon;
            }
            else
            {
                //return isPredSatisfied();
                return true;
            }

            posAdjustmentDir.normalize();
            T_real posAdjustmentDirX = posAdjustmentDir(0);
            T_real posAdjustmentDirY = posAdjustmentDir(1);
            T_real posAdjustmentDirZ = posAdjustmentDir(2);
            if (CConstraint<T_real>::m_particles[0]->getMass()>0 && CConstraint<T_real>::m_particles[1]->getMass()>0)
            {
                CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 0.5 * m_damping;
                CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 0.5 * m_damping;
            }
            else if (CConstraint<T_real>::m_particles[0]->getMass()>0)
                CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 1.0 * m_damping;
            else if (CConstraint<T_real>::m_particles[1]->getMass()>0)
                CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 1.0 * m_damping;

            //return true;
            return isPredSatisfied();
        }

        void setTargetDistance( const T_real& d )
        {
            m_targetDistance  = d;
            m_targetDistance2 = d*d;
        }

        void setDistanceTolerance( const T_real& d )
        {
            m_targetDistanceTolerance  = d;
            m_targetDistanceTolerance2 = d*d;
        }

    protected:
        T_real m_targetDistance;
        T_real m_targetDistance2;
        T_real m_targetDistanceTolerance;
        T_real m_targetDistanceTolerance2;
        T_real m_damping;
    };

}

#endif