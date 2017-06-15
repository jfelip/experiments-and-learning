#ifndef GL_VIEWER_PRIMITIVES_H
#define GL_VIEWER_PRIMITIVES_H

#include <GLGeometry.h>
#include <Material.hpp>
#include <math.h>
#include <memory>
#include <vector>

//TODO: 2DText
//TODO: Light
//TODO: ParticleSystem
//TODO: SolidMesh


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


template<typename T_real=GLfloat, typename T_integer=GLuint>
class CSolidCone
{

	typedef std::shared_ptr<CSolidCone> Ptr;

	typedef std::shared_ptr<CSolidCone> ConstPtr;

protected:
	COpenGLGeometry<T_real,T_real,T_integer> m_geom;
	T_real m_radius;
	T_real m_height;
	T_integer m_resolution;

public:
	CSolidCone(T_real radius=1.0, T_real height=1.0, CTransform<T_real> t=CTransform<T_real>(), T_integer resolution=20)
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

    void translate (const T_real t[3])	{m_geom.translate(t);}

    void generateVertexData()
    {
    	std::vector<T_real> vertices;
    	std::vector<T_integer> indices;
    	std::vector<T_real> normals;
    	std::vector<T_real> colors;
    	std::vector<T_real> textureCoords;

    	T_real x, y, z, alpha; // Storage for coordinates and angles
    	T_real incr = M_PI/m_resolution;
    	T_real M_PI2 = 2*M_PI + incr;
    	T_integer idx = 0;

    	////////////////////////////
    	// DRAW THE CAP
    	////////////////////////////
    	for (alpha = 0.0; alpha < M_PI2; alpha += incr)
    	{
    	        x = m_radius*sin(alpha);
    	        y = m_radius*cos(alpha);
    	        z = 0;
    	        vertices.push_back(x);
    	        vertices.push_back(y);
    	        vertices.push_back(z);
    	        indices.push_back(idx); idx++;
    	        normals.push_back(x);
    	        normals.push_back(y);
    	        normals.push_back(z);

    	        vertices.push_back(0);
    	        vertices.push_back(0);
    	        vertices.push_back(0);
    	        indices.push_back(idx); idx++;
    	        normals.push_back(0);
    	        normals.push_back(0);
    	        normals.push_back(-1);
    	}

    	////////////////////////////
    	// DRAW THE CONE
    	////////////////////////////
    	for (alpha = 0.0; alpha < M_PI2; alpha += incr)
    	{
    	        x = m_radius*sin(alpha);
    	        y = m_radius*cos(alpha);
    	        z = 0;
    	        vertices.push_back(x);
    	        vertices.push_back(y);
    	        vertices.push_back(z);
    	        indices.push_back(idx); idx++;
    	        normals.push_back(x);
    	        normals.push_back(y);
    	        normals.push_back(z);

    	        vertices.push_back(0);
    	        vertices.push_back(0);
    	        vertices.push_back(m_height);
    	        indices.push_back(idx); idx++;
    	        normals.push_back(0);
    	        normals.push_back(0);
    	        normals.push_back(1);
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


template<typename T_real=GLfloat, typename T_integer=GLuint>
class CSolidArrow
{

	typedef std::shared_ptr<CSolidArrow> Ptr;

	typedef std::shared_ptr<CSolidArrow> ConstPtr;

protected:
	CSolidCone<T_real,T_integer> m_head;
	CSolidCylinder<T_real,T_integer> m_body;

public:
	CSolidArrow(T_real head_radius=0.2, T_real head_height=0.2,T_real body_radius=0.1, T_real body_height=0.8, CTransform<T_real> t=CTransform<T_real>(), T_integer resolution=20)
    {
    	m_body.setRadius(body_radius);
    	m_body.setHeight(body_height);
    	m_body.setResolution(resolution);

    	m_head.setRadius(head_radius);
    	m_head.setHeight(head_height);
    	m_head.setResolution(resolution);

    	setTransform(t);
    	generateVertexData();
    }

    void setBodyRadius(const T_real& r){m_body.setRadius(r);}

    void setHeadRadius(const T_real& r){m_head.setRadius(r);}

    void setBodyHeight(const T_real& h){m_body.setHeight(h);}

    void setHeadHeight(const T_real& h){m_head.setHeight(h);}

    void setResolution(const T_integer& resolution)
    {
    	m_body.setResolution(resolution);
    	m_head.setResolution(resolution);
    }

    void setTransform( const CTransform<T_real>& transform )
    {
    	CTransform<T_real> t1;
    	t1.translateZ(m_body.getHeight()*0.5);

    	CTransform<T_real> t2;
    	t2.translateZ(m_body.getHeight());

    	m_body.setTransform(transform*t1);
    	m_head.setTransform(transform*t2);
    }

    T_real getHeadRadius(){return m_head.getRadius();}

    T_real getBodyRadius(){return m_body.getRadius();}

    T_real getHeadHeight(){return m_head.getHeight();}

    T_real getBodyHeight(){return m_body.getHeight();}

    T_integer getResolution(){return m_body.getResolution();}

    CTransform<T_real>& getTransform(){	return m_body.getTransform(); }

    void generateVertexData()
    {
    	m_head.generateVertexData();
    	m_body.generateVertexData();
    }

    void draw(Shader *s)
    {
    	m_head.draw(s);
    	m_body.draw(s);
    }
};

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

//TODO: Change the cone shape to implement the aspect ratio of the camera
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


#endif
