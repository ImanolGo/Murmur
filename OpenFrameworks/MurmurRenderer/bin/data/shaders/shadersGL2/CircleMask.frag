#version 120

uniform vec2       iResolution;
float              thickness = 0.09;

float expsmoothstep(float edge0, float edge1, float x) {
  // Scale, bias and saturate x to 0..1 range
  x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0); 
  //return x;
  // Evaluate polynomial
  return x*x;
}


void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    
    uv -=.5;
    uv.x*= iResolution.x/iResolution.y;
    float dist = length(uv);
    float rad = .5 - thickness;
	float col = 1.0 - expsmoothstep(rad,rad+thickness, dist);

    // Output to screen
    gl_FragColor = vec4(vec3(col),1.0);
}