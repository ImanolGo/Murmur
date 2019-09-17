/*ARB_vertex_shader*/

uniform float simpleMethod=0;

void main()
{
    vec4 dummy, vpos;

    vpos = gl_ModelViewMatrix * gl_Vertex;

    gl_Position = vpos;
    vec4 pos = gl_ProjectionMatrix * vpos;
    pos/=pos.w;
    gl_Position = pos+vec4(0.0,0.0,0,0);
    //gl_Position = gl_ProjectionMatrix * (vpos*vec4(1,1,0,1));
    gl_Position = gl_ProjectionMatrix * vpos;
    //gl_Position = gl_Vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    
    gl_FrontColor = gl_Color;

    if(simpleMethod)
        gl_Position=gl_Vertex;
}


/*ARB_fragment_shader*/

//uniform sampler2D s_tex_0;
//uniform sampler2D s_tex_1;

uniform float alpha=1.0;

void main()
{
    gl_FragColor = gl_Color;
    gl_FragColor.w*=alpha;
    //gl_FragColor.xyz = vec3(1,1,1);
    //gl_FragColor = vec4(1,1,1,1);
}
