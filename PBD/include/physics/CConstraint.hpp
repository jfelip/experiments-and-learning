#ifndef POSITIONBASEDDYNAMICS_CCONSTRAINT_H
#define POSITIONBASEDDYNAMICS_CCONSTRAINT_H


#include <memory>
#include <vector>
#include <CVector3.hpp>
#include <physics/CParticle.hpp>
#include <Eigen/Dense>
#include <Common.h>

namespace PBD {

    const static double constraintEpsilon = 0.0001;

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
                    CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 0.5;
                    CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 0.5;
                }
                else if (CConstraint<T_real>::m_particles[0]->getMass()>0)
                    CConstraint<T_real>::m_particles[0]->m_predPosition -= posAdjustmentDir * err * 1.0;
                else if (CConstraint<T_real>::m_particles[1]->getMass()>0)
                    CConstraint<T_real>::m_particles[1]->m_predPosition += posAdjustmentDir * err * 1.0;
            }

            //TODO: Apply friction
            //TODO: Apply restitution

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

        //TODO: Make this parameters global and dependent of the timeStep
        constexpr static T_real MAX_ANGLE_MAGNITUDE = 0.05;
        constexpr static T_real MAX_AXIS_DIFF = 0.3;

        explicit CShapeMatchingConstraint(const std::vector< PBD::CParticle<>* >& particles)
        {
            for (const auto &p:particles)
            {
                CConstraint<T_real>::m_particles.push_back(p);
            }
            CConstraint<T_real>::m_epsilon = PBD::constraintEpsilon;

            m_restCoM = computeCenterOfMass(CConstraint<T_real>::m_particles);
            computeRestEigenVectors(CConstraint<T_real>::m_particles,m_restCovMat);

            //Populate target particle positions w.r.t. initial CoM and Orientation
            for (const auto &p:particles)
            {
                T_matrix wMo = m_restCovMat;                //Rotation matrix that converts world coordinates to local frame
                T_vector pTo = p->m_position - m_restCoM;   //Translation of the point w.r.t. local frame
                m_shapeMatchingPositions.push_back( wMo.transpose() * pTo ); //Store each particle position w.r.t. local frame
            }

            //Update deformed configuration states
            m_deformedCoM = computePredCenterOfMass(CConstraint<T_real>::m_particles);
            computeEigenVectors(CConstraint<T_real>::m_particles,m_deformedCovMat);
        }

        bool isSatisfied()
        {
            Eigen::Quaterniond restQuat(m_restCovMat);
            Eigen::Quaterniond defQuat(m_deformedCovMat);
            T_real dist = restQuat.angularDistance(defQuat);
            return ( dist < CConstraint<T_real>::m_epsilon );
        }

        bool isPredSatisfied()
        {
            Eigen::Quaterniond restQuat(m_restCovMat);
            Eigen::Quaterniond defQuat(m_deformedCovMat);
            T_real dist = restQuat.angularDistance(defQuat);
            return ( dist < CConstraint<T_real>::m_epsilon );
        }

        T_matrix checkAxisPermutation( const T_matrix& oldMat,  const T_matrix& newMat )
        {
            T_real threshold = 0.01;
            T_matrix res;

            for (uint i=0; i<3; ++i)
            {
                if ( (oldMat.col(0) - newMat.col(0)).norm() < threshold )
                {
                    res.col(i) = oldMat.col(i);
                }
                else if ( (oldMat.col(0) - newMat.col(1)).norm() < threshold )
                {
                    res.col(i) = oldMat.col(i);
                }
                else if ( (oldMat.col(i) - newMat.col(2)).norm() < threshold )
                {
                    res.col(i) = oldMat.col(i);
                }
                else
                {
                    res.col(i) = newMat.col(i);
                }
            }

            return res;
        }

        T_matrix checkAxisInversion( const T_matrix& oldMat,  const T_matrix& newMat )
        {
            T_real threshold = 0.01;
            T_matrix res;

            for (uint i=0; i<3; ++i)
            {
                if ( (oldMat.col(0) - newMat.col(0)).norm() > (1+threshold) )
                {
                    res.col(i) = oldMat.col(i);
                }
                else
                {
                    res.col(i) = newMat.col(i);
                }
            }

            return res;
        }

        bool project()
        {
            //Update the rest configuration descriptors
            m_restCoM = computeCenterOfMass(CConstraint<T_real>::m_particles);
            T_matrix Q, qtmp;
            computeRestEigenVectors(CConstraint<T_real>::m_particles,Q);
            qtmp = checkAxisPermutation( m_restCovMat, Q );
            m_restCovMat = checkAxisInversion  ( m_restCovMat, qtmp );


//            deltaQ = m_restCovMat.transpose() * Q;
//            Eigen::Quaterniond quatRest(deltaQ);
//            T_real distRest = quatRest.angularDistance(Eigen::Quaterniond(1,0,0,0));
//            if (distRest > MAX_ANGLE_MAGNITUDE)
//            {
//                _GENERIC_ERROR_("REST CONFIG UPDATE: Rotation magnitude too big: " + std::to_string(distRest));
//            }
//            else
//            {
//                m_restCovMat = Q;
//            }

            //Update the deformed configuration descriptors
            m_deformedCoM = computePredCenterOfMass(CConstraint<T_real>::m_particles); //CoM in the deformed configuration
            computeEigenVectors(CConstraint<T_real>::m_particles,Q);              //Q component of QR decompositions of the Covariance in the deformed configuration

            //Calculate the rotation w.r.t. deformed covariance matrix in the previous iteration
            Q = m_deformedCovMat.transpose() * Q;
            qtmp = checkAxisPermutation( m_deformedCovMat, Q );
            m_deformedCovMat = checkAxisInversion  ( m_deformedCovMat, qtmp );
//            Eigen::Quaterniond quat(Q);
//            T_real dist = quat.angularDistance(Eigen::Quaterniond(1,0,0,0));
//
//            while (dist > MAX_ANGLE_MAGNITUDE)
//            if (dist > MAX_ANGLE_MAGNITUDE)
//            {
//                Q.setIdentity();
//                quat = quat.slerp(0.5,Eigen::Quaterniond(1,0,0,0));
//                Q = quat.toRotationMatrix();
//                dist = quat.angularDistance(Eigen::Quaterniond(1,0,0,0));
//                _GENERIC_ERROR_("Rotation magnitude too big: " + std::to_string(dist));
//            }
//            m_deformedCovMat = m_deformedCovMat * Q;
//
//
//            m_restCovMat = m_restCovMat * Q;
//
//            else
//            {
//                _GENERIC_DEBUG_("Rotation magnitude: " + std::to_string(dist));
//            }

            uint i = 0;
            double totalDeltas = 0;
            for (const auto& p:CConstraint<T_real>::m_particles)
            {
                //Compute the delta to move each particle to its shape target position in the local frame

                //1 - Convert particle shape target position to world frame
                T_vector tTw = m_deformedCovMat * m_shapeMatchingPositions[i] + m_deformedCoM;

                //2 - Calculate the delta
                T_vector deltaWorld = tTw - p->m_predPosition;

                totalDeltas += deltaWorld.norm();

                //3 - Add the delta to the predPosition
                double stiffness = 1.0; //TODO: Make this a parameter
                p->m_predPosition = p->m_predPosition + deltaWorld * 1.0;
                p->m_predOrientation = Eigen::Quaterniond(m_deformedCovMat);
                ++i;

// POSITION DELTA FROM THE PBD PAPERS
//                T_vector x_star = p->m_predPosition;            //particle position in the deformed configuration
//                T_vector ri = x_star - m_restCoM;               //particle offset to the CoM in the rest configuration
//                T_vector posDelta = (Q * ri + c) - x_star;
//                //p->m_predPosition += posDelta;
//                p->m_predPosition = p->m_position + posDelta;
//                p->m_predOrientation = quat * p->m_orientation;
            }

            //return totalDeltas < 0.0001;
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

        void computeEigenVectors( const std::vector< PBD::CParticle<>* >& particles, T_matrix& eigenvectors )
        {
            T_vector c = computePredCenterOfMass(CConstraint<T_real>::m_particles);
            T_matrix cov;

            cov.setZero();
            for (const auto& p:particles)
            {
                T_matrix A = 0.2*p->getMass()*p->m_size*p->m_predOrientation.toRotationMatrix();
                T_vector x_star = p->m_predPosition;
                T_vector ri = x_star - m_restCoM;
                cov += (x_star - c) * ri.transpose();
//                cov += A + (x_star - c) * ri.transpose();
            }

            Eigen::SelfAdjointEigenSolver<T_matrix> es(cov);
            cov = es.eigenvectors();

            //Make sure it is a right handed orthonormal basis
            eigenvectors.col(0) = cov.col(0);                   //x = first eigenvector
            eigenvectors.col(0).normalize();
            eigenvectors.col(2) = cov.col(0).cross(cov.col(1)); //z = x*y
            eigenvectors.col(2).normalize();
            eigenvectors.col(1) = cov.col(2).cross(cov.col(0)); //y = z*x
            eigenvectors.col(1).normalize();

        }

        void computeRestEigenVectors( const std::vector< PBD::CParticle<>* >& particles, T_matrix& eigenvectors )
        {

            T_matrix cov;
            cov.setIdentity();

            T_vector c = computeCenterOfMass(CConstraint<T_real>::m_particles);

            for (const auto& p:particles)
            {
                T_matrix A = 0.2*p->getMass()*p->m_size*p->m_predOrientation.toRotationMatrix();
                T_vector x_star = p->m_position;
                T_vector ri = x_star - m_restCoM;
                cov += (x_star - c) * ri.transpose();
//                cov += A + (x_star - c) * ri.transpose();
            }

            Eigen::EigenSolver<T_matrix> es(cov);
            cov = es.eigenvectors().real();

            //Make sure it is a right handed orthonormal basis
            eigenvectors.col(0) = cov.col(0);
            eigenvectors.col(0).normalize();
            eigenvectors.col(2) = cov.col(0).cross(cov.col(1));
            eigenvectors.col(2).normalize();
            eigenvectors.col(1) = cov.col(2).cross(cov.col(0));
            eigenvectors.col(1).normalize();

//            Eigen::FullPivHouseholderQR<Eigen::Matrix3d> solver;
//            solver.compute(cov);
//            cov = solver.matrixQ();
        }

        T_vector getCoM()    { return m_restCoM; }
        T_matrix getCovMat() { return m_restCovMat; }
        T_vector getDeformedCoM()    { return m_deformedCoM; }
        T_matrix getDeformedCovMat() { return m_deformedCovMat; }

        std::vector<T_vector> m_shapeMatchingPositions;

    protected:
        T_vector m_restCoM;
        T_matrix m_restCovMat;
        T_vector m_deformedCoM;
        T_matrix m_deformedCovMat;
    };


}

#endif