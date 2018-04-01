#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

float flashPhase = 0.8; 
float flashIntensity = 1.0;
float flashZoomEffect = 0.5;
 
const vec3 flashColor = vec3(0.8, 0.8, 0.3);
const float flashVelocity = 3.0;

vec4 FUNCNAME(vec2 tc)
{
  vec2 p = gl_FragCoord.xy / resolution.xy;
  //vec4 fc = INPUT1(p);
  //vec4 tc = INPUT2(p);
  float intensity = mix(1.0, 2.0*distance(p, vec2(0.5, 0.5)), flashZoomEffect) * flashIntensity * pow(smoothstep(flashPhase, 0.0, distance(0.5, progress)), flashVelocity);
  vec4 c = mix(INPUT1(p), INPUT2(p), smoothstep(0.5*(1.0-flashPhase), 0.5*(1.0+flashPhase), progress));
  c += intensity * vec4(flashColor, 1.0);
  gl_FragColor = c;
  return gl_FragColor;
}
