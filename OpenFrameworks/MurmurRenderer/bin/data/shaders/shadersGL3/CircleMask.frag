#version 150

uniform vec2       iResolution;
float              thickness = 0.07;

out vec4 outputColor;

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    
    uv -=.5;
    uv.x*= iResolution.x/iResolution.y;
    float dist = length(uv);
    float rad = .5;
	float col = smoothstep(rad,rad-thickness, dist);
    
	outputColor = vec4(vec3(col),1.0);
}