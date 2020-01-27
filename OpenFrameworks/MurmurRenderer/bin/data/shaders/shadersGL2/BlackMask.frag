#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex0;
uniform sampler2DRect imageMask;

varying vec2 texCoordVarying;

void main()
{
    vec4 texel0 = texture2DRect(tex0, texCoordVarying);
    vec4 texel1 = texture2DRect(imageMask, texCoordVarying);
    gl_FragColor = vec4(texel0.rgb, texel0.a * texel1.r);
}

// #version 120
// #extension GL_ARB_texture_rectangle : enable

// uniform sampler2DRect tex0;
// uniform sampler2DRect imageMask;

// varying vec2 texCoordVarying;

// void main()
// {
//     vec2 pos = gl_TexCoord[0].st;
//     vec3 src = texture2DRect(tex0, pos).rgb;
//     float mask = texture2DRect(imageMask, pos).r

//     gl_FragColor = vec4( src , mask);
// }