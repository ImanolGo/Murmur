/*ARB_vertex_shader*/

#version 430 compatibility

varying vec3 normal;
varying vec4 vpos;

void main()
{
    //vec4 dummy, vpos;

    //vpos = gl_ModelViewMatrix * gl_Vertex;

    //gl_Position = gl_ProjectionMatrix * vpos;
    //gl_Position.xyz = 0.2*gl_Vertex.xyz;

    vpos = gl_Vertex;
    gl_Position = gl_ModelViewMatrix * (gl_Vertex * vec4(vec3(5.0),1.0));
    normal = gl_Normal;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;

varying vec3 normal;

void main()
{
    //gl_FragColor = texture2D( s_tex_0, fract(gl_TexCoord[0].xy) );
    gl_FragColor = vec4(normal*10,1);
    gl_FragColor = vec4(1,1,1,1);
    //gl_FragColor.xyz = (length(gl_TexCoord[0].xy-vec2(0.5))<0.25) ? (1.0) : vec3(0.0) ;
//    discard;
//    gl_FragColor.xy = fract(gl_TexCoord[0].xy);
//    gl_FragColor = vec4(0.5,0.5,0.5,0.5);
//    gl_FragColor.zw = 0;
//    gl_FragColor = texture2D( s_tex_0, gl_TexCoord[0].xy ).z;
//    gl_FragColor = vec4(1.0);
}
