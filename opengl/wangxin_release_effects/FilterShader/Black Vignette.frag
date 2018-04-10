/*{
        "GUID":"51237319-8F37-4593-BBAF-9FF0A6307C05",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"ColorTuneCircle00.png"
		}
	]
}*/

vec4 FUNCNAME(vec2 tc) 
{
    vec4 tuneColor = INPUT1(tc);
    vec4 blendColor = INPUT2(tc);
	tuneColor.rgb *= blendColor.rgb;
	return tuneColor;
}
