/*ARB_vertex_shader*/

#version 430 compatibility

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
//    gl_Position = ftransform();
    gl_Position.xyz = gl_Vertex.xyz;
    gl_Position.w = 1;
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;
uniform sampler2D s_tex_2;
uniform sampler2D s_tex_3;
uniform sampler2D s_tex_4;
uniform sampler2D s_tex_5;

uniform float deltaTime = 0.0;

uniform int db_0 = 0;
uniform int db_1 = 1;
uniform int db_2 = 2;

uniform vec3 pos0 = vec3(0,0,0);
uniform vec3 pos0Offs = vec3(0,0,0);
uniform float pos2Scale = 1.0;
uniform float posScale = 1.0;
uniform float forceFact = 1.0;
uniform float velFact = 1.0;
uniform float timeFact = 1.0;
uniform vec3 offsScale=vec3(1,1,1);
uniform float shoo = 0.0;
uniform float init = 0.0;
uniform float bypassTracking=0;

vec3 nnAcc(vec3 dist,float d0)
{
    dist/=pos2Scale;
    d0/=pos2Scale;
    float nnC=0.1*pos2Scale;
    float ld=length(dist);
    if(length(dist)>d0)
        return -nnC*normalize(dist)*clamp(1.0+ld,0.0,1.0);
    else
        return nnC*normalize(dist)*clamp(1.0+ld,0.0,1.0);
    return vec3(0);
}

vec3 nnAccR(vec3 dist,float d0)
{
    dist/=pos2Scale;
    d0/=pos2Scale;
    float nnC=0.1*pos2Scale;
    float ld=length(dist);
    if(length(dist)>d0)
        return vec3(0);
    else
        return nnC*normalize(dist)*clamp(1.0+ld,0.0,1.0);
    return vec3(0);
}

