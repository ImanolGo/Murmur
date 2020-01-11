/*EXT_geometry_shader*/
//InputPrimitive LINES
//OutputPrimitive TRIANGLE_STRIP
//VerticesOut 24

#version 150 

uniform float nshift=0.0;
uniform float thick=0.03;

in vec4 vpos[];
in vec3 vnormal[];
out vec3 normal2;


void main(void)
{
    
    normal2=vnormal[0];
    gl_Position = (gl_in[0].gl_Position * (vpos[0] +(nshift-0.5*thick)*vec4(normal2,0))) ;
    EmitVertex();
    gl_Position =  (gl_in[0].gl_Position * (vpos[0] + (nshift+0.5*thick)*vec4(normal2,0))) ;
    EmitVertex();

    normal2=vnormal[1];
    gl_Position = = (gl_in[1].gl_Position * (vpos[1] + (nshift-0.5*thick)*vec4(normal2,0)));
    EmitVertex();
    gl_Position =  (gl_in[1].gl_Position * (vpos[1] + (nshift+0.5*thick)*vec4(normal2,0)));
    EmitVertex();
    EndPrimitive();

}
