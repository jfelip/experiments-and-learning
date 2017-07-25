

#ifndef GLVIEWER_CSOLIDARROW_H
#define GLVIEWER_CSOLIDARROW_H

#include <math.h>
#include <memory>
#include <vector>

#include <Material.hpp>
#include <GLGeometry.h>
#include <primitives/CSolidCone.h>
#include <primitives/CSolidCylinder.h>


template<typename T_real=GLfloat, typename T_integer=GLuint>
class CSolidArrow
{

    typedef std::shared_ptr<CSolidArrow> Ptr;

    typedef std::shared_ptr<CSolidArrow> ConstPtr;

protected:
    CSolidCone<T_real,T_integer> m_head;
    CSolidCylinder<T_real,T_integer> m_body;

public:
    CSolidArrow(T_real head_radius=0.2, T_real head_height=0.2,T_real body_radius=0.1, T_real body_height=0.8, CTransform<T_real> t=CTransform<T_real>(), T_integer resolution=20)
    {
        m_body.setRadius(body_radius);
        m_body.setHeight(body_height);
        m_body.setResolution(resolution);

        m_head.setRadius(head_radius);
        m_head.setHeight(head_height);
        m_head.setResolution(resolution);

        setTransform(t);
        generateVertexData();
    }

    void setBodyRadius(const T_real& r){m_body.setRadius(r);}

    void setHeadRadius(const T_real& r){m_head.setRadius(r);}

    void setBodyHeight(const T_real& h){m_body.setHeight(h);}

    void setHeadHeight(const T_real& h){m_head.setHeight(h);}

    void setResolution(const T_integer& resolution)
    {
        m_body.setResolution(resolution);
        m_head.setResolution(resolution);
    }

    void setTransform( const CTransform<T_real>& transform )
    {
        CTransform<T_real> t1;
        t1.translateZ(m_body.getHeight()*0.5);

        CTransform<T_real> t2;
        t2.translateZ(m_body.getHeight());

        m_body.setTransform(transform*t1);
        m_head.setTransform(transform*t2);
    }

    T_real getHeadRadius(){return m_head.getRadius();}

    T_real getBodyRadius(){return m_body.getRadius();}

    T_real getHeadHeight(){return m_head.getHeight();}

    T_real getBodyHeight(){return m_body.getHeight();}

    T_integer getResolution(){return m_body.getResolution();}

    CTransform<T_real>& getTransform(){	return m_body.getTransform(); }

    void generateVertexData()
    {
        m_head.generateVertexData();
        m_body.generateVertexData();
    }

    void draw(Shader *s)
    {
        m_head.draw(s);
        m_body.draw(s);
    }
};


#endif //OPENGLLEARNING_CSOLIDSPHERE_H