void main()
{
    //posScale=posScale*posScale/10/10;
    vec3 pos0In=pos0;
    if(bypassTracking>=1.0) pos0In=vec3(0,0,0); // bypass tracking

    vec2 ires = vec2(1.0)/textureSize(s_tex_0,0);
    vec3 pos = texture2D(s_tex_0,gl_TexCoord[0].xy).xyz;
    float poslen=length(pos);
    float forceMinDist=1.0;
    vec3 posm0 = texture2D(s_tex_0,gl_TexCoord[0].xy+ires*vec2(-1,0)).xyz;
    vec3 posp0 = texture2D(s_tex_0,gl_TexCoord[0].xy+ires*vec2( 1,0)).xyz;
    vec3 pos0m = texture2D(s_tex_0,gl_TexCoord[0].xy+ires*vec2(0,-1)).xyz;
    vec3 pos0p = texture2D(s_tex_0,gl_TexCoord[0].xy+ires*vec2(0, 1)).xyz;
    vec3 vel =texture2D(s_tex_1,gl_TexCoord[0].xy).xyz;
    vec3 attr=texture2D(s_tex_2,gl_TexCoord[0].xy).xyz;

    vec3 acc = vec3(0);
    vec3 pos1;
    //if(fract(gl_TexCoord[0].x*100+gl_TexCoord[0].y*100.0)<0.3)
    //acc = acc - (pos-(vec3(1,1,1)*pos0-vec3(0,-5.0,1.9))*5.0*vec3(1,0,1))*(0.1+1.0*pow(length(gl_TexCoord[0].xy),6.0))*5.0;

    //vec3 pos0_ = (offsScale*pos0-vec3(+0.5,-5.0,1.9))*5.0*vec3(1,0,1);
    vec3 pos0_ = (offsScale*pos0In+pos0Offs/**posScale*/)/(posScale+0.5);
    if (posScale<0.1) pos0_=vec3(0,0,0); // get bird target back to zero if swarm is scaled to
    //vec3 pos0_ = vec3(-1,0.3,0);
    // attraction to pos0
    acc = acc - (pos-pos0_)/pos2Scale*(0.1+1.0*pow(length(gl_TexCoord[0].xy),6.0))*5.0;
    // shoo from pos0
    acc = acc + (pos-pos0_)/pos2Scale*shoo/(dot(pos-pos0_,pos-pos0_)/pos2Scale/pos2Scale+0.001);
    // attraction to leader
    pos1 =texture2D(s_tex_0,ires*1.5).xyz;
    acc = acc - (pos-pos1)/pos2Scale*0.1;
    // velocity damping
    acc = acc - vel*1.8;

//    acc = acc + 0.1*;
    // next neighbours
    float d0 = (2.0+2.0*texture2D(s_tex_4,gl_TexCoord[0].xy/vec2(64)/ires).x);
    acc = acc + nnAcc(posm0-pos,d0);
    acc = acc + nnAcc(posp0-pos,d0);
    acc = acc + nnAcc(pos0m-pos,d0);
    acc = acc + nnAcc(pos0p-pos,d0);

    float possq = dot(pos,pos);
    float posl = sqrt(possq);
    float poslxy = sqrt(pos.x*pos.x+pos.y*pos.y);
    float costh = pos.z/posl;
    float sinth = poslxy/posl;
    vec3 br=pos/posl;
    vec3 bth=vec3(-pos.x/poslxy*costh,-pos.y/poslxy*costh,sinth);
    // dipole force (toric vortex)
    vec3 dipforce = 10.1*(2.0*costh*br+sinth*bth)/(possq*possq*posl/pos2Scale/pos2Scale/pos2Scale/pos2Scale/pos2Scale/**posl*/+0.01);
    if(poslen>forceMinDist)
        acc=acc+dipforce;

    // z-vortex
    vec3 bph=cross(br,bth);
    if(poslen>forceMinDist)
        acc=acc+3.0*bph/(possq*poslxy/pos2Scale/pos2Scale/pos2Scale+0.001);

    int i;
    int j;
    vec2 s=vec2(1,1);
    for(i=1;i<20;i++)
    {
        s.x=-s.x;
	for(j=1;j<20;j++)
        {
            s.y=-s.y;
            vec3 delta=texture2D(s_tex_0,gl_TexCoord[0].xy+vec2(i,j)*ires.xy).xyz-pos;
            vec3 vdelta=texture2D(s_tex_1,gl_TexCoord[0].xy+vec2(i,j)*ires.xy).xyz-vel;
            //float d0 = (2.0+2.0*texture2D(s_tex_4,(gl_TexCoord[0].xy+vec2(i,j)*ires.xy)/vec2(64)/ires).x);
            float deltasq=dot(delta,delta);
            //acc=acc-delta*0.001/(deltasq*deltasq*deltasq+0.01);
            acc = acc + 0.3*nnAccR(delta,d0) - vdelta*0.002;
            //acc=acc+delta*0.01/(deltasq*deltasq*deltasq+0.01);
            //acc=acc+delta*0.01/(deltasq*deltasq+0.01);
        }
    }

    float dt=deltaTime*0.3*timeFact;
    gl_FragData[0] = vec4(pos/*+vec3(0.001,0,0)*/+vel*dt,1);
    float vmax=5.0;
    vel=vel+acc*dt; float vl=length(vel); if(vl>vmax) vel=vel*(vmax/vl);
    gl_FragData[1] = vec4(vel+acc*forceFact*dt,1)*velFact;
    gl_FragData[2] = vec4(attr+vec3(0.05234,0.005223,0)*dt*length(vel)/0.03,0);
    //gl_FragData[0].xyz = texture2D(s_tex_4,gl_TexCoord[0].xy/vec2(64)/ires).xyz*10.0;

    if(init>0.0)
    {
        gl_FragData[0] = vec4(gl_TexCoord[0].xyz-vec3(0.5,0.5,0),1);
        gl_FragData[1] = vec4(gl_TexCoord[0].xzy-vec3(0.5,0,0.5),1);
    }
}
