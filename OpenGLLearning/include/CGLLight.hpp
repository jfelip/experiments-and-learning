#ifndef __CGLLIGHT_H_
#define __CGLLIGHT_H_

#include <Transform.h>

template <class T_Real=float>
class CAttenuation
{
public:
	CAttenuation():constant(1.0),linear(0.22),quadratic(0.2){}
	CAttenuation(T_Real c, T_Real l, T_Real q):constant(c),linear(l),quadratic(q){}
	//Attenuation parameter values for the light attenuation (spot and point lights)
	T_Real constant = T_Real(0);
	T_Real linear = T_Real(0);
	T_Real quadratic = T_Real(0);

	/*
	* Recommended attenuation values depending on the distance we want the light to have effect
	* Distance	Constant	Linear	Quadratic
	7			1.0			0.7		1.8
	13			1.0			0.35	0.44
	20			1.0			0.22	0.20
	32			1.0			0.14	0.07
	50			1.0			0.09	0.032
	65			1.0			0.07	0.017
	100			1.0			0.045	0.0075
	160			1.0			0.027	0.0028
	200			1.0			0.022	0.0019
	325			1.0			0.014	0.0007
	600			1.0			0.007	0.0002
	3250		1.0			0.0014	0.000007
	*
	* Table extracted from https://learnopengl.com/#!Lighting/Light-casters
	*/

};


template <class T_Real=float>
class CDirectionalLight
{
public:
	T_Real direction[3] = {T_Real(0),T_Real(0),T_Real(0)}; ///> Position and orientation of the light (for directional light only orientation is used as the direction vector)
	T_Real ambient[3]   = {T_Real(0),T_Real(0),T_Real(0)};
	T_Real diffuse[3]   = {T_Real(0),T_Real(0),T_Real(0)};
	T_Real specular[3]  = {T_Real(0),T_Real(0),T_Real(0)};

	void setDirection(const T_Real x, const T_Real y, const T_Real z)
	{
		direction[0] = x;
		direction[1] = y;
		direction[2] = z;
	}

	void setDiffuse(const T_Real r, const T_Real g, const T_Real b)
	{
		diffuse[0] = r;
		diffuse[1] = g;
		diffuse[2] = b;
	}

	void setSpecular(const T_Real r, const T_Real g, const T_Real b)
	{
		specular[0] = r;
		specular[1] = g;
		specular[2] = b;
	}

	void setAmbient(const T_Real r, const T_Real g, const T_Real b)
	{
		ambient[0] = r;
		ambient[1] = g;
		ambient[2] = b;
	}


	void use(Shader *s)
	{
		//TODO: ADD LIGHT TYPE and pass it to the shader
        // Pass the light information to the shader
        GLint lightPosLoc = glGetUniformLocation(s->Program, "dirLight.direction");
        GLint lightAmbientLoc  = glGetUniformLocation(s->Program, "dirLight.ambient");
        GLint lightDiffuseLoc  = glGetUniformLocation(s->Program, "dirLight.diffuse");
        GLint lightSpecularLoc = glGetUniformLocation(s->Program, "dirLight.specular");
        GLint lightEnableLoc = glGetUniformLocation(s->Program, "dirLightEnabled");

        glUniform1i(lightEnableLoc,true);
        glUniform3f(lightPosLoc, direction[0], direction[1], direction[2]);
        glUniform3f(lightAmbientLoc,  ambient[0], ambient[1], ambient[2]);
        glUniform3f(lightDiffuseLoc,  diffuse[0], diffuse[1], diffuse[2]);
        glUniform3f(lightSpecularLoc, specular[0], specular[1], specular[2]);
	}

};

template <class T_Real=float>
class CPointLight
{
public:

	T_Real position[3];
	T_Real direction[3]; ///> Position and orientation of the light (for directional light only orientation is used as the direction vector)

	T_Real color_diffuse;
	T_Real color_specular;

	CAttenuation<T_Real> attenuation;

	uint id;

	void use(Shader *s)
	{

	}

};


template <class T_Real=float>
class CSpotLight
{
public:

	T_Real position[3];
	T_Real direction[3]; ///> Position and orientation of the light (for directional light only orientation is used as the direction vector)

	T_Real color_diffuse;
	T_Real color_specular;

	CAttenuation<T_Real> attenuation;

	T_Real inner_cutoff;
	T_Real outer_cutoff;

	uint id;

	void use(Shader *s)
	{

	}

};


#endif
