# GLSL Conversion notes

## pixel position

----------------------

##### glsl.heroku

	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;

becomes:

    vec2 p = vertTexCoord.xy;


##### shadertoy

	vec2 uv = gl_FragCoord.xy/iResolution.xy;

becomes:
	
	vec2 uv = vertTexCoord.xy;

or:
	
    vec2 p = (-iResolution.xy + 2.0*fragCoord.xy)/iResolution.y;

becomes:

	vec2 p = -vertTexCoord.xy + 0.5;
	
	
## center

--------------------

	vec2 uv = gl_FragCoord.xy;
	
becomes:
	
	vec2 uv = vertTexCoord.xy - vec2(.5,.5);

## resolution

--------------------

##### shadertoy + glsl.heroku
	iResolution
	
becomes:

	uniform vec2 texOffset
	
	
	
	
### Shaders to convert:

* http://glsl.heroku.com/e#1419.0
* http://glsl.heroku.com/e#1329.0
* http://glsl.heroku.com/e#1200.0
* http://glsl.heroku.com/e#1591.3
* http://glsl.heroku.com/e#1692.0
* http://glsl.heroku.com/e#2165.0
* http://glsl.heroku.com/e#2234.1
* http://glsl.heroku.com/e#3005.1
* http://glsl.heroku.com/e#3849.0
* http://glsl.heroku.com/e#4153.0
* http://glsl.heroku.com/e#4154.0
* http://glsl.heroku.com/e#4201.0
* http://glsl.heroku.com/e#4196.0
* http://glsl.heroku.com/e#4243.0
* http://glsl.heroku.com/e#4242.1
* http://glsl.heroku.com/e#4294.0
* http://glsl.heroku.com/e#4346.0
* http://glsl.heroku.com/e#4460.0
* http://glsl.heroku.com/e#4646.0
* http://glsl.heroku.com/e#4722.2
* http://glsl.heroku.com/e#5254.1
* http://glsl.heroku.com/e#5267.0
* http://glsl.heroku.com/e#5246.0
* http://glsl.heroku.com/e#5321.0
* http://glsl.heroku.com/e#5398.8
* http://glsl.heroku.com/e#5514.0
* http://glsl.heroku.com/e#5359.8
* http://glsl.heroku.com/e#5485.0
* http://glsl.heroku.com/e#5611.0
* http://glsl.heroku.com/e#5670.0
* http://glsl.heroku.com/e#5664.1
* http://glsl.heroku.com/e#5688.0
* http://glsl.heroku.com/e#5654.17
* http://glsl.heroku.com/e#5785.8
* http://glsl.heroku.com/e#5985.0
* http://glsl.heroku.com/e#6029.0
* http://glsl.heroku.com/e#6042.5
* http://glsl.heroku.com/e#6098.23
* http://glsl.heroku.com/e#6304.0
* http://glsl.heroku.com/e#6428.0
* http://glsl.heroku.com/e#6416.0
* http://glsl.heroku.com/e#6450.4
* http://glsl.heroku.com/e#6550.0
* http://glsl.heroku.com/e#6726.3
* http://glsl.heroku.com/e#6808.0
* http://glsl.heroku.com/e#6810.0
* http://glsl.heroku.com/e#6786.0
* http://glsl.heroku.com/e#6939.0
* http://glsl.heroku.com/e#6972.1
* http://glsl.heroku.com/e#7016.4
* http://glsl.heroku.com/e#7022.0
* http://glsl.heroku.com/e#7055.0
* http://glsl.heroku.com/e#7083.10
* http://glsl.heroku.com/e#7315.1
* http://glsl.heroku.com/e#7070.6
* http://glsl.heroku.com/e#7403.0
* http://glsl.heroku.com/e#7405.0
* http://glsl.heroku.com/e#7453.12
* http://glsl.heroku.com/e#7520.0
* http://glsl.heroku.com/e#7597.0
* http://glsl.heroku.com/e#7600.1
* http://glsl.heroku.com/e#7716.0
* http://glsl.heroku.com/e#7734.0
* http://glsl.heroku.com/e#7757.5
* http://glsl.heroku.com/e#8034.0
* http://glsl.heroku.com/e#7988.0
* http://glsl.heroku.com/e#8010.0
* 