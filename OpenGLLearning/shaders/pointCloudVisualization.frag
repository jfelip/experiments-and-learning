#version 330 core

uniform mat4 projection;


in VS_OUT
{
    vec4 eye_position;
    vec3 sphere_color;
    float sphere_radius;
    vec3 lightDir;
} vs_out;

out vec4 out_Color;

void main()
{
    // r^2 = (x - x0)^2 + (y - y0)^2 + (z - z0)^2
    vec2 texcoord = gl_PointCoord* 2.0 - vec2(1.0);
    float x = texcoord.x;
    float y = texcoord.y;
    float zz = 1.0 - x*x - y*y;

    if (zz <= 0.0)
      discard;

    float z = sqrt(zz);
    vec4 pos = vs_out.eye_position;
    pos.z += vs_out.sphere_radius*z;
    pos = projection * pos;
    gl_FragDepth = 0.5*(pos.z / pos.w)+0.5;

    vec3 normal = vec3(x,y,z);
    float diffuseTerm = clamp(dot(normal, vs_out.lightDir), 0.0, 1.0);

    out_Color = vec4(vec3(0.15,0.15,0.15) +  diffuseTerm * vs_out.sphere_color, 1.0);

    //out_Color = vec4(vs_out.sphere_color,1.0);
}