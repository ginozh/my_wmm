uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec4 crop_box;
void main()
{
	vec2 uv = textureCoord;
    uv.x = crop_box.r + (uv.x * (crop_box.b - crop_box.r));
    uv.y = crop_box.g + (uv.y * (crop_box.a - crop_box.g));
	gl_FragColor = texture2D(mSample0, uv);
}
