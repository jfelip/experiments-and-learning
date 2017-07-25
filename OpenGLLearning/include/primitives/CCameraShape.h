
#ifndef GLVIEWER_CCAMERASHAPE_H
#define GLVIEWER_CCAMERASHAPE_H

#include <math.h>
#include <memory>
#include <vector>

#include <Material.hpp>
#include <GLGeometry.h>
#include <primitives/CSolidBox.h>
#include <primitives/CSolidCone.h>

//TODO: Change the cone shape to implement the aspect ratio of the camera
//TODO: Add a plane to visualize an image as a texture
template<typename T_real=GLfloat, typename T_integer=GLuint>
class CCameraShape
{

    typedef std::shared_ptr<CCameraShape> Ptr;

    typedef std::shared_ptr<CCameraShape> ConstPtr;

protected:
    CSolidBox<T_real,T_integer> m_box;
    CSolidCone<T_real,T_integer> m_frustrum;
    T_real m_size;
    T_real m_aspectRatio;

public:
    CCameraShape(T_real size=1.0, T_real aspect_ratio=1.0, CTransform<T_real> t=CTransform<T_real>())
    {
        m_frustrum.setResolution(2);
        setSize(size);
        setAspectRatio(aspect_ratio);
        setTransform(t);
        generateVertexData();
    }

    T_real getSize(){ return m_size;}

    void setAspectRatio(const T_real& s)
    {
        m_aspectRatio = s;
    }

    T_real getAspectRatio(){ return m_aspectRatio;}

    void setSize(const T_real& s)
    {
        m_size = s;
        m_box.setDimensions(s*0.2,s*0.2,s*0.2);
        m_frustrum.setHeight(s);
        m_frustrum.setRadius(s);
        generateVertexData();
        setTransform(getTransform());
    }

    void setTransform( const CTransform<T_real>& transform )
    {
        m_box.setTransform(transform);

        CTransform<T_real> t_aux2 = transform;
        t_aux2.translateZ(m_size);
        t_aux2.rotateX(M_PI);
        t_aux2.rotateZ(M_PI*0.25);
        m_frustrum.setTransform(t_aux2);
    }

    CTransform<T_real>& getTransform() {return m_box.getTransform();}

    void generateVertexData()
    {
        m_box.generateVertexData();
        m_frustrum.generateVertexData();
    }

    void draw(Shader *s)
    {
        m_box.draw(s);
        m_frustrum.draw(s);
    }
};


#endif //GLVIEWER_CCAMERASHAPE_H
