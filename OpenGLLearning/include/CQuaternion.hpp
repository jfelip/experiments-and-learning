#ifndef CQUATERNION_H
#define CQUATERNION_H

#include <Common.h>
#include <memory>

template<class T_real=double >
class CQuaternion
{

public:

	typedef std::shared_ptr<CQuaternion> Ptr;

	typedef std::shared_ptr<CQuaternion> ConstPtr;

	CQuaternion(){}

	CQuaternion(const CQuaternion<T_real>& rhs) { *this = rhs;}

	CQuaternion(const T_real& x,const T_real& y,const T_real& z,const T_real& w)
	{
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
		m_data[3] = w;
	}

	~CQuaternion(){};

	void setIdentity();

	void setZero();

	T_real& operator[](const uint& idx) {return m_data[idx];}

	const T_real& operator[](const uint& idx) const {return m_data[idx];}

	CQuaternion<T_real>& operator=(const CQuaternion<T_real>& rhs);

	CQuaternion<T_real> operator+(const CQuaternion<T_real>& rhs) const;

	CQuaternion<T_real> operator-(const CQuaternion<T_real>& rhs) const;

	CQuaternion<T_real> operator*(const CQuaternion<T_real>& rhs) const;

	CQuaternion<T_real> operator*(const T_real& rhs) const;

	CQuaternion<T_real> operator/(const CQuaternion<T_real>& rhs) const;

	CQuaternion<T_real> operator/(const T_real& rhs) const;

	void conjugate();

	CQuaternion<T_real> getConjugate();

	void inverse();

	CQuaternion<T_real> getInverse();

	void normalize();

	T_real norm();

	T_real sqNorm();

	std::string toString();

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const CQuaternion<T>& rhs);

public:
	T_real m_data[4];

};

template<class T_real>
void CQuaternion<T_real>::setIdentity()
{
	for (uint i=0; i<3; ++i)
	{
		m_data[i] = T_real(0.0);
	}
	m_data[3] = T_real(1.0);
}

template<class T_real>
void CQuaternion<T_real>::setZero()
{
	for (uint i=0; i<4; ++i)
	{
		m_data[i] = T_real(0.0);
	}
}


template<class T_real>
CQuaternion<T_real>& CQuaternion<T_real>::operator=(const CQuaternion<T_real>& rhs)
{
	for (uint i=0; i<4; ++i)
	{
		m_data[i] = rhs[i];
	}
	return *this;
}

template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::operator+(const CQuaternion<T_real>& rhs) const
{
	CQuaternion<T_real> res;
	for (uint i=0; i<4; ++i)
	{
		res[i] = (*this)[i] + rhs[i];
	}
	return res;
}

template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::operator-(const CQuaternion<T_real>& rhs) const
{
	CQuaternion<T_real> res;
	for (uint i=0; i<4; ++i)
	{
		res[i] = (*this)[i] - rhs[i];
	}
	return res;
}

template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::operator*(const CQuaternion<T_real>& rhs) const
{
	CQuaternion<T_real> res;
	res[0] =  m_data[0] * rhs[3] + m_data[1] * rhs[2] - m_data[2] * rhs[1] + m_data[3] * rhs[0];
	res[1] = -m_data[0] * rhs[2] + m_data[1] * rhs[3] + m_data[2] * rhs[0] + m_data[3] * rhs[1];
	res[2] =  m_data[0] * rhs[1] - m_data[1] * rhs[0] + m_data[2] * rhs[3] + m_data[3] * rhs[2];
	res[3] = -m_data[0] * rhs[0] - m_data[1] * rhs[1] - m_data[2] * rhs[2] + m_data[3] * rhs[3];

	return res;
}

template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::operator*(const T_real& rhs) const
{
	CQuaternion<T_real> res;
	for (uint i=0; i<4; ++i)
	{
		res[i] =  m_data[i] * rhs;
	}
	return res;
}


//TODO: Implement division
template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::operator/(const CQuaternion<T_real>& rhs)  const
{

}

template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::operator/(const T_real& rhs) const
{
	CQuaternion<T_real> res;

	for (uint i=0; i<4; ++i)
	{
		res[i] =  m_data[i] / rhs;
	}

	return res;
}

template<class T_real>
void CQuaternion<T_real>::conjugate()
{
	for (uint i=0; i<3; ++i)
	{
		m_data[i] = -m_data[i];
	}
}


template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::getConjugate()
{
	CQuaternion<T_real> res;

	for (uint i=0; i<3; ++i)
	{
		res[i] = -m_data[i];
	}
	res[3] = m_data[3];

	return res;
}

template<class T_real>
void CQuaternion<T_real>::inverse()
{
	T_real squaredNorm = sqNorm();

	conjugate();
	for (uint i=0; i<4; ++i)
	{
		m_data[i] /= squaredNorm;
	}
}


//TODO: Implements this
template<class T_real>
CQuaternion<T_real> CQuaternion<T_real>::getInverse()
{
	CQuaternion<T_real> qInv;
	qInv = getConjugate() / sqNorm();
	return qInv;
}


template<class T_real>
void CQuaternion<T_real>::normalize()
{
	T_real n = norm();
	for (uint i=0; i<4; ++i)
	{
		m_data[i] /= n ;
	}
}

template<class T_real>
T_real CQuaternion<T_real>::sqNorm()
{
	return m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2] + m_data[3]*m_data[3];
}

template<class T_real>
T_real CQuaternion<T_real>::norm()
{
	return sqrt(sqNorm());
}


template<class T_real>
std::string CQuaternion<T_real>::toString()
{
	return ("[ " + std::to_string(m_data[0]) + " , " +
			+ std::to_string(m_data[0]) + " , " +
			+ std::to_string(m_data[0]) + " , " +
			+ std::to_string(m_data[0]) + " ] " );
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const CQuaternion<T>& rhs)
{
	os<<rhs.m_data[0]<<" "<rhs.m_data[1]<<" "<rhs.m_data[2]<<" "<rhs.m_data[3];
	return os;
}


#endif
