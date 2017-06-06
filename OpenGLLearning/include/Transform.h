#ifndef GL_VIEWER_TRANSFORM_H
#define GL_VIEWER_TRANSFORM_H

#include <memory>
#include <vector>
#include <math.h>
#include <CDualQuaternion.hpp>
#include <CVector3.hpp>

template<class T_real=double>
class CTransform
{
	typedef std::shared_ptr<CTransform> Ptr;

	typedef std::shared_ptr<CTransform> ConstPtr;

public:
	CTransform(){setIdentity();}

	CTransform(CDualQuaternion<T_real> dq){ m_data = dq;}

	CTransform(const CTransform& r) {*this = r;}

	~CTransform(){}

	void setIdentity(){ m_data.setIdentity(); }

	CTransform& operator=(const CTransform& r);

	CTransform operator*(const CTransform& r) const;

	void translate(const vec3::Vector3<T_real>& t);

	void translateX(const T_real& tx);

	void translateY(const T_real& ty);

	void translateZ(const T_real& tz);

	void rotate(const CQuaternion<T_real>& r);

	void rotateX(const T_real& rx);

	void rotateY(const T_real& ry);

	void rotateZ(const T_real& rz);

	CDualQuaternion<T_real> m_data;
};


template<class T>
CTransform<T>& CTransform<T>::operator=(const CTransform<T>& r)
{
	m_data = r.m_data;
	return *this;
}

template<class T>
CTransform<T> CTransform<T>::operator*(const CTransform<T>& r) const
{
	return CTransform<T>(m_data*r.m_data);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const CTransform<T>& t)
{
	os << t.m_data;
	return os;
}


template<class T_real >
void CTransform<T_real>::translate(const vec3::Vector3<T_real>& t)
{
	CQuaternion<T_real> r;
	r.setIdentity();
	CDualQuaternion<T_real> dQuat(r,t);
	m_data = m_data * dQuat;
}

template<class T_real >
void CTransform<T_real>::translateX(const T_real& tx)
{
	vec3::Vector3<T_real> trans(tx,0,0);
	translate(trans);
}

template<class T_real >
void CTransform<T_real>::translateY(const T_real& ty)
{
	vec3::Vector3<T_real> trans(0,ty,0);
	translate(trans);
}

template<class T_real >
void CTransform<T_real>::translateZ(const T_real& tz)
{
	vec3::Vector3<T_real> trans(0,0,tz);
	translate(trans);
}

template<class T_real >
void CTransform<T_real>::rotate(const CQuaternion<T_real>& r)
{
	CQuaternion<T_real> t;
	t.setZero();
	CDualQuaternion<T_real> dQuat(r,t);
	m_data = m_data * dQuat;
}

template<class T_real >
void CTransform<T_real>::rotateX(const T_real& rx)
{
	CQuaternion<T_real> rxQuat(T_real(sin(rx * T_real(0.5))), T_real(0),T_real(0), T_real(cos(rx * T_real(0.5))));
	rotate(rxQuat);
}

template<class T_real >
void CTransform<T_real>::rotateY(const T_real& ry)
{
	CQuaternion<T_real> ryQuat(0, sin(ry * T_real(0.5)), 0, cos(ry * T_real(0.5)));
	rotate(ryQuat);
}

template<class T_real >
void CTransform<T_real>::rotateZ(const T_real& rz)
{
	CQuaternion<T_real> rzQuat(0,0,sin(rz * T_real(0.5)), cos(rz * T_real(0.5)));
	rotate(rzQuat);
}



#endif

