#version 120

#extension GL_ARB_texture_rectangle : enable

varying vec3 normal2;

void main(void)
{
    gl_FragColor = vec4(mix(vec3(length(normal2)),-normal2,0.7),1);
}

