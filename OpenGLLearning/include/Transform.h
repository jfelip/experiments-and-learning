#ifndef GL_VIEWER_TRANSFORM_H
#define GL_VIEWER_TRANSFORM_H

#include <memory>
#include <vector>
#include <math.h>
#include <CDualQuaternion.hpp>

template<class T_real=double>
class CTransform
{
	typedef std::shared_ptr<CTransform> Ptr;

	typedef std::shared_ptr<CTransform> ConstPtr;

public:
	CTransform(){m_data.setIdentity();}

	CTransform(const CTransform& r);

	~CTransform();

	CTransform& operator=(const CTransform& r);

	CTransform operator*(const CTransform& r) const;

	void setIdentity(){m_data.setIdentity();}

	void translate(const T_real t[3]);

	void translateX(const T_real& tx);

	void translateY(const T_real& ty);

	void translateZ(const T_real& tz);

	void rotate(const T_real r[4]);

	void rotateX(const T_real& rx);

	void rotateY(const T_real& ry);

	void rotateZ(const T_real& rz);

	CDualQuaternion<T_real> m_data;
};



template<class T>
CTransform<T>::~CTransform()
{

}

template<class T>
CTransform<T>::CTransform(const CTransform& r)
{
	*this = r;
}


template<class T>
CTransform<T>& CTransform<T>::operator=(const CTransform<T>& r)
{
	m_data = r.m_data;
	return *this;
}

template<class T>
CTransform<T> CTransform<T>::operator*(const CTransform<T>& r) const
{
	return *this*r;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const CTransform<T>& t)
{
	os << t.m_data;
	return os;
}


//TODO: Implement this
template<class T_real >
void CTransform<T_real>::translate(const T_real t[3])
{
}

template<class T_real >
void CTransform<T_real>::translateX(const T_real& tx)
{
	T_real trans[3] = {tx,0,0};
	translate(trans);
}

template<class T_real >
void CTransform<T_real>::translateY(const T_real& ty)
{
	T_real trans[3] = {0,ty,0};
	translate(trans);
}

template<class T_real >
void CTransform<T_real>::translateZ(const T_real& tz)
{
	T_real trans[3] = {0,0,tz};
	translate(trans);
}

//TODO: Implement this
template<class T_real >
void CTransform<T_real>::rotate(const T_real r[4])
{
}

template<class T_real >
void CTransform<T_real>::rotateX(const T_real& rx)
{
	T_real rxQuat[4] = {T_real(sin(rx * T_real(0.5))), T_real(0),T_real(0), T_real(cos(rx * T_real(0.5)))};
	rotate(rxQuat);
}

template<class T_real >
void CTransform<T_real>::rotateY(const T_real& ry)
{
	T_real ryQuat[4] = {0, sin(ry * T_real(0.5)), 0, cos(ry * T_real(0.5))};
	rotate(ryQuat);
}

template<class T_real >
void CTransform<T_real>::rotateZ(const T_real& rz)
{
	T_real rzQuat[4] = {0,0,sin(rz * T_real(0.5)), cos(rz * T_real(0.5))};
	rotate(rzQuat);
}



#endif
