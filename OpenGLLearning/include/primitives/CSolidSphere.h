

#ifndef GLVIEWER_CSOLIDSPHERE_H
#define GLVIEWER_CSOLIDSPHERE_H

#include <GLGeometry.h>
#include <Material.hpp>
#include <math.h>
#include <memory>
#include <vector>

template<typename T_real=GLfloat, typename T_integer=GLuint>
class CSolidSphere
{

    typedef std::shared_ptr<CSolidSphere> Ptr;

    typedef std::shared_ptr<CSolidSphere> ConstPtr;


protected:
    COpenGLGeometry<T_real,T_real,T_integer> m_geom;
    T_real m_radius;
    T_integer m_alpha_resolution;
    T_integer m_beta_resolution;

public:
    CSolidSphere(T_real radius=1.0, const CTransform<T_real>& t=CTransform<T_real>(), T_integer alpha_resolution=20, T_integer beta_resolution=20)
    {
        m_radius = radius;
        m_alpha_resolution = alpha_resolution;
        m_beta_resolution = beta_resolution;
        setTransform(t);
        generateVertexData();
    }

    void setRadius(T_real r){m_radius = r;}
    void setAlphaResolution(T_integer alpha_resolution){m_alpha_resolution = alpha_resolution;}
    void setBetaResolution(T_integer beta_resolution){m_beta_resolution = beta_resolution;}
    void setTransform( const CTransform<T_real>& transform ) { m_geom.setTransform(transform);}

    T_real getRadius(){return m_radius;}
    T_integer getAlphaResolution(){return m_alpha_resolution;}
    T_integer getBetaResolution(){return m_beta_resolution;}
    CTransform<T_real>& getTransform() {return m_geom.getTransform();}

    void generateVertexData()
    {
        std::vector<T_real> vertices;
        std::vector<T_integer> indices;
        std::vector<T_real> normals;
        std::vector<T_real> colors;
        std::vector<T_real> textureCoords;

        T_real x, y, z, alpha, beta; // Storage for coordinates and angles
        T_real radius = m_radius;
        T_integer idx = 0;

        T_real alpha_incr = M_PI/m_alpha_resolution;
        T_real beta_incr  = M_PI/m_beta_resolution;
        T_integer M_PI2 = 2*M_PI+alpha_incr;
        T_integer M_PI1 = M_PI+beta_incr;

        //TODO: Correct sphere vertices generation
        for (alpha = 0.0; alpha < M_PI1; alpha +=alpha_incr)
        {
            for (beta = 0.0; beta < M_PI2; beta +=beta_incr)
            {
                x = radius*cos(beta)*sin(alpha);
                y = radius*sin(beta)*sin(alpha);
                z = radius*cos(alpha);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                indices.push_back(idx); ++idx;

                x = radius*cos(beta)*sin(alpha + M_PI/m_alpha_resolution);
                y = radius*sin(beta)*sin(alpha + M_PI/m_alpha_resolution);
                z = radius*cos(alpha + M_PI/m_alpha_resolution);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                indices.push_back(idx); ++idx;
            }
        }

        normals = vertices;

        m_geom.setDrawType(GL_TRIANGLE_STRIP);
        m_geom.setVertices(vertices);
        m_geom.setIndices(indices);
        m_geom.setNormals(normals);
        m_geom.updateBuffers();
    }

    void draw(Shader *s)
    {
        m_geom.draw(s);
    }
};


#endif //OPENGLLEARNING_CSOLIDSPHERE_H
