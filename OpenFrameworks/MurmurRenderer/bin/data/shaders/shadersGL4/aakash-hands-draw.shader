/*ARB_vertex_shader*/

#version 430 compatibility

uniform vec3 pos;
uniform vec3 x;
uniform float pos2plane = 0.0;
uniform vec3 planeWHD   = vec3(10,5,0);

uniform  float simpleMethod = 0;
#define W planeWHD.x
#define H planeWHD.y
#define D planeWHD.z

void main()
{
    vec4 dummy, vpos, vpos2;

    vpos = gl_ModelViewMatrix * gl_Vertex;

    vpos2 = (vec4(gl_MultiTexCoord0.x,0,gl_MultiTexCoord0.y,0.5)-vec4(0.5,0.5,0.5,0))*2.0;
    vpos2 *= vec4(W/2.0,1,H/2.0,1);
    vpos2 += vec4(0,D,0,0);
    vpos2 = gl_ModelViewMatrix * vpos2;

    vpos = mix(vpos,vpos2,pos2plane);

    gl_Position = gl_ProjectionMatrix * vpos;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[1] = vpos;
    gl_TexCoord[2].xyz = gl_NormalMatrix * vec3(1,0,0);
    gl_TexCoord[3].xyz = gl_NormalMatrix * vec3(0,0,1);
    gl_TexCoord[4].xyz = cross(gl_TexCoord[2].xyz,gl_TexCoord[3].xyz);

    if(simpleMethod>0.99)
        gl_Position=vec4(gl_TexCoord[0].xy*2.0-vec2(1,1),0,1);
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;
uniform float alpha = 0.0;
uniform float stereo = 0.0;
uniform float fader = 1.0;
uniform float whiter = 0.0;
uniform float modeIdx = 0;

#define vpos (gl_TexCoord[1].xyz)
#define tanx (gl_TexCoord[2].xyz)
#define tany (gl_TexCoord[3].xyz)
#define tanz (gl_TexCoord[4].xyz)

float maskFunc0(float x)
{
    if(x>0) return clamp(-x+1,0,1);
    return clamp(x+1,0,1);
}

float maskFuncDelta(float index, float maskIndex, float fadeDelta)
{
    return clamp(
                 maskFunc0(
                           (index-(maskIndex+0.5-fadeDelta*0.5))
                           /(0.5+fadeDelta*0.5)
                          ) / fadeDelta,
                 0,1
                );
}

float maskFuncDeltaRep(float index, float maskIndex, float fadeDelta, float maxIndex)
{
    return max(maskFuncDelta(index,maskIndex,fadeDelta),maskFuncDelta(index-maxIndex,maskIndex,fadeDelta));
}

void main()
{
    vec4 col = texture2D( s_tex_0, gl_TexCoord[0].xy );
    float len = length(col.xyz);
    gl_FragColor = mix(0.5*vec4(pow(length(vec3(col.xyz-vec3(0,0,0.5))*1.0f),1.0)*1.5),vec4(1),whiter)*fader;
    float myModeIdx=1;
    float modeFader = maskFuncDeltaRep(modeIdx,myModeIdx,0.5,3);
    gl_FragColor.w = mix(1.1-length(gl_FragColor.xyz),1,whiter) *3 * fader * modeFader;
    //gl_FragColor.w = 1;

    //gl_FragColor.w = 1.5*length(col.xy)*length(col.xy)*alpha;
    //gl_FragColor.xyzw=0.7*(gl_FragColor.xyzw-vec3(0.5))+vec3(0.5);
//    gl_FragColor.w = 2.0*length(col.xy-0.5)*alpha;
//    gl_FragColor.w = length(texture3D( s_tex_0, gl_TexCoord[0].xyz ).xy-vec2(0.5))*2.0*alpha;

//    gl_FragColor.z = stereo*col.z;

//    gl_FragColor.z = 0.0;

//    gl_FragColor.w = 1.0;
//    discard;
//    gl_FragColor.w = 1.0;
//    gl_FragColor.xy = fract(gl_TexCoord[0].xy);
//    gl_FragColor = vec4(0.5,0.5,0.5,0.5);
//    gl_FragColor.zw = 0;
//    gl_FragColor = texture2D( s_tex_0, gl_TexCoord[0].xy ).z;
//    gl_FragColor = vec4(1.0);
}
