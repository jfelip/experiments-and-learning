#include <GLGeometry.h>
#include <math.h>
#include <memory>
#include <vector>

//TODO: ReferenceFrame
//TODO: Arrow
//TODO: 2DText
//TODO: Camera
//TODO: Light
//TODO: PointCloud
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
	CSolidSphere(T_real radius, const CTransform<T_real>& t, T_integer alpha_resolution=20, T_integer beta_resolution=20)
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
    CSolidBox(T_real d_x,T_real d_y,T_real d_z, const CTransform<T_real>& t)
    {
    	m_dim[0] = d_x;
    	m_dim[1] = d_y;
    	m_dim[2] = d_z;
    	generateVertexData();
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
    CSolidCylinder(T_real radius, T_real height, const CTransform<T_real>& t, T_integer resolution=20)
    {
    	m_radius = radius;
    	m_height = height;
    	m_resolution = resolution;
    	setTransform(t);
    	generateVertexData();
    }

    void setRadius(T_real r){m_radius = r;}
    void setResolution(T_integer resolution){m_resolution = resolution;}
    void setTransform( const CTransform<T_real>& transform ) { m_geom.setTransform(transform);}

    T_real getRadius(){return m_radius;}
    T_integer getResolution(){return m_resolution;}
    CTransform<T_real>& getTransform() {return m_geom.getTransform();}


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
	CSolidCapsule(T_real radius, T_real height, const CTransform<T_real>& t, T_integer resolution=20)
    {
    	m_radius = radius;
    	m_height = height;
    	m_resolution = resolution;
    	setTransform(t);
    	generateVertexData();
    }

    void setRadius(T_real r){m_radius = r;}
    void setResolution(T_integer resolution){m_resolution = resolution;}
    void setTransform( const CTransform<T_real>& transform ) { m_geom.setTransform(transform);}

    T_real getRadius(){return m_radius;}
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
	CSolidCone(T_real radius, T_real height, const CTransform<T_real>& t, T_integer resolution=20)
    {
    	m_radius = radius;
    	m_height = height;
    	m_resolution = resolution;
    	setTransform(t);
    	generateVertexData();
    }

    void setRadius(T_real r){m_radius = r;}
    void setResolution(T_integer resolution){m_resolution = resolution;}
    void setTransform( const CTransform<T_real>& transform ) { m_geom.setTransform(transform);}

    T_real getRadius(){return m_radius;}
    T_integer getResolution(){return m_resolution;}
    CTransform<T_real>& getTransform() {return m_geom.getTransform();}

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
    	// DRAW THE SIDES
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



