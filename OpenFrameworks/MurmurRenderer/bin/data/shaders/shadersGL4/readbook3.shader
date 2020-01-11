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
uniform sampler2D s_tex_2;
uniform sampler2D s_tex_3;
uniform sampler2D s_tex_4;

uniform float alpha=1.0;
uniform float imageIdx=0.0;
uniform float time=0.0;
uniform float time2=0.0;

/*float maskFunc(float index, float maskIndex)
{
    if(index>maskIndex) return clamp((maskIndex-index)+1,0,1);
    return clamp((index-maskIndex)+1,0,1);
}

float maskFuncDelta(float index, float maskIndex, float fadeDelta)
{
    return clamp(maskFunc(index,maskIndex+0.5-fadeDelta*0.5)/fadeDelta,0,1);
}*/

float maskFunc0(float x)
{
    if(x>0) return clamp(-x+1,0,1);
    return clamp(x+1,0,1);
}

float maskFunc(float index, float maskIndex)
{
    return maskFunc0(index-maskIndex);
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

float gauss(float x)
{
    return exp(-x*x);
}

void main()
{
    float myImageIdx = 2;
    float myImageIdx2 = 3;
    //float myImageIdx3 = 4;
    float omega_t = 0.2*time;
    vec2 offs1 = -0.03*vec2(cos(omega_t),cos(omega_t*0.36254));
    vec2 offs2 = 0.03*vec2(sin(omega_t),sin(omega_t*0.36254));
    vec2 offs3 = 0.03*vec2(cos(omega_t),cos(omega_t*0.36254));
    vec2 offs4 = 0.0*vec2(cos(omega_t),cos(omega_t*0.36254));
    vec2 offs5 = 0.0*vec2(cos(omega_t),cos(omega_t*0.36254));

    //float gateRamp = clamp((imageIdx-myImageIdx-1.0)/1.0,0,1);
    float gateRamp = clamp(time2/14.0,0,1);
    if(imageIdx<=myImageIdx+0.05) gateRamp=0;
    if(imageIdx>myImageIdx2) gateRamp=1;

    float gateScale = 1.0-0.55*gateRamp;
    float offsGate=-0.5*gateRamp;

    float bgFact=0;
    vec4 imgRear1 = texture2D( s_tex_0, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs1*1*0.5), 1.5*(1.0-gateRamp) );
    vec4 imgRear2 = texture2D( s_tex_1, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs1*2*0.5), 2.5*(1.0-gateRamp) );
    vec4 imgRear3 = texture2D( s_tex_2, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs1*3*0.5), 4.5*(1.0-gateRamp) );
    vec4 imgGate  = texture2D( s_tex_3, clamp(((gl_TexCoord[0].xy*vec2(1,-1)+vec2(0,1)+offs4)-vec2(0.5))*gateScale+vec2(0.5)+vec2(0,0.15*gateRamp)+vec2(offsGate,0),vec2(0),vec2(1)), 4.5*gateRamp );
    vec4 imgWall  = texture2D( s_tex_4, clamp(((gl_TexCoord[0].xy*vec2(1,-1)+vec2(0,1)+offs5)-vec2(0.5))*gateScale+vec2(0.5)+vec2(0,0.15*gateRamp),vec2(0),vec2(1)), 4.5*gateRamp );
    imgRear1*=imgRear1.w*(1.0-imgWall.w)*(1.0-imgGate.w);
    imgRear2*=imgRear2.w*(1.0-imgWall.w)*(1.0-imgGate.w);
    imgRear3*=imgRear3.w*(1.0-imgWall.w)*(1.0-imgGate.w);
    imgGate*=imgGate.w*(1.0-imgWall.w);
    imgWall*=imgWall.w;

    float mask =
          clamp((gl_TexCoord[0].x-gl_TexCoord[0].x*gl_TexCoord[0].x)*6,0,1)
        * clamp((gl_TexCoord[0].y-gl_TexCoord[0].y*gl_TexCoord[0].y)*10,0,1);
    mask*=mask;

    gl_FragColor = (imgRear1+imgRear2+imgRear3+imgGate+imgWall) * mask ;
    //gl_FragColor = imgRear1.w*imgRear1;
    gl_FragColor.w = (
                      maskFuncDelta(imageIdx,myImageIdx,0.1) +
                      maskFuncDelta(imageIdx,myImageIdx2,0.1) /*+
                      maskFuncDelta(imageIdx,myImageIdx3,0.1)*/
                     ) * alpha;
    gl_FragColor.xyz *= 1.0-gauss((gl_TexCoord[0].x-0.5)/0.1)*gauss((gl_TexCoord[0].y-0.1)/0.3)*1.3;

    //min(
    //     1.0-clamp(imageIdx-myImageIdx,0.0,1.0), // fade out
    //     clamp(imageIdx-myImageIdx+1.0,0.0,1.0)  // fade in
    //    ) * alpha;

    if(gl_FragColor.w < 0.01) discard;
}
