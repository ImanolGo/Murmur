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

    // direct draw test
    //gl_Position = gl_Vertex;
    //gl_Position=vec4(gl_TexCoord[0].xy*2.0-vec2(1,1),0,1);
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
uniform float init = 0.0;

varying vec2 ires;

varying vec2 randTexCoord;


vec4 benard2D(vec3 texc_)
{
    vec2 texc=texc_.xy;
    float z=texc_.z;

    float eps=0.01; // prop fact for teperqature diff and speed gain per time
    float T=1.0;    // upper temperature
    float dt=0.03;   // time interval
    //vec2 ires = vec2(1.0/256.0,1.0/256.0);
    //dE=v.z*t+;
    vec4 tex;
    tex=texture2D(s_tex_0,vec3(texc,z).xy);
    vec2 offs=-(tex.xy-vec2(0.5))*dt*30.0*ires;
    tex=texture2D(s_tex_0,vec3(texc+offs,z).xy);
    vec3  v=tex.xyz-vec3(0.5);
    float t=tex.w;

    vec2 d[4];
    d[0]=vec2(-1, 0);
    d[1]=vec2( 1, 0);
    d[2]=vec2( 0,-1);
    d[3]=vec2( 0, 1);

    int i;
    for(i=0;i<4;i++)
    {
        tex=texture2D(s_tex_0,vec3(texc+offs+d[i]*ires*1.42734,z).xy);
        vec3  vd=tex.xyz-vec3(0.5);
        float td=tex.w;
    	v.xy-=d[i]*vd.z;
    }
    t=mix(0.9*t,T,v.z+0.5);
    v.z-=eps*(t-T)*dt;
    v-=0.005*normalize(v);
    vec4 col = vec4(v+vec3(0.5),t);
    //col.w=sin(texc.x*3.12343)*sin(texc.y*3.12343*4.0);
    return col;
}



void main()
{
    vec4   v, texc, v00, vp0, vm0, v0p, v0m, vpp, vmm, vpm, vmp;
    vec3   vdelta;
    vec4   rand;
//    float delta;

    // get some random numbers
    rand = texture2D( s_tex_1, fract(randTexCoord) );

//    obstacle = texture2D( s_tex_1, fract(randTexCoord) );

    texc = gl_TexCoord[0];

    v00 = texture2D( s_tex_0, texc.xy ) - vec4(0.5,0.5,0.5,0.0);
    v00 = texture2D( s_tex_0, texc.xy );
//    texc.xy-=ires*(0.2*(rand.xy-vec2(0.5))+v00.xy*22.0);
//    texc.xy+=ires*(v00.xy*0.5);

    float rot = 0.0;

    int i;
    for(i=0;i<int(stepnum);i++)
    {
//        float strengthFact=strength*freqFalloff/(freqFalloff+float(i)/float(stepnum))/*(1.0-0.5-0.5*texc.y)*/;

        float strengthFact=strength*freqFalloff/(freqFalloff+float(i)/float(stepnum))*(1.0-0.5*texc.y);

        vdelta=1.0*pow(2.0,float(stepnum-1-i))*vec3(ires*delta,0);

        vp0 = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 1.0, 0.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;
        vm0 = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2(-1.0, 0.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;
        v0p = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 0.0, 1.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;
        v0m = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 0.0,-1.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;
        vpp = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 1.0, 1.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;
        vmm = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2(-1.0,-1.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;
        vmp = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2(-1.0, 1.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;
        vpm = texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 1.0,-1.0) )/*-vec4(0.5,0.5,0.5,0.0)*/;

    /* ranges from min -6.0*res to max 6.0*res */
        rot += ( vp0.y - vm0.y + vpm.y - vmm.y + vpp.y - vmp.y )/ires.x/float(stepnum);
        rot -= ( v0p.x - v0m.x + vmp.x - vmm.x + vpp.x - vpm.x )/ires.y/float(stepnum);

/*    rot  = (vp0.y-vm0.y)/2.0/ires.x - (v0p.x-v0m.x)/2.0/ires.y;
    rot += (vpm.y-vmm.y)/2.0/ires.x - (vmp.x-vmm.x)/2.0/ires.y;
    rot += (vpp.y-vmp.y)/2.0/ires.x - (vpp.x-vpm.x)/2.0/ires.y;
    rot/=3.0;*/

        texc.x += (v0p.z-v0m.z+vpp.z-vpm.z+vmp.z-vmm.z)*ires.x*strengthFact;
        texc.y -= (vp0.z-vm0.z+vpp.z-vmp.z+vpm.z-vmm.z)*ires.y*strengthFact;
//    texc.xy+=0.5*(rand.xy-vec2(0.5))*ires/double(stepnum);
//    texc.xy *= ires;
/*    texc.xy = vec2(0);
    texc.xy += vec2(1,0)*(v0p.z-v0m.z)*ires;
    texc.xy -= vec2(0,1)*(vp0.z-vm0.z)*ires;
    texc.xy += vec2(1,0)*(vpp.z-vpm.z)*ires;
    texc.xy -= vec2(0,1)*(vpp.z-vmp.z)*ires;
    texc.xy += vec2(1,0)*(vmp.z-vmm.z)*ires;
    texc.xy -= vec2(0,1)*(vpm.z-vmm.z)*ires;
    texc.xy *= 1.0;*/
    }


    gl_FragColor.xy = texture2D( s_tex_0, texc.xy+0.5*vec2((rand.xy-vec2(0.5))*ires) ).xy /*- 0.0005*nabla_rho*/;
//    gl_FragColor.xy = texture3D( s_tex_0, texc.xyz );
    if(smooth_>0.01)
    {
        gl_FragColor.xy += texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 1.0, 0.0) ).xy;
        gl_FragColor.xy += texture2D( s_tex_0, texc.xy+vdelta.xy*vec2(-1.0, 0.0) ).xy;
        gl_FragColor.xy += texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 0.0, 1.0) ).xy;
        gl_FragColor.xy += texture2D( s_tex_0, texc.xy+vdelta.xy*vec2( 0.0,-1.0) ).xy;
        gl_FragColor.xy /= 5.0;
    }
    gl_FragColor.z = ((ires.x+ires.y)/12.0*rot+0.5);

    gl_FragColor = benard2D(gl_TexCoord[0].xyz);

    gl_FragColor.xyz=0.99*(gl_FragColor.xyz-vec3(0.5))+vec3(0.5);
    if(init>0.5) gl_FragColor.xyz=vec3(0);
    //gl_FragColor.xyz=0.98*gl_FragColor.xyz;

    //gl_FragColor.xyz=vec3(1);
    
//    gl_FragColor = vec4(gl_TexCoord[0].z);
//    gl_FragColor = vec4(1);
//    gl_FragColor =vec4(delta*0.5);
//    gl_FragColor = vec4(rand);

//    v00 = texture2D( s_tex_0, texc.xy );

    // get color
//    gl_FragColor = texture2D( s_tex_0, texc.xy );
/*    gl_FragColor = 1.0/5.0 * (
                              texture2D( s_tex_0, texc.xy ) +
                              texture2D( s_tex_0, texc.xy+vec2( 0.002, 0.0) ) +
                              texture2D( s_tex_0, texc.xy+vec2(-0.002, 0.0) ) +
                              texture2D( s_tex_0, texc.xy+vec2( 0.0, 0.002) ) +
                              texture2D( s_tex_0, texc.xy+vec2( 0.0,-0.002) )
                             );
    gl_FragColor.zw = texture2D( s_tex_0, texc.xy ).zw;*/
//    gl_FragColor.zw = v.xy+vec2(0.5);
}
