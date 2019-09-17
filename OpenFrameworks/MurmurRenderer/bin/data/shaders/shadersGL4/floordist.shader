/*ARB_vertex_shader*/

void main()
{
    vec4 dummy, vpos;

    vpos = gl_ModelViewMatrix * gl_Vertex;

    //gl_Position = gl_ProjectionMatrix * ((vpos + vec4(0.0,2.8,0,0))*vec4(2.0,3.5,1,1));
    //gl_Position = gl_ProjectionMatrix * ((vpos + vec4(+0.2,+4.3,0,0))*vec4(-1.0,1.1,1,1));
    //gl_Position = gl_ProjectionMatrix * ((vpos + vec4(-0.42,+6.42,0,0))*vec4( 0.4,-0.4,1,1));
    gl_Position = gl_ProjectionMatrix * ((vpos + vec4(-0.0,+0.0,0,0))*vec4( 0.4,-0.4,1,1));
    //gl_Position = gl_ProjectionMatrix * (vpos);

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    
    gl_FrontColor = gl_Color;
}


/*ARB_fragment_shader*/

//uniform sampler2D s_tex_0;
//uniform sampler2D s_tex_1;

uniform float alpha=0.0;

void main()
{
    gl_FragColor = gl_Color;
    gl_FragColor = vec4(1,1,0,1);
    gl_FragColor.w*=alpha;
    //gl_FragColor.w=1;
//    gl_FragColor.xyz = vec3(1,1,1);
//    gl_FragColor = vec4(1,0,0,1);
}
