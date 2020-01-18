#version 120

uniform vec2       iResolution;
uniform vec3       BottomLeft;
uniform vec3       TopRight;
float              thickness = 0.05;

float expsmoothstep(float edge0, float edge1, float x) {
  // Scale, bias and saturate x to 0..1 range
  x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0); 
  //return x;
  // Evaluate polynomial
  return x*x*x;
}

// returns 1.0 if inside the disk
float rectangle(vec2 r, vec2 bottomLeft, vec2 topRight, vec2 d) {
	float ret;
	ret = 1.0 - expsmoothstep(bottomLeft.x+d.x, bottomLeft.x, r.x);
	ret *= 1.0 - expsmoothstep(bottomLeft.y+d.y, bottomLeft.y, r.y);
	ret *= 1.0 - expsmoothstep(topRight.y-d.y, topRight.y, r.y);
	ret *= 1.0 - expsmoothstep(topRight.x-d.x, topRight.x, r.x);
	return ret;
}


void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    
    float ratio = iResolution.x/iResolution.y;
    vec2 d = vec2(thickness/ratio, thickness);
    //float col = rectangle(uv, vec2(0.0, 0.0), vec2(1.0, 1.0), d);
    float col = rectangle(uv, BottomLeft.xy, TopRight.xy, d);

    // Output to screen
    gl_FragColor = vec4(vec3(col),1.0);
}