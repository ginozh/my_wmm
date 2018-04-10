#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;


float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 FUNCNAME(vec2 tc)
{
  float revProgress = (1.0 - progress);
  float distFromEdges = min(progress, revProgress);
  float squareSize = (50.0 * distFromEdges) + 1.0;  
  
  vec2 p = (floor((gl_FragCoord.xy + squareSize * 0.5) / squareSize) * squareSize) / resolution.xy;
  vec4 fromColor = INPUT1(p);
  vec4 toColor = INPUT2(p);
  
  return mix(fromColor, toColor, progress);
}
