

#ifndef GLVIEWER_CSOLIDCYLINDER_H
#define GLVIEWER_CSOLIDCYLINDER_H

#include <GLGeometry.h>
#include <Material.hpp>
#include <math.h>
#include <memory>
#include <vector>

template<typename T_real=GLfloat, typename T_integer=GLuint>
class CSolidCylinder
{

    typedef std::shared_ptr<CSolidCylinder> Ptr;

    typedef std::shared_ptr<CSolidCylinder> ConstPtr;



protected:
    COpenGLGeometry<T_real,T_real,T_integer> m_geom;
    T_real m_radius;
    T_real m_height;
    T_integer m_resolution;

public:
    CSolidCylinder(T_real radius=1.0, T_real height=1.0, const CTransform<T_real>& t=CTransform<T_real>(), T_integer resolution=20)
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

    void rotate (const T_real r[4])		{m_geom.rotate(r);}
    void translate (const T_real t[4])	{m_geom.translate(t);}

    //TODO:Fix extra faces generated
    void generateVertexData()
    {
        std::vector<T_real> vertices;
        std::vector<T_integer> indices;
        std::vector<T_real> normals;
        std::vector<T_real> colors;
        std::vector<T_real> textureCoords;

        ////////////////////////////
        // DRAW THE SIDES
        ////////////////////////////
        T_real x, y, z, alpha; // Storage for coordinates and angles
        T_real incr = M_PI/m_resolution;
        T_real M_PI2 = 2*M_PI + incr;
        T_integer idx = 0;
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
        // DRAW THE CAPS
        ////////////////////////////
        //Center top cap vertex
        z = +m_height * 0.5;
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(z);
        T_integer idx_center = idx;
        indices.push_back(idx);idx++;
        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(z);
        for (alpha = 0.0; alpha < M_PI2; alpha += incr)
        {
            x = m_radius*sin(alpha);
            y = m_radius*cos(alpha);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            indices.push_back(idx);idx++;
            normals.push_back(0);
            normals.push_back(0);
            normals.push_back(z);

            indices.push_back(idx_center);
        }
        indices.pop_back();

        //Center bottom cap vertex
        z = -m_height * 0.5;
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(z);
        idx_center = idx;
        indices.push_back(idx);idx++;
        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(z);
        for (alpha = 0.0; alpha < M_PI2; alpha += incr)
        {
            x = m_radius*sin(alpha);
            y = m_radius*cos(alpha);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            indices.push_back(idx);idx++;
            normals.push_back(0);
            normals.push_back(0);
            normals.push_back(z);

            indices.push_back(idx_center);
        }
        indices.pop_back();

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
