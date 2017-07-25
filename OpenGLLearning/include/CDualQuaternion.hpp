#ifndef CDUALQUATERNION_H
#define CDUALQUATERNION_H

#include <Common.h>
#include <memory>
#include <CQuaternion.hpp>
#include <CVector3.hpp>

template<class T_real=double>
class CDualQuaternion
{

public:

	typedef std::shared_ptr<CDualQuaternion> Ptr;

	typedef std::shared_ptr<CDualQuaternion> ConstPtr;

	CDualQuaternion(){}

	CDualQuaternion(const CQuaternion<T_real>& qreal,const CQuaternion<T_real>& qdual)
	{
		m_real = qreal;
		m_dual = qdual;
	}

	CDualQuaternion( CQuaternion<T_real> r, vec3::Vector3<T_real> t )
	{
		r.normalize();
		m_real = r;
		CQuaternion<T_real> qt;
		for (uint i=0; i<3;++i)
		{
			qt[i] = t[i];
		}
		m_dual = ( qt * m_real ) * T_real(0.5);
	}

	CDualQuaternion(const CDualQuaternion<T_real>& rhs)
	{
		m_real = rhs.m_real;
		m_dual = rhs.m_dual;
	}

	~CDualQuaternion(){};

	void setIdentity()
	{
		m_real.setIdentity();
		m_dual.setZero();
	}

	CQuaternion<T_real>& getReal(){ return m_real;}
	CQuaternion<T_real>& getDual(){ return m_dual;}

	CDualQuaternion<T_real> operator=(const CDualQuaternion<T_real>& rhs)
	{
		m_real = rhs.m_real;
		m_dual = rhs.m_dual;
		return *this;
	}

	CDualQuaternion<T_real> operator+(const CDualQuaternion<T_real>& rhs) const
	{
		return CDualQuaternion<T_real>(m_real+rhs.m_real, m_dual+rhs.m_dual);
	}

	CDualQuaternion<T_real> operator-(const CDualQuaternion<T_real>& rhs) const
	{
		return CDualQuaternion<T_real>(m_real-rhs.m_real, m_dual-rhs.m_dual);
	}

	CDualQuaternion<T_real> operator*(const CDualQuaternion<T_real>& rhs) const
	{
		return CDualQuaternion<T_real>(m_real*rhs.m_real, m_dual*rhs.m_real + m_real*rhs.m_dual);
	}

	void conjugate()
	{
		m_real.conjugate();
		m_dual.conjugate();
	}

	CDualQuaternion<T_real> getConjugate() const
	{
		return CDualQuaternion(m_real.getConjugate(), m_dual.getConjugate());
	}

    //TODO:Verify this implementation
	void inverse(){ m_real.inverse(); m_dual.conjugate(); }

    //TODO:Verify this implementation
	CDualQuaternion<T_real> getInverse(){ return CDualQuaternion(m_real.getInverse(), m_dual.getConjugate()); }

	void normalize(){m_real.normalize();}

	std::string toString() {return "Real: " + m_real.toString() + " Dual: "+m_dual.toString();}

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const CDualQuaternion<T>& rhs)
	{
		return os << rhs.getReal() << " " << rhs.getDual();
	}

	void computeMatrix();

	CQuaternion<T_real> GetRotation(){ return m_real; }
	vec3::Vector3<T_real> GetTranslation()
	{
		CQuaternion<T_real> trans = ( m_dual * 2.0f ) * m_real.getConjugate();
		return vec3::Vector3<T_real>(trans[0],trans[1],trans[2]);
	}

public:
	CQuaternion<T_real> m_real;
	CQuaternion<T_real> m_dual;
	T_real m_pMatrix[16];
};


template<class T>
void CDualQuaternion<T>::computeMatrix()
{
	normalize();

    //(b,c,d,a)
    //(0,1,2,3)

	//Row 1 (0,0) (0,1) (0,2) (0,3)
	m_pMatrix[0] = m_real[3]*m_real[3] + m_real[0]*m_real[0] - m_real[1]*m_real[1] - m_real[2]*m_real[2];
	m_pMatrix[1] = 2*m_real[0]*m_real[1] + 2*m_real[3]*m_real[2];
	m_pMatrix[2] = 2*m_real[0]*m_real[2] - 2*m_real[3]*m_real[1];
	m_pMatrix[3] = 0;

	//Column 2 (1,0) (1,1) (1,2) (1,3)
	m_pMatrix[4] = 2*m_real[0]*m_real[1] - 2*m_real[3]*m_real[2];
	m_pMatrix[5] = m_real[3]*m_real[3] - m_real[0]*m_real[0] + m_real[1]*m_real[1] - m_real[2]*m_real[2];
	m_pMatrix[6] = 2*m_real[1]*m_real[2] + 2*m_real[3]*m_real[0];
	m_pMatrix[7] = 0;

	//Column 3 (2,0) (2,1) (2,2) (2,3)
	m_pMatrix[8] =  2*m_real[0]*m_real[2] + 2*m_real[3]*m_real[1];
	m_pMatrix[9] =  2*m_real[1]*m_real[2] - 2*m_real[3]*m_real[0];
	m_pMatrix[10] = m_real[3]*m_real[3] - m_real[0]*m_real[0] - m_real[1]*m_real[1] + m_real[2]*m_real[2];
	m_pMatrix[11] = 0;

	//Column 4 (3,0) (3,1) (3,2) (3,3)
	CQuaternion<T> tmp = (m_dual * T(2.0)) * m_real.getConjugate();

	m_pMatrix[12] = tmp[0];
	m_pMatrix[13] = tmp[1];
	m_pMatrix[14] = tmp[2];
	m_pMatrix[15] = 1;

}

#endif
