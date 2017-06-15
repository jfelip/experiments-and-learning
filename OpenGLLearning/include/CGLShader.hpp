#ifndef SHADER_H
#define SHADER_H

//
// Shader abstraction Class for OpenGL adapted from https://learnopengl.com/#!Getting-started/Shaders
//


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <Common.h>

class Shader
{
public:
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string geometryCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream gShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::badbit);

        if (vertexPath != "")
        {
			try
			{
				vShaderFile.open(vertexPath);
				std::stringstream vShaderStream;
				vShaderStream << vShaderFile.rdbuf();
				vShaderFile.close();
				vertexCode = vShaderStream.str();
			}
			catch (std::ifstream::failure& e)
			{
				_GENERIC_ERROR_("ERROR::VERTEX::SHADER::FILE_NOT_SUCCESFULLY_READ");
			}
    	}

        if (geometryPath != "")
        {
			try
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
			catch (std::ifstream::failure& e)
			{
				_GENERIC_ERROR_("ERROR::GEOMETRY::SHADER::FILE_NOT_SUCCESFULLY_READ");
			}
    	}
        if (fragmentPath != "")
        {
			try
			{
				fShaderFile.open(fragmentPath);
				std::stringstream fShaderStream;
				fShaderStream << fShaderFile.rdbuf();
				fShaderFile.close();
				fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure& e)
			{
				_GENERIC_ERROR_("ERROR::FRAGMENT::SHADER::FILE_NOT_SUCCESFULLY_READ");
			}
    	}

        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar * gShaderCode = geometryCode.c_str();
        const GLchar * fShaderCode = fragmentCode.c_str();
        // 2. Compile shaders
        GLuint vertex, geometry, fragment;
        GLint success;
        GLchar infoLog[512];

        if (vertexCode != "")
        {
            // Vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            // Print compile errors if any
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                _GENERIC_ERROR_("ERROR::SHADER::VERTEX::COMPILATION_FAILED: " + std::string(infoLog));
            }
        }

        if (geometryCode != "")
        {
            // Geometry Shader
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            // Print compile errors if any
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(geometry, 512, NULL, infoLog);
                _GENERIC_ERROR_("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED: " + std::string(infoLog));
            }
        }

        if (fragmentCode != "")
        {
            // Fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            // Print compile errors if any
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                _GENERIC_ERROR_("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " + std::string(infoLog));
            }
        }

        // Shader Program
        this->Program = glCreateProgram();
        if (vertexCode != "")
        {
        	glAttachShader(this->Program, vertex);
        }
		if (geometryCode != "")
		{
			glAttachShader(this->Program, geometry);
		}
		if (fragmentCode != "")
		{
			glAttachShader(this->Program, fragment);
		}
        glLinkProgram(this->Program);
        // Print linking errors if any
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            _GENERIC_ERROR_("ERROR::SHADER::PROGRAM::LINKING_FAILED: " + std::string(infoLog));
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        if (vertexCode != "")
        {
        	glDeleteShader(vertex);
        }
        if (geometryCode != "")
        {
        	glDeleteShader(geometry);
        }
        if (fragmentCode != "")
        {
        	glDeleteShader(fragment);
        }

    }
    // Uses the current shader
    void Use()
    {
        glUseProgram(this->Program);
    }
};

#endif
