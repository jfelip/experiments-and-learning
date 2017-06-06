#ifndef _CREAL_H_
#define _CREAL_H_

#include <limits>



template <class T_real=double>
class CReal
{
public:
	//Numeric limits
	static T_real max = std::numeric_limits<T_real>::max();
	static T_real min = std::numeric_limits<T_real>::lowest();
	static T_real e = std::numeric_limits<T_real>::epsilon();
	static T_real inf = std::numeric_limits<T_real>::infinity();
	static T_real NaN = std::numeric_limits<T_real>::quiet_NaN();

public:
	T_real m_value = T_real(0);

	//Constructors
	CReal(const T_real& val){m_value = val;}
	CReal(const CReal& val)	{*this = val;}
	CReal(){m_value = T_real(0);}
	~CReal(){}

	//Casting operators
	operator T_real() const {return m_value;}
	operator float() const {return float(m_value);}
	operator double() const {return double(m_value);}
	operator int() const {return int(m_value);}

	//Assignment operators
	CReal<T_real>& operator= (const CReal<T_real>& rhs){ m_value = rhs.m_value; return *this;}
	CReal<T_real>& operator+=(const CReal<T_real>& rhs){ m_value = m_value + rhs.m_value; return *this;}
	CReal<T_real>& operator-=(const CReal<T_real>& rhs){ m_value = m_value - rhs.m_value; return *this;}
	CReal<T_real>& operator/=(const CReal<T_real>& rhs){ m_value = m_value / rhs.m_value; return *this;}
	CReal<T_real>& operator*=(const CReal<T_real>& rhs){ m_value = m_value * rhs.m_value; return *this;}

	//Increment operators do not make sense to implement in real numbers
//	CReal<T_real>& operator++();	///< Prefix increment operator
//	CReal<T_real>& operator--();	///< Prefix decrement operator
//	CReal<T_real>& operator++(int); ///< Postfix increment operator
//	CReal<T_real>& operator--(int); ///< Postfix decrement operator

	//Relational operators
	inline bool operator==(const CReal<T_real>& rhs){ return (m_value < (rhs.m_value + CReal::e)) && (m_value > (rhs.m_value - CReal::e));}
	inline bool operator!=(const CReal<T_real>& rhs){ return !(*this==rhs); }
	inline bool operator> (const CReal<T_real>& rhs){ return m_value > (rhs.m_value + CReal::e);}
	inline bool operator>=(const CReal<T_real>& rhs){ return (*this>rhs) || (*this==rhs);}
	inline bool operator< (const CReal<T_real>& rhs){ return m_value < (rhs.m_value - CReal::e);}
	inline bool operator<=(const CReal<T_real>& rhs){ return (*this<rhs) || (*this==rhs);}

	//Arithmetic operators
	inline CReal<T_real> operator+(const CReal<T_real>& rhs) const {return CReal(m_value + rhs.m_value);}
	inline CReal<T_real> operator-(const CReal<T_real>& rhs) const {return CReal(m_value - rhs.m_value);}
	inline CReal<T_real> operator*(const CReal<T_real>& rhs) const {return CReal(m_value * rhs.m_value);}
	inline CReal<T_real> operator/(const CReal<T_real>& rhs) const {return CReal(m_value / rhs.m_value);}


};

#endif

