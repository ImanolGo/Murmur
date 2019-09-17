/*ARB_vertex_shader*/
#version 400 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;
uniform sampler2D s_tex_2;

out vec4 vpos;
out vec3 angles;
out vec3 vel;
out float drawme;

uniform float birdSep = 1.0;
uniform float posScale = 3.0;
uniform vec3 pos0 = vec3(0,0,0);
uniform vec3 offsScale = vec3(1,1,1);
uniform vec3 pos0Offs = vec3(0,0,0);
uniform float birdOffsZ=-2.9;

void main()
{
    //posScale=posScale*posScale/10/10;
    gl_TexCoord[0] = gl_MultiTexCoord0;

    vec2 res=textureSize(s_tex_0,0);

    vec2 index2 = gl_TexCoord[0].xy*res;
    int index = int(index2.y*res.x+index2.x);

    float level = birdSep*0.1/**log2(res.x*res.y)*/;

    int indexp = index & ((1<<int(level+1))-1);
    int index0 = index;
    index = index & ((1<<int(level))-1);

    float fact=fract(level);

    vec2 texcp = vec2(0.5)/res+vec2(indexp%int(res.x),indexp/res.x)/res;
    vec2 texc = vec2(0.5)/res+vec2(index%int(res.x),index/res.x)/res;

    //    gl_Position = ftransform();
    //vec3 pos = texture2D(s_tex_0,gl_TexCoord[0].xy).xyz;
    vec3 pos = texture2D(s_tex_0,texc).xyz;
    vec3 posp = texture2D(s_tex_0,texcp).xyz;

    pos=mix(pos,posp,fact);

    drawme=1.5;
    if( index0 > (1<<(int(level)))+100 ) drawme=0.0;

    vec3 pos0_ = (offsScale*pos0+pos0Offs)/(posScale+0.5);
    pos0_=vec3(0);

    //vpos = vec4((pos-pos0_)*posScale+vec3(0,-3.5,0),1.0);
    //float birdOffsZ=-2.9;
    vpos = vec4((pos-pos0_)*posScale+pos0_+vec3(0,0,birdOffsZ),1.0);
    vel = texture2D(s_tex_1,texc).xyz;
    vec3 velp = texture2D(s_tex_1,texcp).xyz;
    vel=mix(vel,velp,fact);
    angles = texture2D(s_tex_1,texc).xyz*10.0;
    vec3 anglesp = texture2D(s_tex_1,texcp).xyz*10.0;
    angles=mix(angles,anglesp,fact);
    gl_Position = vec4(0.0);
}

/*EXT_geometry_shader*/
//InputPrimitive POINTS
//OutputPrimitive TRIANGLE_STRIP
//VerticesOut 24

//#version 120
#version 400 compatibility
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

in vec4 vpos[];
in vec3 angles[];
in vec3 vel[];
in float drawme[];
out vec3 normal;
out vec4 vpos2;
out vec2 texc;
out float gsmode;

//uniform float size=0.02;
uniform float size=0.0;
uniform float allScale = 1.0;
uniform float index;
uniform float mode=0;

void main(void)
{
    gsmode=mode;
#if 1
    if(mode<1.0 && drawme[0]>=1.0) // sitting
    {
        vec2 xynum=vec2(7,1);
        float bP=mode*20;
        vec2 texc0;
        texc0 = vec2( fract(floor(bP)/xynum.x) , floor((bP)/xynum.x)/xynum.y );

        texc = texc0+vec2(0,1)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4(-1,0,-1,0)*size)*allScale;
        EmitVertex();
        texc = texc0+vec2(1,1)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4( 1,0,-1,0)*size)*allScale;
        EmitVertex();
        texc = texc0+vec2(0,0)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4(-1,0, 1,0)*size)*allScale;
        EmitVertex();
        texc = texc0+vec2(1,0)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4( 1,0, 1,0)*size)*allScale;
        EmitVertex();
        EndPrimitive();
    }
    else if(mode<2.0 && drawme[0]>=1.0) // takeoff
    {
        vec2 xynum=vec2(5,2);
        float bP=(mode-1.0)*10;
        vec2 texc0;
        texc0 = vec2( fract(floor(bP)/xynum.x) , floor((bP)/xynum.x)/xynum.y );

        texc = texc0+vec2(0,1)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4(-1,0,-1,0)*size)*allScale;
        EmitVertex();
        texc = texc0+vec2(1,1)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4( 1,0,-1,0)*size)*allScale;
        EmitVertex();
        texc = texc0+vec2(0,0)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4(-1,0, 1,0)*size)*allScale;
        EmitVertex();
        texc = texc0+vec2(1,0)/xynum;
        gl_Position = gl_ModelViewProjectionMatrix * (vpos[0]+vec4( 1,0, 1,0)*size)*allScale;
        EmitVertex();
        EndPrimitive();
    }
    else if(drawme[0]>=1.0) // fly
