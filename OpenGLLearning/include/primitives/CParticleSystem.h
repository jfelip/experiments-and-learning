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

//TODO: Enable particle-wise sphere radius

template<typename T_real=double, typename T_vertex=GLfloat>
class CGLParticleSystem
{
public:
	typedef std::shared_ptr<CGLParticleSystem> Ptr;

	typedef std::shared_ptr<CGLParticleSystem> ConstPtr;

    typedef enum
    {
        POINTS,
        SPHERES
    } DRAW_PRIMITIVE;

public:
    CGLParticleSystem();

    ~CGLParticleSystem();

    bool updateBuffers();

    bool draw(Shader *shader, Shader *constraintShader);

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

    void setTransform( const CTransform<T_real>& transform ) { m_transform = transform;}

    void rotate (const T_real r[4])		{m_transform.rotate(r);}

    void translate (const T_real t[3])	{m_transform.translate(t);}

    void showConstraints( bool c) { m_drawConstraints = c; }

    void setDrawPrimitive( DRAW_PRIMITIVE p ){ m_drawPrimitive = p; }

    DRAW_PRIMITIVE getDrawPrimitive( ){ return m_drawPrimitive; }

    bool addEigenVectorsAndCoMtoVertexBuffer( Eigen::Vector3d CoM, Eigen::Matrix3d cov, T_real colorMult = T_real(1.0), const T_real length = T_real(1.0));

protected:
    bool updateBuffersPoints();

    bool updateConstraintBuffers();

    PBD::CWorld* m_pPSystem;

    std::vector<T_vertex> m_colors;

    std::vector<T_vertex> m_vertexBufferData;

    std::vector<T_vertex> m_vertexBufferDataPoints;

    T_real m_pointSize;

    CTransform<T_real> m_transform;

    GLuint VBO, VAO;

    GLuint VBOConstraints, VAOConstraints;

    bool m_drawConstraints = false;

    DRAW_PRIMITIVE m_drawPrimitive = DRAW_PRIMITIVE::POINTS;

    std::vector<T_real> m_unitSphereVertices;
    std::vector<T_real> m_unitSphereNormals;
};


template<class T_real, class T_vertex>
CGLParticleSystem<T_real,T_vertex>::CGLParticleSystem()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAOConstraints);
    glGenBuffers(1, &VBOConstraints);
	m_pointSize = 1.0;

    //Create vertices for a sphere
    CSolidSphere<T_real,T_vertex> s;
    s.setRadius(1.0);
    s.setAlphaResolution(6);
    s.setBetaResolution (6);
    s.generateVertexData();
    m_unitSphereVertices = s.getVertices();
    m_unitSphereNormals = s.getNormals();

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
    updateBuffersPoints();

    if (m_drawConstraints)
    {
        updateConstraintBuffers();
    }

};


