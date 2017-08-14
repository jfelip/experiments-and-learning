
//Shader almost all obtained from https://github.com/tdd11235813/spheres_shader

#version 330 core
layout (location = 0) in vec3 SpherePosition;
layout (location = 1) in vec3 SphereColor;

out VS_OUT
{
    vec4 eye_position;
    vec3 sphere_color;
    float sphere_radius;
    vec3 lightDir;
} vs_out;


uniform float SphereRadius;
uniform vec3 lightPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform int screenWidth;

void main()
{
      vs_out.eye_position = view * model * vec4(SpherePosition.xyz,1.0f);
      vs_out.sphere_color = SphereColor.xyz;
      vs_out.sphere_radius = SphereRadius;

      vs_out.lightDir = normalize( lightPos.xyz );
      float dist = length(vs_out.eye_position.xyz);

      gl_Position = vs_out.eye_position;
      gl_Position = projection * gl_Position;
    // http://stackoverflow.com/questions/8608844/resizing-point-sprites-based-on-distance-from-the-camera
      vec4 projCorner = projection * vec4(vs_out.sphere_radius, vs_out.sphere_radius, vs_out.eye_position.z, vs_out.eye_position.w);
      gl_PointSize = screenWidth * projCorner.x / projCorner.w;
}