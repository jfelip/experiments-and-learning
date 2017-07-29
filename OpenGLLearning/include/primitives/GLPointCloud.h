#ifndef GL_POINT_CLOUD_H
#define GL_POINT_CLOUD_H

#include <vector>
#include <memory>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Transform.h>
#include <physics/CParticle.hpp>

template<typename T_real=double, typename T_vertex=GLfloat>
class COpenGLPointCloud
{
	typedef std::shared_ptr<COpenGLPointCloud> Ptr;

	typedef std::shared_ptr<COpenGLPointCloud> ConstPtr;

public:
	COpenGLPointCloud();

    ~COpenGLPointCloud();

    bool updateBuffers();

    bool draw(Shader *shader);

    GLuint getPontSize(){ return m_pointSize;}

	void setPointSize(const GLuint& s){	m_pointSize = s; }

    std::vector<T_vertex>& getPoints() {return m_vertices;}

    std::vector<T_vertex>& getNormals() {return m_normals;}

    std::vector<T_vertex>& getColors() {return m_colors;}

    CTransform<T_real>& getTransform() {return m_transform;}

	void setPoints( const std::vector<T_vertex>& vertices ) {m_vertices = vertices;}

	void setPoints( const std::vector<PBD::CParticle<> >& vertices )
	{
		m_vertices.clear();
		for (const auto & p:vertices)
		{
			m_vertices.push_back( p.m_position(0) );
			m_vertices.push_back( p.m_position(1) );
			m_vertices.push_back( p.m_position(2) );
		}
	}

    void setNormals( const std::vector<T_vertex>& normals ) {m_normals = normals;}

    void setColors( const std::vector<T_vertex>& colors ) {m_colors = colors;}

    void setColor( const T_real& r,const T_real& g, const T_real& b )
    {
        m_colors.clear();
        for (const auto & p:m_vertices)
        {
            m_colors.push_back( r );
            m_colors.push_back( g );
            m_colors.push_back( b );
        }
    }

    void setColors( const std::vector<PBD::CParticle<> >& vertices )
    {
        m_colors.clear();
        for (const auto & p:vertices)
        {
            if (p.m_collision)
            {
                m_colors.push_back( .5 );
                m_colors.push_back( 0 );
                m_colors.push_back( 0 );
            }
            else
            {
                m_colors.push_back( 0 );
                m_colors.push_back( .5 );
                m_colors.push_back( 0 );
            }
        }
    }


    void setTransform( const CTransform<T_real>& transform ) { m_transform = transform;}

    void rotate (const T_real r[4])		{m_transform.rotate(r);}

    void translate (const T_real t[3])	{m_transform.translate(t);}


protected:
    std::vector<T_vertex> m_vertices;

    std::vector<T_vertex> m_normals;

    std::vector<T_vertex> m_colors;

    std::vector<T_vertex> m_vertexBufferData;

    GLuint m_pointSize;

    CTransform<T_real> m_transform;

    GLuint VBO, VAO;

};


template<class T_real, class T_vertex>
COpenGLPointCloud<T_real,T_vertex>::COpenGLPointCloud()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	m_pointSize = 1;
}

template<class T_real, class T_vertex>
COpenGLPointCloud<T_real,T_vertex>::~COpenGLPointCloud()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

template<class T_real, class T_vertex>
bool COpenGLPointCloud<T_real,T_vertex>::updateBuffers()
{

	//Vertices and indices must have data
	if (m_vertices.size()==0)
	{
		_GENERIC_ERROR_("Trying to update object buffers with no vertices");
		return false;
	}

	//TODO: Check attribute sizes and initialize them if necessary
	if (m_vertices.size() != m_normals.size())
	{
		_GENERIC_ERROR_("Vertices and normal sizes do not match. Filling with zeros.");
		m_normals.resize(m_vertices.size(),0);
	}

	if (m_vertices.size() != m_colors.size())
	{
		_GENERIC_ERROR_("Vertices and color sizes do not match. Filling with zeros.");
		m_colors.resize(m_vertices.size(),0);
	}

	m_vertexBufferData.clear();
	uint t=0;
	for (uint i=0; i<m_vertices.size(); i+=3)
	{
		m_vertexBufferData.push_back(m_vertices[i]);
		m_vertexBufferData.push_back(m_vertices[i+1]);
		m_vertexBufferData.push_back(m_vertices[i+2]);

		m_vertexBufferData.push_back(m_normals[i]);
		m_vertexBufferData.push_back(m_normals[i+1]);
		m_vertexBufferData.push_back(m_normals[i+2]);

		m_vertexBufferData.push_back(m_colors[i]);
		m_vertexBufferData.push_back(m_colors[i+1]);
		m_vertexBufferData.push_back(m_colors[i+2]);

		t += 2;
	}


	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T_vertex) * m_vertexBufferData.size(), m_vertexBufferData.data(), GL_STATIC_DRAW);

		//TODO: Change the GL_FLOAT type according to the T_vertex template
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(T_vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(T_vertex), (GLvoid*) (3 * sizeof(T_vertex)));
		glEnableVertexAttribArray(1);

		//color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(T_vertex), (GLvoid*) (6 * sizeof(T_vertex)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	return true;
}

template<class T_real, class T_vertex>
bool COpenGLPointCloud<T_real,T_vertex>::draw(Shader *shader)
{
    shader->Use();
    //Update transformation matrix
    m_transform.m_data.computeMatrix();

    GLuint pointSize = glGetUniformLocation(shader->Program, "pointSize");
    GLuint transformLoc = glGetUniformLocation(shader->Program, "model");
    glUniformMatrix4fv(transformLoc, 1,GL_FALSE, m_transform.m_data.m_pMatrix);
    glUniform1ui(pointSize,m_pointSize);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, m_vertices.size()/3);
    glBindVertexArray(0);

    return true;
}


#endif
