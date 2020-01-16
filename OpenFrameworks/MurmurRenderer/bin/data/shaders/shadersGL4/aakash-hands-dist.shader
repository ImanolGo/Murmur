/*ARB_vertex_shader*/

#version 430 compatibility

varying vec3 normal;
varying vec4 vpos;
//uniform float shillScale = 1.0;
//uniform vec2  shillOffs

void main()
{
    //vec4 dummy, vpos;

    //vpos = gl_ModelViewMatrix * gl_Vertex;

    //gl_Position = gl_ProjectionMatrix * vpos;
    //gl_Position.xyz = 0.2*gl_Vertex.xyz;

    vpos = gl_Vertex;
    gl_Position.xyz = gl_Vertex.xyz;
    normal = gl_Normal;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}


/*EXT_geometry_shader*/
//InputPrimitive LINES
//OutputPrimitive TRIANGLE_STRIP
//VerticesOut 24

#version 430 compatibility

uniform float nshift=0.0;
uniform float thick=0.03;
in vec4 vpos[];
in vec3 normal[];
varying vec3 normal2;
varying vec3 normal0;
varying float alphaCoord;

void main(void)
{
    normal2=gl_NormalMatrix*normal[0];
    normal0=normal[0];
    alphaCoord=0;
    gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * (vpos[0] + (nshift-0.5*thick)*vec4(normal0,0)) );
    EmitVertex();
    alphaCoord=1;
    gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * (vpos[0] + (nshift+0.5*thick)*vec4(normal0,0)) );
    EmitVertex();

    normal2=gl_NormalMatrix*normal[1];
    normal0=normal[1];
    alphaCoord=0;
    gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * (vpos[1] + (nshift-0.5*thick)*vec4(normal0,0)) );
    EmitVertex();
    alphaCoord=1;
    gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * (vpos[1] + (nshift+0.5*thick)*vec4(normal0,0)) );
    EmitVertex();
    EndPrimitive();
}

/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;

varying vec3 normal0;
varying vec3 normal2;
varying float alphaCoord;

void main()
{
    //gl_FragColor = texture2D( s_tex_0, fract(gl_TexCoord[0].xy) );
    gl_FragColor = vec4(normal2,alphaCoord<0.5?alphaCoord*2:1.0-(alphaCoord-0.5)*2);
    //gl_FragColor = vec4(1,1,1,1);
    //gl_FragColor.xyz = (length(gl_TexCoord[0].xy-vec2(0.5))<0.25) ? (1.0) : vec3(0.0) ;
//    discard;
//    gl_FragColor.xy = fract(gl_TexCoord[0].xy);
//    gl_FragColor = vec4(0.5,0.5,0.5,0.5);
//    gl_FragColor.zw = 0;
//    gl_FragColor = texture2D( s_tex_0, gl_TexCoord[0].xy ).z;
//    gl_FragColor = vec4(1.0);
}
