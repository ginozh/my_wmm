/*{
	"GUID":"F2BE4853-1C34-4b54-BF89-AFF160A1E054",
	"PARAMS":[
		{
			"NAME":"r",
			"TYPE":"float",
			"DEFAULT":0.0,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"g",
			"TYPE":"float",
			"DEFAULT":0.0,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"b",
			"TYPE":"float",
			"DEFAULT":0.0,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"blendThreshold",
			"TYPE":"int",
			"DEFAULT":5,
			"MIN":0,
			"MAX":100
		},
		{
			"NAME":"eraseThreshold",
			"TYPE":"int",
			"DEFAULT":5,
			"MIN":0,
			"MAX":100
		}
	]
}*/

vec4 rgb_to_xyz(vec4 color) {
	float var_R = (color.r); //R from 0.0 to 255.0
	float var_G = (color.g); //G from 0.0 to 255.0
	float var_B = (color.b); //B from 0.0 to 255.0

	if (var_R > 0.04045) {
		var_R = pow(((var_R + 0.055) / 1.055), 2.4);
	} else {
		var_R = var_R / 12.92;
	}
	if (var_G > 0.04045) {
		var_G = pow(((var_G + 0.055) / 1.055), 2.4);
	} else {
		var_G = var_G / 12.92;
	}

	if (var_B > 0.04045) {
		var_B = pow(((var_B + 0.055) / 1.055), 2.4);
	} else {
		var_B = var_B / 12.92;
	}

	var_R = var_R * 100.0;
	var_G = var_G * 100.0;
	var_B = var_B * 100.0;

	//Observer. = 2.0°, Illuminant = D65
	float X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
	float Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
	float Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

	return vec4(X, Y, Z, color.a);
}

vec4 xyz_to_lab(vec4 color) {

	float ref_X = 95.047; //Observer= 2.0°, Illuminant= D65
	float ref_Y = 100.000;
	float ref_Z = 108.883;

	float var_X = color.r / ref_X;
	float var_Y = color.g / ref_Y;
	float var_Z = color.b / ref_Z;

	if (var_X > 0.008856) {
		var_X = pow(var_X, (1.0 / 3.0));
	} else {
		var_X = (7.787 * var_X) + (16.0 / 116.0);
	}
	if (var_Y > 0.008856) {
		var_Y = pow(var_Y, (1.0 / 3.0));
	} else {
		var_Y = (7.787 * var_Y) + (16.0 / 116.0);
	}
	if (var_Z > 0.008856) {
		var_Z = pow(var_Z, (1.0 / 3.0));
	} else {
		var_Z = (7.787 * var_Z) + (16.0 / 116.0);
	}

	float L = (116.0 * var_Y) - 16.0;
	float a = 500.0 * (var_X - var_Y);
	float b = 200.0 * (var_Y - var_Z);

	return vec4(L, a, b, color.a);
}

vec4 lab_to_lch(vec4 color) {

	const float MPI = 3.14159265359;

	float var_H = atan(color.b, color.g); //in GLSL this is atan2

	if (var_H > 0.0) {
		var_H = (var_H / MPI) * 180.0;
	} else {
		var_H = 360.0 - (abs(var_H) / MPI) * 180.0;
	}

	float C = sqrt(pow(color.g, 2.0) + pow(color.b, 2.0));
	float H = var_H;

	return vec4(color.r, C, H, color.a);
}
// ---------------------------
vec4 lch_to_lab(vec4 color) {
	float a = cos(radians(color.b)) * color.g;
	float b = sin(radians(color.b)) * color.g;

	return vec4(color.r, a, b, color.a);
}

