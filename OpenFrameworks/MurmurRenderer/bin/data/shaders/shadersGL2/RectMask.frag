#version 120

uniform vec2       iResolution;
uniform vec3       BottomLeft;
uniform vec3       TopRight;
float              thickness = 0.02;

// returns 1.0 if inside the disk
float rectangle(vec2 r, vec2 bottomLeft, vec2 topRight, vec2 d) {
	float ret;
	ret = mix(bottomLeft.x, bottomLeft.x+d.x, r.x);
	ret *= mix(bottomLeft.y, bottomLeft.y+d.y, r.y);
	ret *= 1.0 - mix(topRight.y-d.y, topRight.y, r.y);
	ret *= 1.0 - mix(topRight.x-d.x, topRight.x, r.x);
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