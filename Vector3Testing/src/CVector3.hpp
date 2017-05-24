#ifndef _CVECTOR3_H_
#define _CVECTOR3_H_

namespace Vector3
{

	typedef double TReal;
	typedef double Vector3[3];

	TReal dot(const Vector3 a, const Vector3 b){}

	void cross(const Vector3 a, const Vector3 b,Vector3 c){}

	void add(const Vector3 a, const Vector3 b,Vector3 c){}

	void add(const Vector3 a, const TReal b,Vector3 c){}

	void subtract(const Vector3 a, const Vector3 b,Vector3 c){}

	void subtract(const Vector3 a, const TReal b,Vector3 c){}

	TReal norm(const Vector3 a){}

	TReal squared_norm(const Vector3 a){}

	void normalize(const Vector3 a, Vector3 b){}

	void assign(Vector3 a, Vector3 b){}

	void assign(Vector3 a,TReal b){}

	void inverse(Vector3 a)
	{
		a[0] = -a[0];
		a[1] = -a[1];
		a[2] = -a[2];
	}

	void element_product(const Vector3 a, TReal b, Vector3 c){}

	void element_divide(const Vector3 a, TReal b, Vector3 c){}

	void element_product(const Vector3 a, const Vector3 b, Vector3 c){}

	void element_divide(const Vector3 a, const Vector3 b, Vector3 c){}


	template<class T=double>
	class CVector3
	{
	 protected:
	    T m_x, m_y, m_z;

	 public:

	    CVector3& operator=(const T& other)
	    {
	    	m_x = other;
	    	m_y = other;
	    	m_z = other;
	    	return *this;
	    }

	    void inverse()
	    {
	        m_x = -m_x;
	        m_y = -m_y;
	        m_z = -m_z;
	    }
	};
}

#endif