vec4 lab_to_xyz(vec4 color) {
	float var_Y = (color.r + 16.0) / 116.0;
	float var_X = color.g / 500.0 + var_Y;
	float var_Z = var_Y - color.b / 200.0;

	if (pow(var_Y, 3.0) > 0.008856) {
		var_Y = pow(var_Y, 3.0);
	} else {
		var_Y = (var_Y - 16.0 / 116.0) / 7.787;
	}
	if (pow(var_X, 3.0) > 0.008856) {
		var_X = pow(var_X, 3.0);
	} else {
		var_X = (var_X - 16.0 / 116.0) / 7.787;
	}
	if (pow(var_Z, 3.0) > 0.008856) {
		var_Z = pow(var_Z, 3.0);
	} else {
		var_Z = (var_Z - 16.0 / 116.0) / 7.787;
	}

	float ref_X = 95.047; //Observer= 2.0 degrees, Illuminant= D65
	float ref_Y = 100.000;
	float ref_Z = 108.883;

	float X = ref_X * var_X;
	float Y = ref_Y * var_Y;
	float Z = ref_Z * var_Z;

	return vec4(X, Y, Z, color.a);
}

vec4 xyz_to_rgb(vec4 color) {
	float var_X = color.r / 100.0; //X from 0.0 to  95.047      (Observer = 2.0 degrees, Illuminant = D65);
	float var_Y = color.g / 100.0; //Y from 0.0 to 100.000;
	float var_Z = color.b / 100.0; //Z from 0.0 to 108.883;

	float var_R = var_X * 3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
	float var_G = var_X * -0.9689 + var_Y * 1.8758 + var_Z * 0.0415;
	float var_B = var_X * 0.0557 + var_Y * -0.2040 + var_Z * 1.0570;

	if (var_R > 0.0031308) {
		var_R = 1.055 * pow(var_R, (1.0 / 2.4)) - 0.055;
	} else {
		var_R = 12.92 * var_R;
	}
	if (var_G > 0.0031308) {
		var_G = 1.055 * pow(var_G, (1.0 / 2.4)) - 0.055;
	} else {
		var_G = 12.92 * var_G;
	}
	if (var_B > 0.0031308) {
		var_B = 1.055 * pow(var_B, (1.0 / 2.4)) - 0.055;
	} else {
		var_B = 12.92 * var_B;
	}

	float R = var_R;
	float G = var_G;
	float B = var_B;

	return vec4(R, G, B, color.a);
}


vec4 rgb_to_lch(vec4 color) {
	vec4 xyz = rgb_to_xyz(color);
	vec4 lab = xyz_to_lab(xyz);
	vec4 lch = lab_to_lch(lab);
	return lch;
}


vec4 lch_to_rgb(vec4 color) {
	vec4 lab = lch_to_lab(color);
	vec4 xyz = lab_to_xyz(lab);
	vec4 rgb = xyz_to_rgb(xyz);
	return rgb;
}

vec4 rgb_to_lab(vec4 color) {
	vec4 xyz = rgb_to_xyz(color);
	vec4 lab = xyz_to_lab(xyz);
	return lab;
}

vec4 lab_to_rgb(vec4 color) {
	vec4 xyz = lab_to_xyz(color);
	vec4 rgb = xyz_to_rgb(xyz);
	return rgb;
}

float distanceThreeDimensions(vec4 lab1, vec4 lab2)
{
    return sqrt(pow((lab1.r - lab2.r),2.0) + pow((lab1.g - lab2.g),2.0) + pow((lab1.b - lab2.b),2.0));
}

float calcAlpha(vec4 lab1, vec4 lab2)
{
	float blendThreshold = float(PREFIX(blendThreshold));
    float eraseThreshold = float(PREFIX(eraseThreshold));
    blendThreshold += eraseThreshold;
    float difThreshold = blendThreshold - eraseThreshold;
	
	float cDistance = distanceThreeDimensions(lab1, lab2);
    float alpha = 1.0;
    if (cDistance < eraseThreshold)
    {
        alpha = 0.0;
    }
    else if (cDistance < blendThreshold)
    {
        cDistance = ((blendThreshold - cDistance) / difThreshold);
        cDistance = cDistance * cDistance;
        alpha = 1.0 - cDistance;
    }
	return alpha;
}

vec4 FUNCNAME(vec2 tc)
{
	vec4 col = INPUT(tc);
    vec4 lab = rgb_to_lab(col);
    
    vec4 blk = vec4(vec3(PREFIX(r),PREFIX(g),PREFIX(b)),1.0);
    vec4 black_lab = rgb_to_lab(blk);
    
    col.a = col.a * calcAlpha(lab, black_lab);
	return col;	
}