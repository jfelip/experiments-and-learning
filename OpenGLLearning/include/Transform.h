#ifndef GL_VIEWER_TRANSFORM_H
#define GL_VIEWER_TRANSFORM_H

#include <memory>
#include <vector>
#include <math.h>

template<class T_real=double>
class CTransform
{
	typedef std::shared_ptr<CTransform> Ptr;

	typedef std::shared_ptr<CTransform> ConstPtr;

public:
	CTransform();

	CTransform(const CTransform& r);

	~CTransform();

	CTransform& operator=(const CTransform& r);

	CTransform& operator*(const CTransform& r);

	void computeMatrix();

	void setIdentity();

	T_real m_pPos[3];

	T_real m_pQua[4];

	T_real m_pMatrix[16];

};

template<class T>
void CTransform<T>::computeMatrix()
{

    T norm = sqrt (m_pQua[0]*m_pQua[0] + m_pQua[1]*m_pQua[1] + m_pQua[2]*m_pQua[2] + m_pQua[3]* m_pQua[3] );

    if (norm != 0)
    {
		m_pQua[0] /= norm;
		m_pQua[1] /= norm;
		m_pQua[2] /= norm;
		m_pQua[3] /= norm;
    }
    else
    {
    	_GENERIC_ERROR_("Invalid quaternion. Using identity");
    	m_pQua[0] = 0;
    	m_pQua[1] = 0;
    	m_pQua[2] = 0;
    	m_pQua[3] = 1;
    }

    //(b,c,d,a)
    //(0,1,2,3)

	//Row 1 (0,0) (0,1) (0,2) (0,3)
	m_pMatrix[0] = m_pQua[3]*m_pQua[3] + m_pQua[0]*m_pQua[0] - m_pQua[1]*m_pQua[1] - m_pQua[2]*m_pQua[2];
	m_pMatrix[1] = 2*m_pQua[0]*m_pQua[1] - 2*m_pQua[3]*m_pQua[2];
	m_pMatrix[2] = 2*m_pQua[0]*m_pQua[2] + 2*m_pQua[3]*m_pQua[1];
	m_pMatrix[3] = 0;

	//Column 2 (1,0) (1,1) (1,2) (1,3)
	m_pMatrix[4] = 2*m_pQua[0]*m_pQua[1] + 2*m_pQua[3]*m_pQua[2];
	m_pMatrix[5] = m_pQua[3]*m_pQua[3] - m_pQua[0]*m_pQua[0] + m_pQua[1]*m_pQua[1] - m_pQua[2]*m_pQua[2];
	m_pMatrix[6] = 2*m_pQua[1]*m_pQua[2] - 2*m_pQua[3]*m_pQua[0];
	m_pMatrix[7] = 0;

	//Column 3 (2,0) (2,1) (2,2) (2,3)
	m_pMatrix[8] =  2*m_pQua[0]*m_pQua[2] - 2*m_pQua[3]*m_pQua[1];
	m_pMatrix[9] =  2*m_pQua[1]*m_pQua[2] + 2*m_pQua[3]*m_pQua[0];
	m_pMatrix[10] = m_pQua[3]*m_pQua[3] - m_pQua[0]*m_pQua[0] - m_pQua[1]*m_pQua[1] + m_pQua[2]*m_pQua[2];
	m_pMatrix[11] = 0;

	//Column 4 (3,0) (3,1) (3,2) (3,3)
	m_pMatrix[12] = m_pPos[0];
	m_pMatrix[13] = m_pPos[1];
	m_pMatrix[14] = m_pPos[2];
	m_pMatrix[15] = 1;

}

template<class T>
void CTransform<T>::setIdentity()
{
	//Row 1 (0,0) (0,1) (0,2) (0,3)
	m_pMatrix[0] = 1;
	m_pMatrix[1] = 0;
	m_pMatrix[2] = 0;
	m_pMatrix[3] = 0;
	//Column 2 (1,0) (1,1) (1,2) (1,3)
	m_pMatrix[4] = 0;
	m_pMatrix[5] = 1;
	m_pMatrix[6] = 0;
	m_pMatrix[7] = 0;
	//Column 3 (2,0) (2,1) (2,2) (2,3)
	m_pMatrix[8] =  0;
	m_pMatrix[9] =  0;
	m_pMatrix[10] = 1;
	m_pMatrix[11] = 0;
	//Column 4 (3,0) (3,1) (3,2) (3,3)
	m_pMatrix[12] = 0;
	m_pMatrix[13] = 0;
	m_pMatrix[14] = 0;
	m_pMatrix[15] = 1;

	m_pPos[0] = 0;
	m_pPos[1] = 0;
	m_pPos[2] = 0;

	m_pQua[0] = 0;
	m_pQua[1] = 0;
	m_pQua[2] = 0;
	m_pQua[3] = 1;
}

template<class T>
CTransform<T>::CTransform()
{
	for(uint i=0 ; i<3; ++i)
	{
		m_pPos[i] = 0;
		m_pQua[i] = 0;
	}
	m_pQua[3] = 1;
	computeMatrix();
}

template<class T>
CTransform<T>::~CTransform()
{

}

template<class T>
CTransform<T>::CTransform(const CTransform& r)
{
	for(uint i=0 ; i<3; ++i)
	{
		m_pPos[i] = r.m_pPos[i];
		m_pQua[i] = r.m_pQua[i];
	}
	m_pQua[3] = r.m_pQua[3];

	computeMatrix();

}


template<class T>
CTransform<T>& CTransform<T>::operator=(const CTransform<T>& r)
{
	for(uint i=0 ; i<3; ++i)
	{
		m_pPos[i] = r.m_pPos[i];
		m_pQua[i] = r.m_pQua[i];
	}
	m_pQua[3] = r.m_pQua[3];

	computeMatrix();

	return *this;
}

template<class T>
CTransform<T>& CTransform<T>::operator*(const CTransform<T>& r)
{
	CTransform c;

	//Translation adds
	for(uint i=0 ; i<3; ++i)
	{
		c.m_pPos[i] = m_pPos[i] + r.m_pPos[i];
	}

	//Quaternion product
	c.m_pPos[0] =  m_pQua[0] * r.m_pQua[3] + m_pQua[1] * r.m_pQua[2] - m_pQua[2] * r.m_pQua[1] + m_pQua[3] * r.m_pQua[0];
	c.m_pPos[1] = -m_pQua[0] * r.m_pQua[2] + m_pQua[1] * r.m_pQua[3] + m_pQua[2] * r.m_pQua[0] + m_pQua[3] * r.m_pQua[1];
	c.m_pPos[2] =  m_pQua[0] * r.m_pQua[1] - m_pQua[1] * r.m_pQua[0] + m_pQua[2] * r.m_pQua[3] + m_pQua[3] * r.m_pQua[2];
	c.m_pPos[3] = -m_pQua[0] * r.m_pQua[0] - m_pQua[1] * r.m_pQua[1] - m_pQua[2] * r.m_pQua[2] + m_pQua[3] * r.m_pQua[3];

	c.computeMatrix();

	return c;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const CTransform<T>& t)
{
	os << t.m_pPos[0] << " " << t.m_pPos[1] << " " << t.m_pPos[2] << " " <<
			t.m_pQua[0] << " " << t.m_pQua[1] << " " << t.m_pQua[2] << " " << t.m_pQua[3] <<std::endl;

	for (uint i=0; i<16; i+=4)
	{
		os << t.m_pMatrix[i] << " " << t.m_pMatrix[i+1] << " " << t.m_pMatrix[i+2] << " " << t.m_pMatrix[i+3] << std::endl;
	}
	return os;
}



#endif
