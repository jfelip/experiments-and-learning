

#ifndef GLVIEWER_CREFERENCEFRAME_H
#define GLVIEWER_CREFERENCEFRAME_H

#include <math.h>
#include <memory>
#include <vector>

#include <Material.hpp>
#include <GLGeometry.h>
#include <primitives/CSolidArrow.h>

template<typename T_real=GLfloat, typename T_integer=GLuint>
class CReferenceFrame
{

    typedef std::shared_ptr<CReferenceFrame> Ptr;

    typedef std::shared_ptr<CReferenceFrame> ConstPtr;

protected:
    CSolidArrow<T_real,T_integer> m_xaxis;
    CSolidArrow<T_real,T_integer> m_yaxis;
    CSolidArrow<T_real,T_integer> m_zaxis;
    CTransform<T_real> m_xaxis_transform;
    CTransform<T_real> m_yaxis_transform;
    CMaterial<T_real> matX;
    CMaterial<T_real> matY;
    CMaterial<T_real> matZ;

    T_real m_length;

public:
    CReferenceFrame(T_real length=1.0, CTransform<T_real> t=CTransform<T_real>(), T_integer resolution=2)
    {
        setLength(length);
        setTransform(t);
        generateVertexData();
        matX.setSpecular(1.0,0.0,0.0);
        matX.setDiffuse (1.0,0.0,0.0);
        matX.shininess = 0.1;
        matY.setSpecular(0.0,1.0,0.0);
        matY.setDiffuse (0.0,1.0,0.0);
        matY.shininess = 0.1;
        matZ.setSpecular(0.0,0.0,1.0);
        matZ.setDiffuse (0.0,0.0,1.0);
        matZ.shininess = 0.1;
        setResolution(resolution);
    }

    T_real getLenght(){ return m_length;}

    void setLength(const T_real& len)
    {
        m_length = len;
        m_xaxis.setHeadHeight(m_length*0.2);
        m_xaxis.setHeadRadius(m_length*0.05);
        m_xaxis.setBodyRadius(m_length*0.01);
        m_xaxis.setBodyHeight(m_length*0.8);

        m_yaxis.setHeadHeight(m_length*0.2);
        m_yaxis.setHeadRadius(m_length*0.05);
        m_yaxis.setBodyRadius(m_length*0.01);
        m_yaxis.setBodyHeight(m_length*0.8);

        m_zaxis.setHeadHeight(m_length*0.2);
        m_zaxis.setHeadRadius(m_length*0.05);
        m_zaxis.setBodyRadius(m_length*0.01);
        m_zaxis.setBodyHeight(m_length*0.8);
    }

    void setResolution(const T_integer& resolution)
    {
        m_xaxis.setResolution(resolution);
        m_yaxis.setResolution(resolution);
        m_zaxis.setResolution(resolution);
    }

    void setTransform( const CTransform<T_real>& transform )
    {
        CTransform<T_real> t_aux1 = transform;
        t_aux1.rotateY(M_PI*0.5);
        m_xaxis.setTransform(t_aux1);

        CTransform<T_real> t_aux2 = transform;
        t_aux2.rotateX(-M_PI*0.5);
        m_yaxis.setTransform(t_aux2);

        m_zaxis.setTransform(transform);
    }

    T_integer getResolution(){return m_zaxis.getResolution();}

    CTransform<T_real>& getTransform() {return m_zaxis.getTransform();}

    void generateVertexData()
    {
        m_xaxis.generateVertexData();
        m_yaxis.generateVertexData();
        m_zaxis.generateVertexData();
    }

    void draw(Shader *s)
    {
        matX.use(s);
        m_xaxis.draw(s);
        matY.use(s);
        m_yaxis.draw(s);
        matZ.use(s);
        m_zaxis.draw(s);
    }
};

#endif //OPENGLLEARNING_CSOLIDSPHERE_H
