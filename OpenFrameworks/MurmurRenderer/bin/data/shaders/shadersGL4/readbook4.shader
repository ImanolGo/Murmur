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
uniform sampler2D s_tex_5;
uniform sampler2D s_tex_6;

uniform float alpha=1.0;
uniform float imageIdx=0.0;
uniform float time=0.0;

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

float maskFuncDeltaRep(float index, float maskIndex, float fadeDelta, float maxIndex)
{
    return max(maskFuncDelta(index,maskIndex,fadeDelta),maskFuncDelta(index-maxIndex,maskIndex,fadeDelta));
}

float gauss(float x)
{
    return exp(-x*x);
}

void main()
{
    float myImageIdx = 4;
    float omega_t = 0.2*time;
    vec2 offs1 = -0.03*vec2(cos(omega_t),cos(omega_t*0.36254));
    vec2 offs2 = 0.03*vec2(sin(omega_t),sin(omega_t*0.36254));
    vec2 offs3 = 0.03*vec2(cos(omega_t),cos(omega_t*0.36254));

    vec4 img1 = texture2D( s_tex_0, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs1) );
    vec4 img2 = texture2D( s_tex_1, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) );
    vec4 img3 = texture2D( s_tex_2, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs3) );

    float sailIdx=1.0*time;
    sailIdx=fract(sailIdx/3.0)*3.0;
    vec4 img4 = texture2D( s_tex_3, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) )*maskFuncDeltaRep(sailIdx,0,1.0,3);
    vec4 img5 = texture2D( s_tex_4, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) )*maskFuncDeltaRep(sailIdx,1,1.0,3);
    vec4 img6 = texture2D( s_tex_5, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) )*maskFuncDeltaRep(sailIdx,2,1.0,3);
    vec4 imgSail = img4+img5+img6;

    float mask =
          clamp((gl_TexCoord[0].x-gl_TexCoord[0].x*gl_TexCoord[0].x)*6,0,1)
        * clamp((gl_TexCoord[0].y-gl_TexCoord[0].y*gl_TexCoord[0].y)*10,0,1);
    mask*=mask;

    gl_FragColor = (img1*2+img2*0.8+img3*1.5 + imgSail) * mask ;
    gl_FragColor.w = maskFuncDelta(imageIdx,myImageIdx,1.0) * alpha;
    gl_FragColor.xyz *= 1.0-gauss((gl_TexCoord[0].x-0.5)/0.1)*gauss((gl_TexCoord[0].y-0.1)/0.3)*1.3;

    //min(
    //     1.0-clamp(imageIdx-myImageIdx,0.0,1.0), // fade out
    //     clamp(imageIdx-myImageIdx+1.0,0.0,1.0)  // fade in
    //    ) * alpha;

    if(gl_FragColor.w < 0.01) discard;
}
