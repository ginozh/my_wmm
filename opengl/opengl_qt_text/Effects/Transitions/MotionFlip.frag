uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
float curPos = PREFIX(current_pos);

const float PI = 3.141592653589793;

float GetParabolaMap(float t, float T)	
{
	return (3. * T * t * t - 2. * t * t * t) / (T * T);
}

float GetSinusoidalMap(float t, float T)	
{
	return t - T * sin(2. * PI * t / T) / (2. * PI);
}

float GetBothMap(float t, float T)
{
  return GetParabolaMap(GetSinusoidalMap(t,T), T);
}

vec4 tex2D(int texId, vec2 uv, float bx, float by) {
    if (uv.x < bx || abs(uv.y) < by || uv.x > (1.00-bx) || abs(uv.y) > (1.0 - by))
        {
            return vec4(0.0,0.0,0.0,1.0);
        }
    else
        {
            uv.x = (uv.x - bx) / (1.0 - bx);
            uv.y = (uv.y - by) / (1.0 - by);
			if (texId == 1)
				return texture2D(mSample0, uv);
			else
				return texture2D(mSample1, uv);
        }
}
vec4 BlurV (int texId, vec2 size, vec2 uv, float radius, float bx, float by) {

	if (radius >= 0.1)
	{
		vec4 C = vec4(0.0); 

		float height = 1.0 / size.y;

		float divisor = 0.0; 

       for (float y = -20.0; y <= 20.0; y++)
		{
			C += tex2D(texId, uv + vec2(0.0, y * height)*radius, bx, by);
			divisor++; 
		}

		return vec4(C.r / divisor, C.g / divisor, C.b / divisor, 1.0);
	}

	return tex2D(texId, uv, bx, by);
}

const float ZOOM_TIME =.1;
void main() 
{
	vec2 tc = textureCoord;
	vec2 uv = tc;
	float bx = 0.045;
	float by = 0.05;
	float proc = iGlobalTime;
	float speed = 0.;
	int texId = 1;
	if (proc > 0.5) {
		proc = 1.0 - proc;
		texId = 2;
	}
	if (proc <= ZOOM_TIME) {
		bx = proc / ZOOM_TIME * bx;
		by = proc / ZOOM_TIME * by;
	}
	else {
		speed = (proc-ZOOM_TIME*2.0) / (1.0-ZOOM_TIME*2.0);
		proc = (iGlobalTime - ZOOM_TIME) / (1.0 - ZOOM_TIME*2.0);
		uv.y -= mod(4.0*GetBothMap(proc,1.0), 1.0);
		if (uv.y < 0.0)
			uv.y = 1.0 + uv.y;
	}

	gl_FragColor = BlurV(texId, u_resolution.xy, uv, 7.0*speed, bx, by);

}
