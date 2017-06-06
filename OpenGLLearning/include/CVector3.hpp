#ifndef _CVECTOR3_H_
#define _CVECTOR3_H_

#include <ostream>

namespace vec3
{

template <class T_Real=double >
class Vector3
{
public:

	Vector3()
	{
		m_data[0] = 0.0;
		m_data[1] = 0.0;
		m_data[2] = 0.0;
	}

	Vector3(T_Real x,T_Real y,T_Real z)
	{
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
	}

	Vector3( const Vector3& right )
	{
		m_data[0] = right(0);
		m_data[1] = right(1);
		m_data[2] = right(2);
	}

	T_Real x() {return m_data[0];}
	T_Real y() {return m_data[1];}
	T_Real z() {return m_data[2];}

	T_Real& operator[](unsigned int i) {return m_data[i];}
	T_Real operator()(unsigned int i) const {return m_data[i];}


	Vector3& operator=( const Vector3& right )
	{
		m_data[0] = right(0);
		m_data[1] = right(1);
		m_data[2] = right(2);
		return *this;
	}

	Vector3& operator=( const T_Real right )
	{
		m_data[0] = right;
		m_data[1] = right;
		m_data[2] = right;
		return *this;
	}


	Vector3& operator*=( const Vector3& right )
	{
		m_data[0] = m_data[0] * right(0);
		m_data[1] = m_data[1] * right(1);
		m_data[2] = m_data[2] * right(2);
		return *this;
	}

	Vector3& operator*=( const T_Real right )
	{
		m_data[0] = m_data[0] * right;
		m_data[1] = m_data[1] * right;
		m_data[2] = m_data[2] * right;
		return *this;
	}

	Vector3& operator/=( const Vector3& right )
	{
		m_data[0] = m_data[0] / right(0);
		m_data[1] = m_data[1] / right(1);
		m_data[2] = m_data[2] / right(2);
		return *this;
	}

	Vector3& operator/=( const T_Real right )
	{
		m_data[0] = m_data[0] / right;
		m_data[1] = m_data[1] / right;
		m_data[2] = m_data[2] / right;
		return *this;
	}

	Vector3& operator+=( const Vector3& right )
	{
		m_data[0] = m_data[0] + right(0);
		m_data[1] = m_data[1] + right(1);
		m_data[2] = m_data[2] + right(2);
		return *this;
	}

	Vector3& operator+=( const T_Real right )
	{
		m_data[0] = m_data[0] + right;
		m_data[1] = m_data[1] + right;
		m_data[2] = m_data[2] + right;
		return *this;
	}

	Vector3& operator-=( const Vector3& right )
	{
		m_data[0] = m_data[0] - right(0);
		m_data[1] = m_data[1] - right(1);
		m_data[2] = m_data[2] - right(2);
		return *this;
	}

	Vector3& operator-=( const T_Real right )
	{
		m_data[0] = m_data[0] - right;
		m_data[1] = m_data[1] - right;
		m_data[2] = m_data[2] - right;
		return *this;
	}


	T_Real norm()
	{
		return sqrt(squared_norm());
	}

	T_Real squared_norm()
	{
		return ( m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2]);
	}

	void normalize()
	{
		*this = *this / norm();
	}

protected:
	T_Real m_data[3];
};


template<typename T>
Vector3<T> operator*(const Vector3<T>& rht, const Vector3<T>& lht)
{
	Vector3<T> result;
	result[0] = rht(0) * lht(0);
	result[1] = rht(1) * lht(1);
	result[2] = rht(2) * lht(2);
	return result;
}

template<typename T>
Vector3<T> operator*(const Vector3<T>& rht, const T& lht)
{
	Vector3<T> result;
	result[0] = rht(0) * lht;
	result[1] = rht(1) * lht;
	result[2] = rht(2) * lht;
	return result;
}

template<typename T>
Vector3<T> operator/(const Vector3<T>& rht, const Vector3<T>& lht)
{
	Vector3<T> result;
	result[0] = rht(0) / lht(0);
	result[1] = rht(1) / lht(1);
	result[2] = rht(2) / lht(2);
	return result;
}

template<typename T>
Vector3<T> operator/(const Vector3<T>& rht, const T& lht)
{
	Vector3<T> result;
	result[0] = rht(0) / lht;
	result[1] = rht(1) / lht;
	result[2] = rht(2) / lht;
	return result;
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& t)
{
	return (os<< t(0) << " " << t(1) << " " << t(2));
}

template<typename T>
T dot( const Vector3<T>& left, const Vector3<T>& right )
{
	return (left[0]*right(0) + left[1]*right(1) + left[2]*right(2));
}

template<typename T>
void cross( const Vector3<T>& left, const Vector3<T>& right, Vector3<T>& result )
{
	result[0] = left(1)*right(2)-left(2)*right(1);
	result[1] = left(2)*right(0)-left(0)*right(2);
	result[2] = left(0)*right(1)-left(1)*right(0);
}



}

#endif
