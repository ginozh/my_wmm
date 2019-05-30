#version 110

uniform sampler2D image;

uniform float radius;
uniform vec2 resolution;
uniform bool horiz_blur;
uniform bool vert_blur;

void main(void) {
	float rad = ceil(radius);
	float x_rad = (horiz_blur) ? rad : 0.5;
	float y_rad = (vert_blur) ? rad : 0.5;
	vec2 texCoord = gl_FragCoord.xy/resolution;
	if (radius == 0.0 || (!horiz_blur && !vert_blur)) {
		gl_FragColor = texture2D(image, texCoord);
	} else {
		float divider = 1.0;
		if (horiz_blur) divider /= rad;
		if (vert_blur) divider /= rad;
		vec4 color = vec4(0.0);
		for (float x=-x_rad+0.5;x<=x_rad;x+=2.0) {					
			for (float y=-y_rad+0.5;y<=y_rad;y+=2.0) {
				color += texture2D(image, (vec2(gl_FragCoord.x+x, gl_FragCoord.y+y))/resolution)*(divider);
			}
		}
		gl_FragColor = color;
	}
}