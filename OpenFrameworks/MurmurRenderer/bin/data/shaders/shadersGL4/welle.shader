/*ARB_vertex_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;
uniform sampler2D s_tex_3;
/* input */
//uniform vec2 res=vec2(1024);
//uniform vec2 randres;
uniform float depth = 0.0;

/* out 2 fragment shader */

void main()
{
    vec4 dummy, vpos;

    vpos = gl_ModelViewMatrix * gl_Vertex;

    gl_Position = gl_ProjectionMatrix * vpos;

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[0].z = depth;
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler3D s_tex_0;
uniform sampler2D s_tex_1;
uniform sampler2D s_tex_2;
uniform sampler2D s_tex_3;

//uniform float delta = 1.0;
uniform float strength = 500;
uniform float viscosity = 0.0;
uniform float dt = 0.03;
uniform float randomness = 0.3;
uniform float wellness = 0.1;
uniform float init = 0;
uniform vec2 rand;

//varying vec2 ires;

vec4 blow(vec2 pos, vec2 size, vec3 strength, vec2 texc)
{
    float s = 0.1;
    if( texc.x<pos.x+size.x && texc.x>pos.x-size.x && texc.y<pos.y+size.y && texc.y>pos.y-size.y )
    {
        return vec4(strength.x,strength.y,strength.z,0);
    }
    else
    {
        return vec4(0.0);
    }
}

vec4 softDot(vec2 pos, float r, vec4 strength, vec2 texc)
{
    float d=length(texc-pos);
    float s = 0.1;
    if( d<r )
    {
        return strength*(r-d)/r;
    }
    else
    {
        return vec4(0.0);
    }
}

void main()
{
    #define colTex s_tex_0
    #define randTex s_tex_3
    vec4   v, texc, v00, vp0, vm0, v0p, v0m, vpp, vmm, vpm, vmp;
    vec2   vdelta;
    vec4   rand01;

    float strength=500;
    float wellness=1.1;
//    float stepnum = 8.0;

    texc = gl_TexCoord[0];
    float z=gl_TexCoord[0].z;

    vec2 res=textureSize(s_tex_0,0).xy;
    vec2 randres=vec2(textureSize(randTex,0));
    vec2 randTexCoord = texc.xy*res/randres+rand;

    v00 = texture3D( s_tex_0, vec3(texc.xy,z) );
    rand01 = texture2DLod( randTex, randTexCoord/*+v00.xy*15121.12341253*/, 0 );
//    rand=0.5;

    vec2 ires=vec2(1.0)/res;

    vdelta=vec2(ires)*2.5;
//    vdelta=vec2(0.002);

    vec3 texc2=gl_TexCoord[0].xyz;

    vp0 = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2( 1.0, 0.0),z) );
    vm0 = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2(-1.0, 0.0),z) );
    v0p = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2( 0.0, 1.0),z) );
    v0m = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2( 0.0,-1.0),z) );
    vpp = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2( 1.0, 1.0),z) );
    vmm = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2(-1.0,-1.0),z) );
    vmp = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2(-1.0, 1.0),z) );
    vpm = texture3D( s_tex_0, vec3(texc2.xy+vdelta*vec2( 1.0,-1.0),z) );

    vec2 d;
    d.x = dot(vec3(0.333333),vp0-vm0+vpp-vmp+vpm-vmm);
    d.y = dot(vec3(0.333333),v0p-v0m+vpp-vpm+vmp-vmm);

    float acc = strength*((vp0.x+v0p.x+vm0.x+v0m.x)*0.125+(vpp.x+vmm.x+vmp.x+vpm.x)*0.125-v00.x) /*- (v00.x-0.5)*strength*0.1*/ - viscosity*v00.y;
    acc=acc*dt*0.5;
    v00.y+=acc;
    v00.x+=v00.y*dt;
    v00.y+=acc;

    v00.xy=v00.xy*0.7+(vp0.xy+v0p.xy+vm0.xy+v0m.xy)*0.25*0.3;

//    v00+=blow(vec2(0.5), vec2(0.02), vec3(1.0), texc2);
//    v00+=blow(vec2(0.5), vec2(0.01), vec3(0.01,0,0), texc2);
//    v00=0.0;

    gl_FragData[0].x = v00.x;
    gl_FragData[0].y = v00.y;
    v00.xy = clamp(v00.xy,vec2(-1.0,-1.0),vec2(1.0,1.0));
    gl_FragData[0] = mix(texture3D( s_tex_0, vec3(texc2.xy+(d.xy+mix(vec2(0.0),rand01.xy-vec2(0.5),randomness))*ires*1.4,z) ),v00,wellness);
    
    gl_FragData[0].x*=gl_FragData[0].x>0.9?0.6:0.995;
    gl_FragData[0].y*=gl_FragData[0].x>0.9?0.6:0.995;
    gl_FragData[0].z*=gl_FragData[0].x>0.9?0.6:0.995;
//    gl_FragColor = texture2D( s_tex_0, texc2.xy+(d.xy+mix(0.0,rand.xy-vec2(0.5),randomness))*ires*1.4 );
//    gl_FragColor = v00;
//    gl_FragColor.xy = fract(gl_TexCoord[0].xy*5);
//    gl_FragColor = gl_TexCoord[0];
//    gl_FragColor.xyzw = vec4(0.0,0.0,0,0);
//    gl_FragData[0] = vec4(0.0);
    //gl_FragData[0].xyzw += softDot(vec2(0.5,0.5),0.05,0.3*vec4(0.6,0.3,0.2,0.6),texc2);
//    gl_FragData[0].xyzw = vec4(0.0);

    if(init > 0.5) gl_FragData[0] = vec4(0);
    gl_FragData[1] = vec4(0);
    gl_FragData[2] = vec4(0);
//    gl_FragData[0].xy += blow(vec2(0.5), vec2(0.05), vec3(0.1), texc2);
}
