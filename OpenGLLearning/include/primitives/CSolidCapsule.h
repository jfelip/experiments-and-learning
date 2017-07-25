

#ifndef GLVIEWER_CSOLIDCAPSULE_H
#define GLVIEWER_CSOLIDCAPSULE_H

#include <GLGeometry.h>
#include <Material.hpp>
#include <math.h>
#include <memory>
#include <vector>

template<typename T_real=GLfloat, typename T_integer=GLuint>
class CSolidCapsule
{

    typedef std::shared_ptr<CSolidCapsule> Ptr;

    typedef std::shared_ptr<CSolidCapsule> ConstPtr;



protected:
    COpenGLGeometry<T_real,T_real,T_integer> m_geom;
    T_real m_radius;
    T_real m_height;
    T_integer m_resolution;

public:
    CSolidCapsule(T_real radius=1.0, T_real height=1.0, CTransform<T_real> t=CTransform<T_real>(), T_integer resolution=20)
    {
        m_radius = radius;
        m_height = height;
        m_resolution = resolution;
        setTransform(t);
        generateVertexData();
    }

    void setRadius(const T_real& r){m_radius = r;}
    void setHeight(const T_real& h){m_height = h;}
    void setResolution(T_integer resolution){m_resolution = resolution; generateVertexData();}
    void setTransform( const CTransform<T_real>& transform ) { m_geom.setTransform(transform);}

    T_real getRadius(){return m_radius;}
    T_real getHeight(){return m_height;}
    T_integer getResolution(){return m_resolution;}
    CTransform<T_real>& getTransform() {return m_geom.getTransform();}


    //TODO:Fix internal extra faces generated
    void generateVertexData()
    {
        std::vector<T_real> vertices;
        std::vector<T_integer> indices;
        std::vector<T_real> normals;
        std::vector<T_real> colors;
        std::vector<T_real> textureCoords;

        T_real x, y, z, alpha,beta; // Storage for coordinates and angles
        T_real incr = M_PI/m_resolution;
        T_real M_PI2 = 2*M_PI + incr;
        T_integer idx = 0;

        ////////////////////////////
        // DRAW THE TOP CAP
        ////////////////////////////
        for (alpha = 0; alpha < M_PI/2.0; alpha += M_PI/m_resolution)
        {
            for (beta = 0.0; beta < M_PI2; beta += M_PI/m_resolution)
            {
                x = m_radius*cos(beta)*sin(alpha);
                y = m_radius*sin(beta)*sin(alpha);
                z = m_radius*cos(alpha) + m_height/2;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
                indices.push_back(idx); idx++;

                x = m_radius*cos(beta)*sin(alpha + M_PI/m_resolution);
                y = m_radius*sin(beta)*sin(alpha + M_PI/m_resolution);
                z = m_radius*cos(alpha + M_PI/m_resolution) + m_height/2;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
                indices.push_back(idx); idx++;
            }
        }


        ////////////////////////////
        // DRAW THE SIDES
        ////////////////////////////
        for (alpha = 0.0; alpha < M_PI2; alpha += incr)
        {
            x = m_radius*sin(alpha);
            y = m_radius*cos(alpha);
            z = -m_height * 0.5;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            indices.push_back(idx); idx++;
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);

            //x = radius*sin(alpha);
            //y = radius*cos(alpha);
            z = m_height * 0.5;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            indices.push_back(idx); idx++;
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }

        ////////////////////////////
        // DRAW THE BOTTOM CAP
        ////////////////////////////
        for (alpha = M_PI/2.0; alpha < M_PI; alpha += M_PI/m_resolution)
        {
            for (beta = 0.0; beta < M_PI2; beta += M_PI/m_resolution)
            {
                x = m_radius*cos(beta)*sin(alpha);
                y = m_radius*sin(beta)*sin(alpha);
                z = m_radius*cos(alpha) - m_height/2;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
                indices.push_back(idx); idx++;

                x = m_radius*cos(beta)*sin(alpha + M_PI/m_resolution);
                y = m_radius*sin(beta)*sin(alpha + M_PI/m_resolution);
                z = m_radius*cos(alpha + M_PI/m_resolution) - m_height/2;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
                indices.push_back(idx); idx++;
            }
        }


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

#endif
