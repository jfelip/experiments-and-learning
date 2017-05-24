#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out VS_OUT
{
	vec3 ourColor;
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos; 	
} vs_out;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);	
	vs_out.ourColor = color;
	// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	// TexCoord = texCoord;
	vs_out.TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.Normal = normal;
}