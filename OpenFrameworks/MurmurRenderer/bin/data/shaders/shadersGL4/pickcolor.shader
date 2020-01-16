/*ARB_vertex_shader*/

#version 430 compatibility

void main()
{
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = ftransform();
    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
}


/*ARB_fragment_shader*/

//uniform sampler2D s_tex_0;

#version 430 compatibility

void main()
{
    gl_FragColor = gl_Color;
}
