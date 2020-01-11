/*ARB_vertex_shader*/

#version 430 compatibility

void main()
{
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = ftransform();
}


/*ARB_fragment_shader*/

#version 430 compatibility

uniform sampler2D s_tex_0;
uniform vec4 pickcolor;
uniform float blink = 1;

bool myCmp(vec2 texc)
{
    return(
           texture2D(s_tex_0,texc).x == pickcolor.x &&
           texture2D(s_tex_0,texc).y == pickcolor.y &&
           texture2D(s_tex_0,texc).z == pickcolor.z &&
           texture2D(s_tex_0,texc).w == pickcolor.w
          );
}

void main()
{
    if(
       !myCmp(gl_TexCoord[0].xy) &&
       (
        myCmp(gl_TexCoord[0].xy-vec2( 0.003,0)) ||
        myCmp(gl_TexCoord[0].xy-vec2(-0.003,0)) ||
        myCmp(gl_TexCoord[0].xy-vec2(0, 0.003)) ||
        myCmp(gl_TexCoord[0].xy-vec2(0,-0.003))
       )
      )
    {
        gl_FragColor = vec4(vec3(1,1,0)*(1.0-0.1*fract(blink)),0.8);
    }
    else if(myCmp(gl_TexCoord[0].xy))
    {
        gl_FragColor = vec4(vec3(1,1,1)*fract(blink),0.5);
    }
    else
    {
        discard;
    }
    //gl_FragColor = texture2D(s_tex_0,gl_TexCoord[0].xy);
}
