#ifndef __CMATERIAL_H_
#define __CMATERIAL_H_


#include<CGLShader.hpp>

template <class T_Real=float>
class CMaterial
{
public:
	T_Real diffuse[3] = {T_Real(0),T_Real(0),T_Real(0)};
	T_Real specular[3]= {T_Real(0),T_Real(0),T_Real(0)};
	T_Real shininess  = T_Real(0);

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

	void use(Shader *s)
	{
    	GLint matDiffuseLoc  = glGetUniformLocation(s->Program, "material.diffuse");
    	GLint matSpecularLoc = glGetUniformLocation(s->Program, "material.specular");
    	GLint matShineLoc    = glGetUniformLocation(s->Program, "material.shininess");
    	glUniform3f(matDiffuseLoc,  diffuse[0], diffuse[1], diffuse[2]);
    	glUniform3f(matSpecularLoc, specular[0], specular[1], specular[2]);
    	glUniform1f(matShineLoc,    shininess);
	}
};

#endif
