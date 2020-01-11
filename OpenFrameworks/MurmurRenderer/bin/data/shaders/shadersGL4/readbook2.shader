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
    float myImageIdx = 1;
    float omega_t = 0.2*time;
    vec2 offs1 = -0.03*vec2(cos(omega_t),cos(omega_t*0.36254));
    vec2 offs2 = 0.03*vec2(sin(omega_t),sin(omega_t*0.36254));
    //vec2 offs3 = 0.03*vec2(cos(omega_t),cos(omega_t*0.36254));

    vec4 img1 = texture2D( s_tex_0, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs1) );
    vec4 img2 = texture2D( s_tex_1, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) );
    vec4 img3 = texture2D( s_tex_2, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) );
    vec4 img4 = texture2D( s_tex_3, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) );
    vec4 img5 = texture2D( s_tex_4, fract(gl_TexCoord[0].xy*vec2(1,-1)+offs2) );

    float mask =
          clamp((gl_TexCoord[0].x-gl_TexCoord[0].x*gl_TexCoord[0].x)*6,0,1)
        * clamp((gl_TexCoord[0].y-gl_TexCoord[0].y*gl_TexCoord[0].y)*10,0,1);
    mask*=mask;

    float f1 = maskFunc((imageIdx-myImageIdx)*4,0);
    float f2 = maskFunc((imageIdx-myImageIdx)*4,1);
    float f3 = maskFunc((imageIdx-myImageIdx)*4,2);
    float f4 = maskFunc((imageIdx-myImageIdx)*4,3);

    vec4 img2345 =
          img2*f1
        + img3*f2
        + img4*f3
        + img5*f4;

    gl_FragColor = (img1*4.0+img2345) * mask ;
    gl_FragColor.w = maskFuncDelta(imageIdx,myImageIdx,0.1) * alpha;
    gl_FragColor.xyz *= 1.0-gauss((gl_TexCoord[0].x-0.5)/0.1)*gauss((gl_TexCoord[0].y-0.1)/0.3)*1.3;

    if(gl_FragColor.w < 0.01) discard;
}