#endif
    {
        vec3 bx=normalize(vec3(-vel[0].y,vel[0].x,0));
        vec3 by=normalize(vel[0]);
        vec3 bz=cross(bx,by);
	gl_TexCoord[0].xy = gl_TexCoordIn[0][0].xy;

        float angle = angles[0].x*5.0;
        float cosangle = cos(angle*0.2)*0.6;
        float cosangle2 = cosangle*0.2;
        float sw=1.0;

        //left wing
        normal = bz;

        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(- 0-by-cosangle2*bz,0) );
        texc = vec2(0.5,1)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(+bx*sw-by+cosangle*bz,0) );
        texc = vec2(0.0,1)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(- 0+by-cosangle2*bz,0) );
        texc = vec2(0.5,0)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(+bx*sw+by+cosangle*bz,0) );
        texc = vec2(0.0,0)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        EndPrimitive();

        //right wing
        normal = bz;

        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(-bx*sw-by+cosangle*bz,0) );
        texc = vec2(1.0,1)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(+ 0-by-cosangle2*bz,0) );
        texc = vec2(0.5,1)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(-bx*sw+by+cosangle*bz,0) );
        texc = vec2(1.0,0)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(+ 0+by-cosangle2*bz,0) );
        texc = vec2(0.5,0)*vec2(0.5,1);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        EndPrimitive();

        //yz plane
        normal = bx;

        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(-by-bz-cosangle2*bz,0) );
        texc = vec2(0,0)*vec2(0.5,1)+vec2(0.5,0);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(+by-bz-cosangle2*bz,0) );
        texc = vec2(1,0)*vec2(0.5,1)+vec2(0.5,0);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(-by+bz-cosangle2*bz,0) );
        texc = vec2(0,1)*vec2(0.5,1)+vec2(0.5,0);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        vpos2 = gl_ModelViewMatrix * ( vpos[0] + size*vec4(+by+bz-cosangle2*bz,0) );
        texc = vec2(1,1)*vec2(0.5,1)+vec2(0.5,0);
	gl_Position = gl_ProjectionMatrix * vpos2;
	EmitVertex();
        EndPrimitive();
    }
}

/*ARB_fragment_shader*/

#version 400 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;
uniform sampler2D s_tex_2;
uniform sampler2D s_tex_3;
uniform sampler2D s_tex_4;
uniform sampler2D s_tex_5;
uniform sampler2D s_tex_6;


uniform int db_0 = 0;
uniform int db_1 = 1;
uniform int db_2 = 2;

varying vec3 normal;
varying vec4 vpos2;
varying vec2 texc;
varying float gsmode;

uniform float hide = 0;

void main()
{

#define envTex s_tex_3

      mat4 viewMatrix = gl_TextureMatrix[4];
//      gl_FragColor = texture2D(s_tex_1,gl_TexCoord[0].xy)*0.5*5.0+vec4(vec3(0.5),0);
//      gl_FragColor = texture2D(s_tex_0,gl_TexCoord[0].xy)*0.5*5.0+vec4(vec3(0.5),0);
//      gl_FragColor = texture2D(s_tex_2,gl_TexCoord[0].xy);
//      gl_FragColor = vec4(normal*0.5+vec3(0.5),1);

      vec3 light0 = gl_LightSource[0].position.xyz-vpos2.xyz;
//      vec3 light0 = vec3(100.0,100,100)-vpos2.xyz;
      vec3 R = normalize(-reflect(-vpos2.xyz,normal)) * gl_NormalMatrix;
      R = (vec4(R,0)*viewMatrix).xyz;
//    R.xyz=-R.xzy;
      vec2 rtexc = normalize(R.xy)*sqrt((1.0-R.z)/2.0);
      rtexc = rtexc*0.5+0.5;
      float shiny=5.0;

      float diff=clamp(length(dot(normal,normalize(vec3(1,0,1)))),0.2,1.0);

      gl_FragData[db_0] = vec4(normal*0.5+vec3(0.5),1);
      //float alpha=1.0-texture2D(s_tex_3,texc).w;
      if(gsmode<0.99)
          gl_FragData[db_0] = texture2D(s_tex_5,texc)*1;
      else if(gsmode<1.99)
          gl_FragData[db_0] = texture2D(s_tex_6,texc)*1;
      else
      {
          gl_FragData[db_0] = texture2D(s_tex_3,texc)*2;
          gl_FragData[db_0].xyz *= diff;
      }
      //gl_FragData[db_0].xyz *= gl_FragData[db_0].w;
      //gl_FragData[db_0] = vec4(0.3);
      //gl_FragData[db_0].w = alpha;
      if(gl_FragData[db_0].w < 0.01) discard;
      if(hide>=1.0) discard;
//      gl_FragColor = vec4(normal*0.5+vec3(0.5),1);
      gl_FragData[db_1] = vec4(vec3(mix(diff,1.0,0.2)),1);
//      gl_FragData[db_0].xyz = normal;
      float fresnel=(1.0-clamp(dot(normal,normalize(-vpos2.xyz)),0.0,1.0));
      fresnel*=fresnel;
      fresnel*=fresnel;
      gl_FragData[db_2] = texture2D(envTex,rtexc,shiny)*(0.04+fresnel*0.96);
//      gl_FragData[db_2] = vec4(fresnel);

//      gl_FragData[1] = texture2D(s_tex_2,gl_TexCoord[0].xy);
//      gl_FragData[0] = vec4(1);
//      gl_FragColor = vec4(vec3(clamp(dot(normal,vec3(1,1,1)),0.2,1.0)),1);
}
