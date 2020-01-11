/*ARB_vertex_shader*/

#version 430 compatibility

uniform vec3 pos;
uniform vec3 x;

void main()
{
    vec4 dummy, vpos;

    vpos = gl_ModelViewMatrix * ( gl_Vertex * vec4(5,5,0,1) - vec4(0,30,1.5,0) );
    //vpos = gl_ModelViewMatrix * gl_Vertex ;

    gl_Position = gl_ProjectionMatrix * vpos;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;

uniform float fader = 1;
uniform float maskSize = 1.0;
//uniform float alpha = 0;

float diffVal(vec3 texc)
{
    return
        length(texture2D(s_tex_0, texc.xy)-texture2D(s_tex_0, (texc+vec3(0.01,0,0)).xy)) +
        length(texture2D(s_tex_0, texc.xy)-texture2D(s_tex_0, (texc+vec3(0,0.01,0)).xy));
}

void main()
{
    vec3 correct;
    //discard;
/*    float fact;
    vec3 texc;
    fact=0.15+0.5*(1.0+sin(gl_TexCoord[0].z*3.0*3.1428))*0.7;
    texc = gl_TexCoord[0].xyz;
    texc.z = sin(gl_TexCoord[0].x*10.0)*sin(gl_TexCoord[0].y*10.0);
    texc.z = fract(gl_TexCoord[0].z+0.1+0.1*texc.z*texc.z);
    texc.xyz = gl_TexCoord[0].zyx;
    texc.z=gl_TexCoord[0].z+gl_TexCoord[0].x;
    texc.x=gl_TexCoord[0].x;
    texc.xyz = gl_TexCoord[0].xyz;*/
/*    correct.xy=gl_TexCoord[0].xy-vec2(0.4);
    correct.xy*=vec2(1.024,0.768);
    correct.xy*=length(correct.xy);
    correct.xy*=-0.36;
    correct.xy/=vec2(1.024,0.768);
    correct.z=0.0;*/
    //gl_FragColor = texture3D(s_tex_0, fract(gl_TexCoord[0].xyz*vec3(1,1,1))/*+correct*/);

    // plain version for plain floor scene
    vec4 col1=pow(texture2D(s_tex_0, fract(gl_TexCoord[0].xyz*vec3(1,1,1)).xy).x*vec4(0.0,0.5,1.0,1)*4.1,vec4(3));
    col1=clamp(col1,vec4(0),vec4(1));
    col1.w=1;
    //gl_FragColor = texture3D(s_tex_0, fract(gl_TexCoord[0].xyz*vec3(1,1,1))).x*vec4(0.0,0.5,1,1);

    // differential version for water scene
    vec4 col2=8.0*diffVal(fract(gl_TexCoord[0].xyz*vec3(1,1,1)))*vec4(0.0,0.5,1,1)-vec4(0.0,0.0,0.0,0);
    col2=clamp(col2,vec4(0),vec4(1));
    col2.w=1;

    //col1 = vec4(1,0,0,1);
    //col2 = vec4(0,1,0,1);
    vec4 col3 = vec4(0,0,0,1);


    gl_FragColor = vec4(0,0,0,1);

    //float fader=1.1;

    vec2 v = 3.0*(gl_TexCoord[0].xy-vec2(0.5,0.46))/maskSize;
    float mask = clamp(1.0-dot(v,v),0.0,1.0);
    //mask=1.0;
    float fader03 = fract(fader/3.0)*3.0;

    /*if(fader03<3.0)
        gl_FragColor = mask*mix(col3,mix(col1,col2,0.0),fract(fader03));
    if(fader03<2.0)
        gl_FragColor = mask*mix(col2,col3,fract(fader03));
    if(fader03<1.0)
        gl_FragColor = mask*mix(mix(col1,col2,0.0),col2,fract(fader03));*/

    if(fader03<3.0)
        gl_FragColor = mask*mix(col2,col3,fract(fader03));
    if(fader03<2.0)
        gl_FragColor = mask*mix(mix(col1,col2,0.0),col2,fract(fader03));
    if(fader03<1.0)
        gl_FragColor = mask*mix(col3,mix(col1,col2,0.0),fract(fader03));

    //gl_FragColor.w=1;
    //gl_FragColor=vec4(1);

    /*
     gl_FragColor = mix(col1,col2,fader);

    gl_FragColor.xy += 0.2*gl_TexCoord[0].xy;
    if(fract(gl_TexCoord[0].x*10)<0.2) discard;
    if(fract(gl_TexCoord[0].y*10)<0.2) discard;

    if(fader>1.5) gl_FragColor = vec4(0,0,0,1);
    */

//    gl_FragColor=vec4(gl_FragColor.z);
//    gl_FragColor.z=0;
}
