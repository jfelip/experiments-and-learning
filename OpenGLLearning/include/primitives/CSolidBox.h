

#ifndef GLVIEWER_CSOLIDBOX_H
#define GLVIEWER_CSOLIDBOX_H

#include <GLGeometry.h>
#include <Material.hpp>
#include <math.h>
#include <memory>
#include <vector>

template<typename T_real=float, typename T_integer=int>
class CSolidBox
{

    typedef std::shared_ptr<CSolidBox> Ptr;

    typedef std::shared_ptr<CSolidBox> ConstPtr;

protected:
    COpenGLGeometry<T_real,T_real,T_integer> m_geom;
    T_real m_dim[3];

public:
    CSolidBox(T_real d_x=1.0,T_real d_y=1.0,T_real d_z=1.0, const CTransform<T_real>& t=CTransform<T_real>())
    {
        m_dim[0] = d_x;
        m_dim[1] = d_y;
        m_dim[2] = d_z;
        generateVertexData();
        setTransform(t);
    }

    void setDimensions(T_real x,T_real y,T_real z){m_dim[0]= x;m_dim[1]= y; m_dim[2]= z;}
    void setTransform( const CTransform<T_real>& transform ) { m_geom.setTransform(transform);}

    T_real* getDimensions(){return m_dim;}
    CTransform<T_real>& getTransform() {return m_geom.getTransform();}

    void generateVertexData()
    {
        std::vector<T_real> vertices;
        std::vector<T_integer> indices;
        std::vector<T_real> normals;
        std::vector<T_real> colors;
        std::vector<T_real> textureCoords;

        vertices = { 			1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
                                1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
                                1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
                                -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
                                -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
                                1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back)

        // normal array
        normals = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
                    1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
                    0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
                    -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
                    0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
                    0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

        // index array of vertex array for glDrawElements() & glDrawRangeElement()
        indices  = { 0, 1, 2,   0, 2, 3,      // front
                     4, 5, 6,   6, 7, 4,      // right
                     8, 9,10,  10,11, 8,      // top
                     12,13,14,  14,15,12,      // left
                     16,17,18,  18,19,16,      // bottom
                     20,21,22,  22,23,20 };    //dim_x back


        T_real tex_multx = 1;
        T_real tex_multy = 1;
        T_real tex_multz = 1;

        textureCoords = { 0, 0,  tex_multx, 0,  tex_multx,tex_multy,   0,tex_multy,   // v0,v1,v2,v3 (front)
                          0, 0,  tex_multy, 0,  tex_multy,tex_multz,   0,tex_multz,   // v0,v3,v4,v5 (right)
                          0, 0,  tex_multz, 0,  tex_multz,tex_multx,   0,tex_multx,   // v0,v5,v6,v1 (top)
                          0, 0,  tex_multz, 0,  tex_multz,tex_multy,   0,tex_multy,   // v1,v6,v7,v2 (left)
                          0, 0,  tex_multx, 0,  tex_multx,tex_multz,   0,tex_multz,	// v7,v4,v3,v2 (bottom)
                          0, 0,  tex_multx, 0,  tex_multx,tex_multy,   0,tex_multy }; // v4,v7,v6,v5 (back)


        for (uint i=0; i<vertices.size(); i+=3)
        {
            vertices[i] = vertices[i] * m_dim[0] * 0.5;
            vertices[i+1] = vertices[i+1] * m_dim[1] * 0.5;
            vertices[i+2] = vertices[i+2] * m_dim[2] * 0.5;
        }

        m_geom.setDrawType(GL_TRIANGLES);
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
