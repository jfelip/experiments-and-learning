#ifndef GLVIEWER_PARTICLE_SYSTEM_H
#define GLVIEWER_PARTICLE_SYSTEM_H

#include <vector>
#include <memory>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Transform.h>
#include <physics/CWorld.h>

template<typename T_real=double, typename T_vertex=GLfloat>
class CGLParticleSystem
{
	typedef std::shared_ptr<CGLParticleSystem> Ptr;

	typedef std::shared_ptr<CGLParticleSystem> ConstPtr;

public:
    CGLParticleSystem();

    ~CGLParticleSystem();

    bool updateBuffers();

    bool updateConstraintBuffers();

    bool draw(Shader *shader);

    GLuint getPontSize(){ return m_pointSize;}

	void setPointSize(const GLuint& s){	m_pointSize = s; }

    PBD::CWorld* getParticleSystem() {return m_pPSystem;}

    void setParticleSystem( PBD::CWorld* pS) {m_pPSystem = pS;}

    std::vector<T_vertex>& getColors() {return m_colors;}

    CTransform<T_real>& getTransform() {return m_transform;}

    void setColors( const std::vector<T_vertex>& colors ) {m_colors = colors;}

    void setColor( const T_real& r,const T_real& g, const T_real& b )
    {
        m_colors.clear();
        for (uint p=0; p<m_pPSystem->m_particles.size() ; ++p)
        {
            m_colors.push_back( r );
            m_colors.push_back( g );
            m_colors.push_back( b );
        }
    }

    void setColors( const std::vector<PBD::CParticle<> >& vertices )
    {
        m_colors.clear();
        for (uint p=0; p<vertices.size() ; ++p)
        {
            if (vertices[p].m_collision)
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

    void showConstraints( bool c) { m_drawConstraints = c; }

protected:
    PBD::CWorld* m_pPSystem;

    std::vector<T_vertex> m_colors;

    std::vector<T_vertex> m_vertexBufferData;

    GLuint m_pointSize;

    CTransform<T_real> m_transform;

    GLuint VBO, VAO;

    GLuint VBOConstraints, VAOConstraints;

    bool m_drawConstraints = false;

};


template<class T_real, class T_vertex>
CGLParticleSystem<T_real,T_vertex>::CGLParticleSystem()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAOConstraints);
    glGenBuffers(1, &VBOConstraints);
	m_pointSize = 1;
}

template<class T_real, class T_vertex>
CGLParticleSystem<T_real,T_vertex>::~CGLParticleSystem()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAOConstraints);
    glDeleteBuffers(1, &VBOConstraints);
}

template<class T_real, class T_vertex>
bool CGLParticleSystem<T_real,T_vertex>::updateBuffers()
{

    if( m_colors.size()*3 != m_pPSystem->m_particles.size())
    {
        m_colors.resize(m_pPSystem->m_particles.size()*3,0);

        uint i=0;
        for (uint p=0; p<m_pPSystem->m_particles.size() ; ++p)
        {
            std::default_random_engine rd(m_pPSystem->m_particles[p]->m_group+2);
            std::uniform_real_distribution<GLfloat> dist(0.0, 1.0);
            m_colors[i] = dist(rd);
            m_colors[i+1] = dist(rd);
            m_colors[i+2] = dist(rd);
            i+=3;
        }
    }

    m_vertexBufferData.clear();
	uint i=0;
    for (uint p=0; p<m_pPSystem->m_particles.size() ; ++p)
    {
        m_vertexBufferData.push_back(m_pPSystem->m_particles[p]->m_position(0));
        m_vertexBufferData.push_back(m_pPSystem->m_particles[p]->m_position(1));
        m_vertexBufferData.push_back(m_pPSystem->m_particles[p]->m_position(2));

		m_vertexBufferData.push_back(m_colors[i]);
		m_vertexBufferData.push_back(m_colors[i+1]);
		m_vertexBufferData.push_back(m_colors[i+2]);

		i += 3;
	}


	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T_vertex) * m_vertexBufferData.size(), m_vertexBufferData.data(), GL_STATIC_DRAW);

		//TODO: Change the GL_FLOAT type according to the T_vertex template
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(T_vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(T_vertex), (GLvoid*) (3 * sizeof(T_vertex)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


    if (m_drawConstraints)
    {
        updateConstraintBuffers();
    }

	return true;
}

template<class T_real, class T_vertex>
bool CGLParticleSystem<T_real,T_vertex>::updateConstraintBuffers()
{
    m_vertexBufferData.clear();
    for (uint p=0; p<m_pPSystem->m_constraints.size() ; ++p)
    {
        m_vertexBufferData.push_back(m_pPSystem->m_constraints[p]->m_particles[0]->m_position(0));
        m_vertexBufferData.push_back(m_pPSystem->m_constraints[p]->m_particles[0]->m_position(1));
        m_vertexBufferData.push_back(m_pPSystem->m_constraints[p]->m_particles[0]->m_position(2));

        if (m_pPSystem->m_constraints[p]->isSatisfied())
        {
            m_vertexBufferData.push_back(0);
            m_vertexBufferData.push_back(1);
            m_vertexBufferData.push_back(0);
        }
        else
        {
            m_vertexBufferData.push_back(1);
            m_vertexBufferData.push_back(0);
            m_vertexBufferData.push_back(0);
        }

        m_vertexBufferData.push_back(m_pPSystem->m_constraints[p]->m_particles[1]->m_position(0));
        m_vertexBufferData.push_back(m_pPSystem->m_constraints[p]->m_particles[1]->m_position(1));
        m_vertexBufferData.push_back(m_pPSystem->m_constraints[p]->m_particles[1]->m_position(2));

        if (m_pPSystem->m_constraints[p]->isSatisfied())
        {
            m_vertexBufferData.push_back(0);
            m_vertexBufferData.push_back(1);
            m_vertexBufferData.push_back(0);
        }
        else
        {
            m_vertexBufferData.push_back(1);
            m_vertexBufferData.push_back(0);
            m_vertexBufferData.push_back(0);
        }
    }


    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAOConstraints);
    glBindBuffer(GL_ARRAY_BUFFER, VBOConstraints);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T_vertex) * m_vertexBufferData.size(), m_vertexBufferData.data(), GL_DYNAMIC_DRAW);

    //TODO: Change the GL_FLOAT type according to the T_vertex template
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(T_vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(T_vertex), (GLvoid*) (3 * sizeof(T_vertex)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    return true;
}


template<class T_real, class T_vertex>
bool CGLParticleSystem<T_real,T_vertex>::draw(Shader *shader)
{
    shader->Use();
    //Update transformation matrix
    m_transform.m_data.computeMatrix();

    GLuint pointSize = glGetUniformLocation(shader->Program, "pointSize");
    GLuint transformLoc = glGetUniformLocation(shader->Program, "model");
    glUniformMatrix4fv(transformLoc, 1,GL_FALSE, m_transform.m_data.m_pMatrix);
    glUniform1ui(pointSize,m_pointSize);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, m_pPSystem->m_particles.size());
    glBindVertexArray(0);

    if (m_drawConstraints)
    {
        glBindVertexArray(VAOConstraints);
        glDrawArrays(GL_LINES, 0, m_pPSystem->m_constraints.size()*2);
        glBindVertexArray(0);
    }

    return true;
}


#endif
