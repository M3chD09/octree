#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in float aSize;

out VS_OUT
{
    float size;
}
vs_out;

void main()
{
    vs_out.size = aSize;
    gl_Position = vec4(aPos, 1.0);
}
