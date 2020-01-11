/*ARB_vertex_shader*/

#version 430 compatibility

/* input */
uniform vec2 res;
uniform vec2 rand;
uniform vec2 randres;
uniform float depth = 0.0;

//uniform vec4 arg1;

/* out 2 fragment shader */
varying vec2 ires;

varying vec2 randTexCoord;

void main()
{
    vec4 dummy, vpos;

    vpos = gl_ModelViewMatrix * gl_Vertex;

    gl_Position = gl_ProjectionMatrix * vpos;

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[0].z = depth;

    ires = vec2(1.0/res.x,1.0/res.y);
    randTexCoord = gl_MultiTexCoord0.xy*res/randres+rand;
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;

uniform float delta = 1.0;
uniform float strength = 0.4;
uniform float freqFalloff=-1.0;
uniform float stepnum = 1.0;
uniform float smooth_ = 0.0;
uniform float init = 0;
uniform float fadeOut = 1;

varying vec2 ires;

varying vec2 randTexCoord;

void main()
{
    vec4 texc = gl_TexCoord[0];
    vec4 colx = texture2D( s_tex_0, texc.xy );
    gl_FragColor = clamp(colx-vec4(0.003),vec4(0),vec4(1));
    if(init>0.5) gl_FragColor.xyz=vec3(0);
}
