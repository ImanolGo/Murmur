/*ARB_vertex_shader*/

#version 430 compatibility

uniform vec3 pos;
uniform vec3 x;

void main()
{
    vec4 dummy, vpos;

    vpos = gl_ModelViewMatrix * (gl_Vertex.xzyw*0.9*vec4(16,1,10,1)+vec4(0,20,0,0));
    //vpos = gl_ModelViewMatrix * (vec4(vec3(gl_TexCoord[0].xy*2.0-vec2(1,1),0)*10,1));

    gl_Position = gl_ProjectionMatrix * vpos;
    //vpos.z=0.99;
    //gl_Position=vec4(gl_TexCoord[0].xy*2.0-vec2(1,1),0,1);

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;

uniform float alpha=1.0;

void main()
{
    gl_FragColor = texture2D( s_tex_0, fract(gl_TexCoord[0].xy*vec2(1,-1)) );
    gl_FragColor.xyz=pow(gl_FragColor.xyz,vec3(1.5))*alpha;

    //gl_FragColor.w=(1.0-pow((gl_FragColor.x),0.4))*alpha;

    //gl_FragColor.w=(1.0-(gl_FragColor.x)*2.3)*alpha;
    //gl_FragColor.w=(1.0-gl_FragColor.x)*alpha;

    gl_FragColor.w=(1.0-pow((gl_FragColor.x),0.2))*alpha;
    gl_FragColor.xyz=vec3(0);

    //gl_FragColor.w=sqrt(gl_FragColor.w);
    //if(gl_FragColor.x>0.5) discard;
    //if(gl_FragColor.w<0.1) discard;
//    gl_FragColor.w = 1.0;
//    gl_FragColor.xy = fract(gl_TexCoord[0].xy);
//    gl_FragColor = vec4(0.5,0.5,0.5,0.5);
//    gl_FragColor.zw = 0;
//    gl_FragColor = texture2D( s_tex_0, gl_TexCoord[0].xy ).z;
//    gl_FragColor = vec4(1.0);
}
