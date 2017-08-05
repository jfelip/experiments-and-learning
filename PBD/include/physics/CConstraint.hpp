#ifndef POSITIONBASEDDYNAMICS_CCONSTRAINT_H
#define POSITIONBASEDDYNAMICS_CCONSTRAINT_H


#include <memory>
#include <vector>
#include <CVector3.hpp>
#include <physics/CParticle.hpp>
#include <Eigen/Dense>
#include <Common.h>

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


    template<typename T_real=double, typename T_vector=Eigen::Vector3d>
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
            T_vector posAdjustmentDir =
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
                    CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 1.0;
                    CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 1.0;
                }
                else if (CConstraint<T_real>::m_particles[0]->getMass()>0)
                    CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 1.0;
                else if (CConstraint<T_real>::m_particles[1]->getMass()>0)
                    CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 1.0;
            }

            return isPredSatisfied();
        }

    };


    template<typename T_real=double, typename T_vector=Eigen::Vector3d>
    class CConstantDistanceConstraint: public CConstraint<T_real>
    {
    public:
        typedef std::shared_ptr< CConstantDistanceConstraint > Ptr;

        typedef const std::shared_ptr< CConstantDistanceConstraint > ConstPtr;


        CConstantDistanceConstraint(PBD::CParticle<>* p1, PBD::CParticle<>* p2)
        {
            CConstraint<T_real>::m_particles.push_back(p1);
            CConstraint<T_real>::m_particles.push_back(p2);
            m_targetDistance2 = (p1->m_position - p2->m_position).squaredNorm();
            m_targetDistance = std::sqrt(m_targetDistance2);
            m_targetDistanceTolerance = 0.01;
            m_targetDistanceTolerance2 = m_targetDistanceTolerance * m_targetDistanceTolerance;
            CConstraint<T_real>::m_epsilon = PBD::constraintEpsilon;
            m_damping = 0.99;
        }

        bool isSatisfied()
        {
            T_real err = (CConstraint<T_real>::m_particles[0]->m_position -
                          CConstraint<T_real>::m_particles[1]->m_position).squaredNorm() - m_targetDistance2;

            return -m_targetDistanceTolerance2 < err && err < m_targetDistanceTolerance2;
        }

        bool isPredSatisfied()
        {
            T_real err = (CConstraint<T_real>::m_particles[0]->m_predPosition -
                          CConstraint<T_real>::m_particles[1]->m_predPosition).squaredNorm() - m_targetDistance2;

            return m_targetDistanceTolerance2 < err && err < m_targetDistanceTolerance2;
        }

        bool project()
        {
            T_vector posAdjustmentDir =
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

        void setConstraintStiffness ( const T_real& d )
        {
            m_damping = d;
        }


    protected:
        T_real m_targetDistance;
        T_real m_targetDistance2;
        T_real m_targetDistanceTolerance;
        T_real m_targetDistanceTolerance2;
        T_real m_damping;
    };

    template<typename T_real=double, typename T_vector=Eigen::Vector3d, typename T_matrix=Eigen::Matrix3d>
    class CShapeMatchingConstraint: public CConstraint<T_real>
    {
    public:
        typedef std::shared_ptr< CShapeMatchingConstraint > Ptr;

        typedef const std::shared_ptr< CShapeMatchingConstraint > ConstPtr;


        explicit CShapeMatchingConstraint(const std::vector< PBD::CParticle<>* >& particles)
        {
            for (const auto &p:particles)
            {
                CConstraint<T_real>::m_particles.push_back(p);
            }
            CConstraint<T_real>::m_epsilon = PBD::constraintEpsilon;

            updateRestStates();
//            computeCovariance(CConstraint<T_real>::m_particles,m_restCovMat);

            //Update deformed configuration states
            m_deformedCoM = computePredCenterOfMass(CConstraint<T_real>::m_particles);
            computeCovariance(CConstraint<T_real>::m_particles,m_deformedCovMat);
        }

        bool isSatisfied()
        {
            T_matrix cov, covDiffL2Norm;
            computeCovariance(CConstraint<T_real>::m_particles,cov);
            return ( (m_restCovMat - cov).norm() < m_covDistanceThreshold );
        }

        bool isPredSatisfied()
        {
            T_matrix cov, covDiffL2Norm;
            computeCovariance(CConstraint<T_real>::m_particles,cov);
            return ( (m_restCovMat - cov).norm() < m_covDistanceThreshold );
        }

        bool project()
        {
            updateRestStates();
            T_vector c = computePredCenterOfMass(CConstraint<T_real>::m_particles); //CoM in the deformed configuration
            T_matrix Q;
            computeCovariance(CConstraint<T_real>::m_particles,Q);              //Eigenvectors of Covariance in the deformed configuration

            //Detect if the rotation matrix has flipped axes
            T_vector x_axis = (Q.col(0) + m_restCovMat.col(0)) * 0.5;
            T_vector y_axis = (Q.col(1) + m_restCovMat.col(1)) * 0.5;
            T_vector z_axis = (Q.col(2) + m_restCovMat.col(2)) * 0.5;
            if (x_axis.norm() < 0.1)
            {
                _GENERIC_ERROR_("X axis might be flipped");
                Q.col(0) = -Q.col(0);
            }
            if (y_axis.norm() < 0.1)
            {
                _GENERIC_ERROR_("Y axis might be flipped");
                Q.col(1) = -Q.col(1);
            }
            if (z_axis.norm() < 0.1)
            {
                _GENERIC_ERROR_("Z axis might be flipped");
                Q.col(2) = -Q.col(2);
            }
            m_deformedCovMat = Q;
            m_deformedCoM = c;


            //Calculate the rotation w.r.t. covariance matrix in the rest state
            //Q = m_restCovMat.transpose() * Q;
            Q = Q * m_restCovMat.transpose();

            Eigen::Quaterniond quat(Q);
            T_real dist = quat.angularDistance(Eigen::Quaterniond(1,0,0,0));

            if (dist > 0.5)
            {
                Q.setIdentity();
                _GENERIC_ERROR_("Rotation magnitude too big: " + std::to_string(dist));
            }


            for (const auto& p:CConstraint<T_real>::m_particles)
            {
                T_vector x_star = p->m_predPosition;                //particle position in the deformed configuration
                T_vector ri = x_star - m_restCoM;               //particle offset to the CoM in the rest configuration
                T_vector posDelta = (Q * ri + c) - x_star;
                //p->m_predPosition += posDelta;
                p->m_predPosition = p->m_position + posDelta;
            }

            //return isPredSatisfied();
            return true;
        }

        //TODO: HIGH Consider particles can have different mass in the com computation
        T_vector computeCenterOfMass( const std::vector< PBD::CParticle<>* >& particles )
        {
            T_vector CoM (T_real(0),T_real(0),T_real(0));

            for (const auto& p:particles)
            {
                CoM += p->m_position;
            }
            CoM /= particles.size();
            return CoM;
        }

        T_vector computePredCenterOfMass( const std::vector< PBD::CParticle<>* >& particles )
        {
            T_vector CoM (T_real(0),T_real(0),T_real(0));

            for (const auto& p:particles)
            {
                CoM += p->m_predPosition;
            }
            CoM /= particles.size();
            return CoM;
        }

        void computeCovariance( const std::vector< PBD::CParticle<>* >& particles, T_matrix& cov )
        {

            cov.setZero();

            T_vector c = computePredCenterOfMass(CConstraint<T_real>::m_particles);

            for (const auto& p:particles)
            {
                T_vector x_star = p->m_predPosition;
                T_vector ri = x_star - m_restCoM;
                cov += (x_star - c) * ri.transpose();
            }

            Eigen::FullPivHouseholderQR<Eigen::Matrix3d> solver;
            solver.compute(cov);
            cov = solver.matrixQ();
        }

        void computeRestCovariance( const std::vector< PBD::CParticle<>* >& particles, T_matrix& cov )
        {

            cov.setZero();

            T_vector c = computeCenterOfMass(CConstraint<T_real>::m_particles);

            for (const auto& p:particles)
            {
                T_vector x_star = p->m_position;
                T_vector ri = x_star - m_restCoM;
                cov += (x_star - c) * ri.transpose();
            }

            Eigen::FullPivHouseholderQR<Eigen::Matrix3d> solver;
            solver.compute(cov);
            cov = solver.matrixQ();
        }


        void updateRestStates ()
        {
            m_restCoM = computeCenterOfMass(CConstraint<T_real>::m_particles);
            T_matrix Q;
            computeRestCovariance(CConstraint<T_real>::m_particles,Q);
            //Detect if the rotation matrix has flipped axes
            T_vector x_axis = (Q.col(0) + m_restCovMat.col(0)) * 0.5;
            T_vector y_axis = (Q.col(1) + m_restCovMat.col(1)) * 0.5;
            T_vector z_axis = (Q.col(2) + m_restCovMat.col(2)) * 0.5;
            if (x_axis.norm() < 0.1)
            {
                _GENERIC_ERROR_("X axis might be flipped");
                Q.col(0) = -Q.col(0);
            }
            if (y_axis.norm() < 0.1)
            {
                _GENERIC_ERROR_("Y axis might be flipped");
                Q.col(1) = -Q.col(1);
            }
            if (z_axis.norm() < 0.1)
            {
                _GENERIC_ERROR_("Z axis might be flipped");
                Q.col(2) = -Q.col(2);
            }
            m_restCovMat = Q;
        }

        T_vector getCoM()    { return m_restCoM; }
        T_matrix getCovMat() { return m_restCovMat; }
        T_vector getDeformedCoM()    { return m_deformedCoM; }
        T_matrix getDeformedCovMat() { return m_deformedCovMat; }

    protected:
        T_vector m_restCoM;
        T_matrix m_restCovMat;
        T_vector m_deformedCoM;
        T_matrix m_deformedCovMat;
        T_real   m_covDistanceThreshold;
    };


}

#endif