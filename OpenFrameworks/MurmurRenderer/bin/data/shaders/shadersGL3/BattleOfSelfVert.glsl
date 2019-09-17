/*ARB_vertex_shader*/

#version 150

in vec4 position;
in vec3 normal;

out vec3 vnormal;
out vec4 vpos;
 
void main(){
    
    vpos = position;
    gl_Position.xyz = position.xyz;
    vnormal= normal;

    //gl_TexCoord[0] = gl_TextureMatrix[0] * texcoord;


}



