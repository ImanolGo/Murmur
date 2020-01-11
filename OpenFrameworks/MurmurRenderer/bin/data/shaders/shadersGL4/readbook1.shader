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

uniform float alpha=0.0;
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

vec4 rand01(vec2 texc, int i)
{
    vec2 ires = vec2(1.0)/vec2(textureSize(s_tex_3,0));
    return texture2D( s_tex_3, texc+i*ires );
}

vec4 getTexBlur( sampler2D tex, vec2 texc, float blur)
{
#if 0
    vec4 col = vec4(0);
    float stepnum=max(1,blur*32);
    int i;
    for(i=0;i<stepnum;i++) {
        vec2 offs = (rand01(texc,i).xy-vec2(0.5));
        col+=texture2D( s_tex_0, texc+offs*blur );
    }
    col/=stepnum;
    return col;
#endif
    return texture2D( tex, texc );
}

float gauss(float x)
{
    return exp(-x*x);
}

void main()
{
    float myImageIdx = 0;
    float omega_t = 0.2*time;
    vec2 offs1 = -0.03*vec2(cos(omega_t),cos(omega_t*0.36254));
    vec2 offs2 = 0.03*vec2(sin(omega_t),sin(omega_t*0.36254));
    vec2 offs3 = 0.03*vec2(cos(omega_t),cos(omega_t*0.36254));

    float fade = maskFuncDelta(imageIdx,myImageIdx,0.5);

    /*vec4 img1 = texture2D( s_tex_0, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs1) );
    vec4 img2 = texture2D( s_tex_1, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) );
    vec4 img3 = texture2D( s_tex_2, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs3) );
    */
    vec4 img1 = getTexBlur( s_tex_0, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs1), 1-fade );
    vec4 img2 = getTexBlur( s_tex_1, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2), 1-fade );
    vec4 img3 = getTexBlur( s_tex_2, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs3), 1-fade );
    vec4 img4 = getTexBlur( s_tex_3, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs3), 1-fade );

    float mask =
          clamp((gl_TexCoord[0].x-gl_TexCoord[0].x*gl_TexCoord[0].x)*6,0,1)
        * clamp((gl_TexCoord[0].y-gl_TexCoord[0].y*gl_TexCoord[0].y)*10,0,1);
    mask*=mask;

    gl_FragColor = (img1*4.0+img2*2+(img3+img4)) * mask ;
    gl_FragColor.xyz *= 1.0-gauss((gl_TexCoord[0].x-0.5)/0.1)*gauss((gl_TexCoord[0].y-0.1)/0.3)*1.3;
    gl_FragColor.w = fade * alpha;

    //min(
    //     1.0-clamp(imageIdx-myImageIdx,0.0,1.0), // fade out
    //     clamp(imageIdx-myImageIdx+1.0,0.0,1.0)  // fade in
    //    ) * alpha;

    if(gl_FragColor.w < 0.01) discard;
}
