#version 430 core
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in VS_OUT
{
    float size;
}
gs_in[];

out GS_OUT
{
    vec3 fragPos;
    vec3 normal;
}
gs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 mv = projection * view;
    vec4 vertices[8] = {
        vec4(-1.0, -1.0, -1.0, 0.0),
        vec4(1.0, -1.0, -1.0, 0.0),
        vec4(-1.0, 1.0, -1.0, 0.0),
        vec4(1.0, 1.0, -1.0, 0.0),
        vec4(-1.0, -1.0, 1.0, 0.0),
        vec4(1.0, -1.0, 1.0, 0.0),
        vec4(-1.0, 1.0, 1.0, 0.0),
        vec4(1.0, 1.0, 1.0, 0.0),
    };

    for (int i = 0; i < 8; i++) {
        vertices[i] = gl_in[0].gl_Position + 0.5 * gs_in[0].size * vertices[i];
    }

    int indices[6][4] = {
        { 0, 1, 2, 3 },
        { 0, 1, 4, 5 },
        { 0, 2, 4, 6 },
        { 1, 3, 5, 7 },
        { 2, 3, 6, 7 },
        { 4, 5, 6, 7 },
    };

    vec3 normals[6] = {
        vec3(0.0, 0.0, -1.0),
        vec3(0.0, -1.0, 0.0),
        vec3(-1.0, 0.0, 0.0),
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0),
    };

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            gs_out.fragPos = vec3(model * vertices[indices[i][j]]);
            gs_out.normal = normals[i];
            gl_Position = mv * vec4(gs_out.fragPos, 1.0);
            EmitVertex();
        }
        EndPrimitive();
    }
}
