varying vec3 VertColor;
varying vec2 TextCoord;
varying vec2 textCoord;

uniform sampler2D tex;

//out vec4 color;


void main()
{
    //gl_FragColor = texture2D(tex, vec2(TextCoord.s, 1.0-TextCoord.t));
	gl_FragColor = texture2D(tex, TextCoord);
}
