#version 120

varying vec3 normal;
varying vec4 vpos;

void main()
{
    vpos = gl_Vertex;
    gl_Position.xyz = gl_Vertex.xyz;
    normal = gl_Normal;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
