/*{
	"GUID":"51237319-8F37-4593-BBAF-9FF0A6307C04"
}*/
vec4 FUNCNAME(vec2 tc)
{
	// Grayscale conversion using NTSC conversion weights
	vec4 color = INPUT(tc);
    color.rgb = vec3(1.0,1.0,1.0)-color.rgb;
    
    return color;
}