template<class T_real, class T_vertex>
bool CGLParticleSystem<T_real,T_vertex>::updateBuffersPoints()
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

    m_vertexBufferDataPoints.clear();
	uint i=0;
    for (uint p=0; p<m_pPSystem->m_particles.size() ; ++p)
    {
        m_vertexBufferDataPoints.push_back(m_pPSystem->m_particles[p]->m_position(0));
        m_vertexBufferDataPoints.push_back(m_pPSystem->m_particles[p]->m_position(1));
        m_vertexBufferDataPoints.push_back(m_pPSystem->m_particles[p]->m_position(2));

        m_vertexBufferDataPoints.push_back(m_colors[i]);
        m_vertexBufferDataPoints.push_back(m_colors[i+1]);
        m_vertexBufferDataPoints.push_back(m_colors[i+2]);

		i += 3;
	}

    //Show shape matching particle positions
    if (m_drawConstraints)
    {
        for (const auto& c:m_pPSystem->m_shapeMatchingConstraints)
        {
            for (const auto& p:c->m_shapeMatchingPositions)
            {
                Eigen::Vector3d pos = c->getDeformedCovMat() * p + c->getDeformedCoM();
                m_vertexBufferDataPoints.push_back(pos(0));
                m_vertexBufferDataPoints.push_back(pos(1));
                m_vertexBufferDataPoints.push_back(pos(2));

                m_vertexBufferDataPoints.push_back(1);
                m_vertexBufferDataPoints.push_back(0);
                m_vertexBufferDataPoints.push_back(0);
            }
        }
    }


	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T_vertex) * m_vertexBufferDataPoints.size(), m_vertexBufferDataPoints.data(), GL_STATIC_DRAW);

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
bool CGLParticleSystem<T_real,T_vertex>::updateConstraintBuffers()
{
    m_vertexBufferData.clear();

    //CREATE LINES FOR PARTICLE ORIENTATION
    for (uint p=0; p<m_pPSystem->m_particles.size() ; ++p)
    {
        if (m_pPSystem->m_particles[p]->getMass() > 0)
            addEigenVectorsAndCoMtoVertexBuffer(m_pPSystem->m_particles[p]->m_position, m_pPSystem->m_particles[p]->m_orientation.toRotationMatrix(), 0.5, m_pPSystem->m_particles[p]->m_size );
    }

    //CREATE LINES FOR CONTACT CONSTRAINTS
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

    //CREATE LINES FOR PERMANENT CONSTRAINTS
    for (uint p=0; p<m_pPSystem->m_permanentConstraints.size() ; ++p)
    {
        m_vertexBufferData.push_back(m_pPSystem->m_permanentConstraints[p]->m_particles[0]->m_position(0));
        m_vertexBufferData.push_back(m_pPSystem->m_permanentConstraints[p]->m_particles[0]->m_position(1));
        m_vertexBufferData.push_back(m_pPSystem->m_permanentConstraints[p]->m_particles[0]->m_position(2));

        if (m_pPSystem->m_permanentConstraints[p]->isSatisfied())
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

        m_vertexBufferData.push_back(m_pPSystem->m_permanentConstraints[p]->m_particles[1]->m_position(0));
        m_vertexBufferData.push_back(m_pPSystem->m_permanentConstraints[p]->m_particles[1]->m_position(1));
        m_vertexBufferData.push_back(m_pPSystem->m_permanentConstraints[p]->m_particles[1]->m_position(2));

        if (m_pPSystem->m_permanentConstraints[p]->isSatisfied())
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

    //CREATE LINES FOR SHAPE MATCHING CONSTRAINTS
    for (uint p=0; p<m_pPSystem->m_shapeMatchingConstraints.size() ; ++p)
    {

        addEigenVectorsAndCoMtoVertexBuffer( m_pPSystem->m_shapeMatchingConstraints[p]->getCoM(),
                                             m_pPSystem->m_shapeMatchingConstraints[p]->getCovMat(), 1.0  );

        addEigenVectorsAndCoMtoVertexBuffer( m_pPSystem->m_shapeMatchingConstraints[p]->getDeformedCoM()*1.01,
                                             m_pPSystem->m_shapeMatchingConstraints[p]->getDeformedCovMat(), 0.2  );

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
bool CGLParticleSystem<T_real,T_vertex>::addEigenVectorsAndCoMtoVertexBuffer( Eigen::Vector3d CoM, Eigen::Matrix3d cov, const T_real colorMult, const T_real length)
{
    //Vertex 1 of the first eigenvector
    m_vertexBufferData.push_back( CoM[0] );
    m_vertexBufferData.push_back( CoM[1] );
    m_vertexBufferData.push_back( CoM[2] );

    m_vertexBufferData.push_back(1*colorMult);
    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(0);

    //Vertex 2 of the first eigenvector
    m_vertexBufferData.push_back( cov.col(0)[0] * length + CoM[0] );
    m_vertexBufferData.push_back( cov.col(0)[1] * length + CoM[1] );
    m_vertexBufferData.push_back( cov.col(0)[2] * length + CoM[2] );

    m_vertexBufferData.push_back(1*colorMult);
    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(0);

    //Vertex 1 of the SECOND eigenvector
    m_vertexBufferData.push_back( CoM[0] );
    m_vertexBufferData.push_back( CoM[1] );
    m_vertexBufferData.push_back( CoM[2] );

    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(1*colorMult);
    m_vertexBufferData.push_back(0);

    //Vertex 2 of the SECOND eigenvector
    m_vertexBufferData.push_back( cov.col(1)[0] * length + CoM[0] );
    m_vertexBufferData.push_back( cov.col(1)[1] * length + CoM[1] );
    m_vertexBufferData.push_back( cov.col(1)[2] * length + CoM[2] );

    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(1*colorMult);
    m_vertexBufferData.push_back(0);

    //Vertex 1 of the THIRD eigenvector
    m_vertexBufferData.push_back( CoM[0] );
    m_vertexBufferData.push_back( CoM[1] );
    m_vertexBufferData.push_back( CoM[2] );

    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(1*colorMult);

    //Vertex 2 of the THIRD eigenvector
    m_vertexBufferData.push_back( cov.col(2)[0] * length + CoM[0] );
    m_vertexBufferData.push_back( cov.col(2)[1] * length + CoM[1] );
    m_vertexBufferData.push_back( cov.col(2)[2] * length + CoM[2] );

    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(0);
    m_vertexBufferData.push_back(1*colorMult);

};


template<class T_real, class T_vertex>
bool CGLParticleSystem<T_real,T_vertex>::draw(Shader *shader, Shader *constraintShader)
{
    //Update transformation matrix
    m_transform.m_data.computeMatrix();

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
    glEnable(GL_POINT_SPRITE_ARB);

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

    glDepthMask(GL_TRUE);

    shader->Use();
    GLuint SphereRadiusLoc = glGetUniformLocation(shader->Program, "SphereRadius");
    GLuint transformLoc = glGetUniformLocation(shader->Program, "model");
    glUniformMatrix4fv(transformLoc, 1,GL_FALSE, m_transform.m_data.m_pMatrix);
    glUniform1f(SphereRadiusLoc, m_pPSystem->m_particles[0]->m_size/2.0);

    constraintShader->Use();
    GLuint pointSize = glGetUniformLocation(constraintShader->Program, "pointSize");
    transformLoc = glGetUniformLocation(constraintShader->Program, "model");
    glUniformMatrix4fv(transformLoc, 1,GL_FALSE, m_transform.m_data.m_pMatrix);
    glUniform1ui(pointSize,m_pointSize);

    shader->Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, m_vertexBufferDataPoints.size() / 6);
    glBindVertexArray(0);

    glDisable(GL_POINT_SPRITE_ARB);


    if (m_drawConstraints)
    {
        constraintShader->Use();
        glBindVertexArray(VAOConstraints);
        glDrawArrays(GL_LINES, 0, m_vertexBufferData.size() / 6);
        glBindVertexArray(0);
    }

    return true;
}

#endif
