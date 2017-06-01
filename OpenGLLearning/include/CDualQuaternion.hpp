#ifndef CDUALQUATERNION_H
#define CDUALQUATERNION_H

#include <Common.h>
#include <memory>
#include <CQuaternion.hpp>

template<class T_real=double>
class CDualQuaternion
{

public:

	typedef std::shared_ptr<CDualQuaternion> Ptr;

	typedef std::shared_ptr<CDualQuaternion> ConstPtr;

	CDualQuaternion(){setIdentity();}

	CDualQuaternion(const CDualQuaternion<T_real>& rhs) { *this = rhs;}

	~CDualQuaternion(){};

	void setIdentity(){m_real.setIdentity(); m_dual.setIdentity(); m_dual[3]=0;}

	CQuaternion<T_real>& getReal(){ return m_real;}
	CQuaternion<T_real>& getDual(){ return m_dual;}

	T_real& operator[](const uint& idx);

	CDualQuaternion<T_real> operator=(const CDualQuaternion<T_real>& rhs){ m_real = rhs.m_real; m_dual = rhs.m_dual; return *this;}

	CDualQuaternion<T_real> operator+(const CDualQuaternion<T_real>& rhs);

	CDualQuaternion<T_real> operator-(const CDualQuaternion<T_real>& rhs);

	CDualQuaternion<T_real> operator*(const CDualQuaternion<T_real>& rhs);

	CDualQuaternion<T_real> operator/(const CDualQuaternion<T_real>& rhs);

	void conjugate();

	CDualQuaternion<T_real> getConjugate();

	void inverse();

	void getInverse();

	void normalize(){m_real.normalize();}

	T_real norm();

	T_real sqNorm();

	std::string toString();

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const CDualQuaternion<T>& rhs);

	void computeMatrix();

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
	m_pMatrix[1] = 2*m_real[0]*m_real[1] - 2*m_real[3]*m_real[2];
	m_pMatrix[2] = 2*m_real[0]*m_real[2] + 2*m_real[3]*m_real[1];
	m_pMatrix[3] = 0;

	//Column 2 (1,0) (1,1) (1,2) (1,3)
	m_pMatrix[4] = 2*m_real[0]*m_real[1] + 2*m_real[3]*m_real[2];
	m_pMatrix[5] = m_real[3]*m_real[3] - m_real[0]*m_real[0] + m_real[1]*m_real[1] - m_real[2]*m_real[2];
	m_pMatrix[6] = 2*m_real[1]*m_real[2] - 2*m_real[3]*m_real[0];
	m_pMatrix[7] = 0;

	//Column 3 (2,0) (2,1) (2,2) (2,3)
	m_pMatrix[8] =  2*m_real[0]*m_real[2] - 2*m_real[3]*m_real[1];
	m_pMatrix[9] =  2*m_real[1]*m_real[2] + 2*m_real[3]*m_real[0];
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
