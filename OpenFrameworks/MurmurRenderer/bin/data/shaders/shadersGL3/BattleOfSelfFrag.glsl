/*ARB_fragment_shader*/

#version 150 

uniform sampler2D s_tex_0;
uniform sampler2D s_tex_1;

in vec3 normal2;

out vec4 fragColor;

void main()
{
    //gl_FragColor = texture2D( s_tex_0, fract(gl_TexCoord[0].xy) );
    fragColor = vec4(mix(vec3(length(normal2)),-normal2,0.7),1);
    //gl_FragColor = vec4(1,1,1,1);
    //gl_FragColor.xyz = (length(gl_TexCoord[0].xy-vec2(0.5))<0.25) ? (1.0) : vec3(0.0) ;
//    discard;
//    gl_FragColor.xy = fract(gl_TexCoord[0].xy);
//    gl_FragColor = vec4(0.5,0.5,0.5,0.5);
//    gl_FragColor.zw = 0;
//    gl_FragColor = texture2D( s_tex_0, gl_TexCoord[0].xy ).z;
//    gl_FragColor = vec4(1.0);
}
