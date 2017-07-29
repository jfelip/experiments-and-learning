#ifndef GL_GEOMETRY_PRIMITIVES_H
#define GL_GEOMETRY_PRIMITIVES_H

#include <vector>
#include <memory>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Transform.h>
#include <CGLShader.hpp>

template<typename T_real=double, typename T_vertex=GLfloat, typename T_indices=GLuint>
class COpenGLGeometry
{
public:
	typedef std::shared_ptr<COpenGLGeometry> Ptr;

	typedef const std::shared_ptr<COpenGLGeometry> ConstPtr;

public:
	COpenGLGeometry(T_indices draw_type = GL_TRIANGLES);

    ~COpenGLGeometry();

    bool updateBuffers();

    bool draw(Shader *shader);

    std::vector<T_vertex>& getVertices() {return m_vertices;}

    std::vector<T_vertex>& getNormals() {return m_normals;}

    std::vector<T_indices>& getIndices() {return m_indices;}

    std::vector<T_vertex>& getTextureCoords() {return m_textureCoords;}

    std::vector<T_vertex>& getColors() {return m_colors;}

    CTransform<T_real>& getTransform() {return m_transform;}

    T_indices& getDrawType() {return m_draw_type;}

    void setVertices( const std::vector<T_vertex>& vertices ) {m_vertices = vertices;}

    void setNormals( const std::vector<T_vertex>& normals ) {m_normals = normals;}

    void setIndices( const std::vector<T_indices>& indices ) {m_indices = indices;}

    void setTextureCoords( const std::vector<T_vertex>& textureCoords ) {m_textureCoords = textureCoords;}

    void setColors( const std::vector<T_vertex>& colors ) {m_colors = colors;}

    void setTransform( const CTransform<T_real>& transform ) { m_transform = transform;}

    void rotate (const T_real r[4])		{m_transform.rotate(r);}

    void translate (const T_real t[3])	{m_transform.translate(t);}

    void setDrawType( const T_indices& draw_type) {m_draw_type = draw_type;}

protected:
    std::vector<T_vertex> m_vertices;

    std::vector<T_vertex> m_normals;

    std::vector<T_indices> m_indices;

    std::vector<T_vertex> m_textureCoords;

    std::vector<T_vertex> m_colors;

    std::vector<T_vertex> m_vertexBufferData;

    CTransform<T_real> m_transform;

    GLuint VBO, VAO, EBO;

    T_indices m_draw_type;
};


template<class T_real, class T_vertex, class T_indices>
COpenGLGeometry<T_real,T_vertex,T_indices>::COpenGLGeometry(T_indices draw_type)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	m_draw_type = draw_type;
}

template<class T_real, class T_vertex, class T_indices>
COpenGLGeometry<T_real,T_vertex,T_indices>::~COpenGLGeometry()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

template<class T_real, class T_vertex, class T_indices>
bool COpenGLGeometry<T_real,T_vertex,T_indices>::updateBuffers()
{

	//Vertices and indices must have data
	if (m_vertices.size()==0 || m_indices.size()==0)
	{
		_GENERIC_ERROR_("Trying to update object buffers with no vertices or indices");
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

	if (m_vertices.size()/3 != m_textureCoords.size()/2)
	{
		_GENERIC_ERROR_("Vertices and texureCoord sizes do not match. Filling with zeros.");
		m_textureCoords.resize(2*(m_vertices.size()/3),0);
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

		m_vertexBufferData.push_back(m_textureCoords[t]);
		m_vertexBufferData.push_back(m_textureCoords[t+1]);
		t += 2;
	}


	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T_vertex) * m_vertexBufferData.size(), m_vertexBufferData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(T_indices) * m_indices.size() , m_indices.data(), GL_STATIC_DRAW);

		//TODO: Change the GL_FLOAT type according to the T_vertex template
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(T_vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(T_vertex), (GLvoid*) (3 * sizeof(T_vertex)));
		glEnableVertexAttribArray(1);

		//color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(T_vertex), (GLvoid*) (6 * sizeof(T_vertex)));
		glEnableVertexAttribArray(2);

		//texture
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(T_vertex), (GLvoid*) (9 * sizeof(T_vertex)));
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	return true;
}

template<class T_real, class T_vertex, class T_indices>
bool COpenGLGeometry<T_real,T_vertex,T_indices>::draw(Shader *shader)
{
	shader->Use();

	//Update transformation matrix
	m_transform.m_data.computeMatrix();

	GLuint transformLoc = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(transformLoc, 1,GL_FALSE, m_transform.m_data.m_pMatrix);

	//TODO: Calculate the normal matrix to avoid the problem of scaling normals with not iso-scaling factors
	GLuint transformNorm = glGetUniformLocation(shader->Program, "normalMatrix");
	glUniformMatrix4fv(transformNorm, 1,GL_FALSE, m_transform.m_data.m_pMatrix);

    glBindVertexArray(VAO);
    //TODO: Change the GL_UNSIGNED_INT type according to the T_index template
    glDrawElements(m_draw_type, m_vertices.size() , GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

	return true;
}


#endif
