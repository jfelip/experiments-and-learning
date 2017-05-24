#version 330
layout(triangles) in;

// Three lines will be generated: 6 vertices
layout(line_strip, max_vertices=6) out;

in Vertex
{
  vec4 normal;
  vec4 color;
  vec2 texCoord;
  vec3 fragPos;
} vertex[];

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform float normal_length;

out vec4 ourColor; //Vertex color
//out vec3 FragPos;
//out vec3 Normal;
//out vec2 TexCoords;

void main()
{

}
