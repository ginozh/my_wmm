/*{
	"PARAMS":[
		{
			"NAME":"emboss",
			"TYPE":"float",
			"DEFAULT":0.5,
			"MIN":0.1,
			"MAX":1.0
		},
		{
			"NAME":"steps",
			"TYPE":"int",
			"DEFAULT":8,
			"MIN":1,
			"MAX":10
		}
	]   	
}*/

uniform sampler2D mSample0;
varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform float u_emboss;
uniform int u_steps;

float u_time = u_global_time;
        	
const float PI = 3.1415926535897932;

// play with these parameters to custimize the effect
// ===================================================

//speed
const float speed = 0.2;
const float speed_x = 0.3;
const float speed_y = 0.3;

float time = u_time * 1.3;
float time_speed = time * speed;
float time_speed_x = time * speed_x;
float time_speed_y = time * speed_y;

// refraction
float emboss = u_emboss;
const float intensity = 2.4;
const float frequency = 6.0;
const int angle = 7; // better when a prime
const float delta_theta = 0.8975979010256551; // 2.0 * PI / float(angle);

int steps = u_steps;

// reflection
const float delta = 60.;
const float intence = 700.;

const float reflectionCutOff = 0.012;
const float reflectionIntence = 200000.;

// ===================================================

  float col(vec2 coord)
  {
    float col = 0.0;
    float theta = 0.0;
    for (int i = 0; i < steps; i++)
    {
      vec2 adjc = coord;
      theta = delta_theta*float(i);
      adjc.x += cos(theta)*time_speed + time_speed_x;
      adjc.y -= sin(theta)*time_speed - time_speed_y;
      col = col + cos( (adjc.x*cos(theta) - adjc.y*sin(theta))*frequency)*intensity;
    }

    return cos(col);
  }

//---------- main

void main() 
{
	vec2 p = textureCoord, c1 = p, c2 = p;
	float cc1 = col(c1);

	c2.x += u_resolution.x/delta;
	float dx = emboss*(cc1-col(c2))/delta;

	c2.x = p.x;
	c2.y += u_resolution.y/delta;
	float dy = emboss*(cc1-col(c2))/delta;

	c1.x += dx*2.;
	c1.y = c1.y+dy*2.;

	float alpha = 1.+dot(dx,dy)*intence;
		
	float ddx = dx - reflectionCutOff;
	float ddy = dy - reflectionCutOff;
	if (ddx > 0. && ddy > 0.)
		alpha = pow(alpha, ddx*ddy*reflectionIntence);
		
	gl_FragColor = texture2D(mSample0, c1)*(alpha);
}

