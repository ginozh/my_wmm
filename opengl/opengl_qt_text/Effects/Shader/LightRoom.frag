uniform sampler2D mSample0;
varying vec2 textureCoord;
uniform vec2 u_resolution;

uniform float u_angle;
uniform int u_hflip;
uniform int u_vflip;
uniform float u_transparence;
// Temperature parameter
uniform int u_temperature;
// Tint parameter
uniform float u_tint;
// Exposure parameter
uniform float u_exposure;
// Brightness parameter
uniform float u_brightness;
// Contrast parameter
uniform float u_contrast;
// Vibrance parameter
uniform float u_vib;
// Saturation parameter
uniform float u_sat;
uniform float u_stabilization;

uniform float u_chroma_transparence;
uniform float u_chroma_r;
uniform float u_chroma_g;
uniform float u_chroma_b;
uniform float u_chroma_similarity;
#if 0
// HighLight parameter
uniform float u_highLight;
// Shadow parameter
uniform float u_shadow;
// HDR white&black level parameter
uniform float u_whiteLevel;
uniform float u_blackLevel;

// HSL parameter
uniform float u_hsl_red_sat;
uniform float u_hsl_red_hue;
uniform float u_hsl_red_brightness;
uniform float u_hsl_orange_sat;
uniform float u_hsl_orange_hue;
uniform float u_hsl_orange_brightness;
uniform float u_hsl_yellow_sat;
uniform float u_hsl_yellow_hue;
uniform float u_hsl_yellow_brightness;
uniform float u_hsl_green_sat;
uniform float u_hsl_green_hue;
uniform float u_hsl_green_brightness;
uniform float u_hsl_magenta_sat;
uniform float u_hsl_magenta_hue;
uniform float u_hsl_magenta_brightness;
uniform float u_hsl_purple_sat;
uniform float u_hsl_purple_hue;
uniform float u_hsl_purple_brightness;
uniform float u_hsl_blue_sat;
uniform float u_hsl_blue_hue;
uniform float u_hsl_blue_brightness;
uniform float u_hsl_aqua_sat;
uniform float u_hsl_aqua_hue;
uniform float u_hsl_aqua_brightness;

// Vignette parameter
uniform float u_vignette_amount;
uniform float u_vignette_feather;
uniform float u_vignette_highlights;
uniform float u_vignette_size;
uniform float u_vignette_roundness;
uniform float u_vignette_exposure;
#endif

const float PI = 3.141592653589793;
//---------------------------------------------------------------------------------------//
// Temperature functions
//---------------------------------------------------------------------------------------//
float saturate(float v) { return clamp(v, 0.0, 1.0); }
vec2  saturate(vec2  v) { return clamp(v, vec2(0.0), vec2(1.0)); }
vec3  saturate(vec3  v) { return clamp(v, vec3(0.0), vec3(1.0)); }
vec4  saturate(vec4  v) { return clamp(v, vec4(0.0), vec4(1.0)); }

//---------------------------------------------------------------------------------------//
vec3 temperatureToRGB(float temperatureInKelvins)
{
	vec3 retColor;

	temperatureInKelvins = clamp(temperatureInKelvins, 1000.0, 40000.0) / 100.0;

	if (temperatureInKelvins <= 66.0)
	{
		retColor.r = 1.0;
		retColor.g = saturate(0.39008157876901960784 * log(temperatureInKelvins) - 0.63184144378862745098);
	}
	else
	{
		float t = temperatureInKelvins - 60.0;
		retColor.r = saturate(1.29293618606274509804 * pow(t, -0.1332047592));
		retColor.g = saturate(1.12989086089529411765 * pow(t, -0.0755148492));
	}

	if (temperatureInKelvins >= 66.0)
		retColor.b = 1.0;
	else if (temperatureInKelvins <= 19.0)
		retColor.b = 0.0;
	else
		retColor.b = saturate(0.54320678911019607843 * log(temperatureInKelvins - 10.0) - 1.19625408914);

	return retColor;
}

//---------------------------------------------------------------------------------------//
vec3 HSLtoRGB(in vec3 HSL)
{
	float R = abs(HSL.x * 6.0 - 3.0) - 1.0;
	float G = 2.0 - abs(HSL.x * 6.0 - 2.0);
	float B = 2.0 - abs(HSL.x * 6.0 - 4.0);
	vec3 RGB = saturate(vec3(R, G, B));
	float C = (1.0 - abs(2.0 * HSL.z - 1.0)) * HSL.y;

	return (RGB - 0.5) * C + vec3(HSL.z);
}

//---------------------------------------------------------------------------------------//
vec3 RGBtoHSL(vec3 RGB)
{
	vec4 P = (RGB.g < RGB.b) ? vec4(RGB.bg, -1.0, 2.0 / 3.0) : vec4(RGB.gb, 0.0, -1.0 / 3.0);
	vec4 Q = (RGB.r < P.x) ? vec4(P.xyw, RGB.r) : vec4(RGB.r, P.yzx);
	float C = Q.x - min(Q.w, Q.y);
	float H = abs((Q.w - Q.y) / (6.0 * C + 1e-10) + Q.z);
	vec3 HCV = vec3(H, C, Q.x);
	float L = HCV.z - HCV.y * 0.5;
	float S = HCV.y / (1.0 - abs(L * 2.0 - 1.0) + 1e-10);

	return vec3(HCV.x, S, L);
}

//---------------------------------------------------------------------------------------//
float calTemperatureRadio(float temp)
{
	float ratio = 1.0 - (temp) / 11500.0;
	ratio = abs(ratio) * 1.7;

	return ratio;
}

//---------------------------------------------------------------------------------------//
float luminance(vec3 color)
{
	float fmin = min(min(color.r, color.g), color.b);
	float fmax = max(max(color.r, color.g), color.b);

	return (fmin + fmax) / 2.0;
}

//---------------------------------------------------------------------------------------//
vec3 mixColorAndTmpColor(vec3 orignalColor, vec3 temperateColor, float factor)
{
	vec3 ret;
	float ratio = factor;
	float orignalRatio = 1.0 - ratio;

	ret.r = clamp((orignalColor.r * orignalRatio + temperateColor.r * ratio), 0.0, 1.0);
	ret.g = clamp((orignalColor.g * orignalRatio + temperateColor.g * ratio), 0.0, 1.0);
	ret.b = clamp((orignalColor.b * orignalRatio + temperateColor.b * ratio), 0.0, 1.0);

	return ret;
}

//---------------------------------------------------------------------------------------//
// Tint functions
//---------------------------------------------------------------------------------------//
vec3 calNewTintPixel(vec3 color, float r, float g, float b)
{
	float gray = color.r * 0.3 + color.g * 0.59 + color.b * 0.11;
	float rr = r * gray + color.r;
	float gg = g * gray + color.g;
	float bb = b * gray + color.b;

	vec3 ret = vec3(rr, gg, bb);

	return ret;
}

//---------------------------------------------------------------------------------------//
// Exposure functions
//---------------------------------------------------------------------------------------//
vec4 calPowColor(vec4 color, float exposure)
{
	vec4 ret = color * exposure;
	ret.a = color.a;
	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
// Brightness functions
//---------------------------------------------------------------------------------------//
float calBrightValue(float bright)
{
	float brightValue = 0.0;

	if (bright > 0.0)
	{
		brightValue = 1.0 + bright / 100.0;
	}
	else
	{
		brightValue = 1.0 - 1.0 / (0.99 + bright / 253.0);
		brightValue *= 0.6;
	}

	return brightValue;
}

//---------------------------------------------------------------------------------------//
vec4 newBrightness(vec4 color, float bright)
{
	vec4 ret;

	if (bright > 0.0)
		ret = color * bright;
	else
		ret = color + bright;

	ret.a = color.a;
	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
// Contrast functions
//---------------------------------------------------------------------------------------//
vec3 calContrastValue(float contrast)
{
	float contrastValue = 0.0;
	int contrastVal = 0;
	int nHigh = 0;
	int nStretch = 0;
	vec3 ret;

	if (contrast > 0.0)
		contrastValue = 1.0 / (1.0 - contrast / 255.0) - 1.0;
	else
		contrastValue = contrast / 255.0;

    contrastVal = int((contrastValue * 100.0) / 2.0);
	nHigh = 255 - contrastVal;

	if (nHigh < contrastVal)
	{
		nHigh = 127;
		contrastVal = 120;
	}
	if (contrastVal < -127)
		contrastVal = -120;

	if (contrastVal >= 0)
		nStretch = 255 - 2 * contrastVal;
	else
		nStretch = 255 + 2 * contrastVal;

    ret.x = float(contrastVal) / 255.0;
    ret.y = float(nHigh) / 255.0;
    ret.z = float(nStretch) / 255.0;

	return ret;
}

//---------------------------------------------------------------------------------------//
vec4 calContrastColor(vec4 color, float contrastVal, float nHigh, float nStretch)
{
	vec4 ret;

    if (contrastVal > 0.0)
	{
		if (color.r <= contrastVal)
			ret.r = 0.0;
		else if (color.r > nHigh)
			ret.r = 1.0;
		else
			ret.r = (color.r - contrastVal) / nStretch;

		if (color.g <= contrastVal)
			ret.g = 0.0;
		else if (color.g > nHigh)
			ret.g = 1.0;
		else
			ret.g = (color.g - contrastVal) / nStretch;

		if (color.b <= contrastVal)
			ret.b = 0.0;
		else if (color.b > nHigh)
			ret.b = 1.0;
		else
			ret.b = (color.b - contrastVal) / nStretch;
	}
	else
	{
		ret.r = (color.r * nStretch) - contrastVal;
		ret.g = (color.g * nStretch) - contrastVal;
		ret.b = (color.b * nStretch) - contrastVal;
	}

	ret.a = color.a;
	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
// Vibrance functions
//---------------------------------------------------------------------------------------//
vec4 mixLumaAndColor(vec4 color, float lumaValue, float lumaMask, float vibrance)
{
	vec4 ret;

	float radio = 1.0 + vibrance * lumaMask;
	float repersed = 1.0 - radio;

	ret = color * radio + lumaValue * repersed;
	ret = clamp(ret, vec4(0.0), vec4(1.0));
	ret.a = color.a;

	return ret;
}

//---------------------------------------------------------------------------------------//
// Saturation functions
//---------------------------------------------------------------------------------------//
vec4 calNewColor(vec4 color, float saturation)
{
	vec4 ret;

	float average = (color.r + color.g + color.b) / 3.0;
	float mult = 1.0 - 1.0 / (1.5 - saturation);
	if (saturation > 0.0)
	{
		ret.r = color.r + (average - color.r) * mult;
		ret.g = color.g + (average - color.g) * mult;
		ret.b = color.b + (average - color.b) * mult;
	}
	else
	{
		ret.r = color.r + (average - color.r) * (-saturation);
		ret.g = color.g + (average - color.g) * (-saturation);
		ret.b = color.b + (average - color.b) * (-saturation);
	}

	ret.a = color.a;
	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
// HighLight & Shadow functions
//---------------------------------------------------------------------------------------//
float enHanceColor(float color, float coff)
{
	float adjust = coff * color;
	float val = 1.0 - (1.0 - adjust) * (1.0 - color);

	return val;
}

//---------------------------------------------------------------------------------------//
vec4 calHighlight(vec4 color, float highLight)
{
	vec4 ret;
	float lumaince = highLight * (max(color.r, max(color.g, color.b)));

	ret.r = enHanceColor(color.r, lumaince);
	ret.g = enHanceColor(color.g, lumaince);
	ret.b = enHanceColor(color.b, lumaince);
	ret.a = color.a;

	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
vec4 calShadow(vec4 color, float shadow)
{
	vec4 ret;
	float lumaince = shadow * (1.0 - max(color.r, max(color.g, color.b)));

	ret.r = enHanceColor(color.r, lumaince);
	ret.g = enHanceColor(color.g, lumaince);
	ret.b = enHanceColor(color.b, lumaince);
	ret.a = color.a;

	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
// HDRWhiteLevel & HDRBlackLevel functions
//---------------------------------------------------------------------------------------//
vec4 calWhiteLevelPixel(vec4 color, float level)
{
	vec4 ret;
	float lumaince = level * (max(color.r, max(color.g, color.b)));

	ret.r = enHanceColor(color.r, lumaince);
	ret.g = enHanceColor(color.g, lumaince);
	ret.b = enHanceColor(color.b, lumaince);

	if (lumaince > 0.0)
	{
		ret.r = enHanceColor(ret.r, lumaince);
		ret.g = enHanceColor(ret.g, lumaince);
		ret.b = enHanceColor(ret.b, lumaince);
	}

	ret.a = color.a;

	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
vec4 calBlackLevelPixel(vec4 color, float level)
{
	vec4 ret;
	float lumaince = level * (1.0 - max(color.r, max(color.g, color.b)));

	ret.r = enHanceColor(color.r, lumaince);
	ret.g = enHanceColor(color.g, lumaince);
	ret.b = enHanceColor(color.b, lumaince);
	ret.a = color.a;

	ret = clamp(ret, vec4(0.0), vec4(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
// HSL functions
//---------------------------------------------------------------------------------------//
vec3 HSL_RGBtoHSL(vec3 RGB)
{
	vec3 hsl;
	float R = RGB.r;
	float G = RGB.g;
	float B = RGB.b;
	float Max = 0.0;
	float Min = 0.0;
	float H = 0.0;
	float S = 0.0;
	float L = 0.0;

	Min = min(R, min(G, B));
	Max = max(R, max(G, B));

	if (Min == Max)
	{
		H = 2.0 / 3.0;
		S = 0.0;
		L = R;
	}
	else
	{
		L = (Max + Min) / 2.0;

		if (L < 0.5)
			S = (Max - Min) / (Max + Min);
		else
			S = (Max - Min) / (2.0 - Max - Min);

		if (R == Max)
			H = (G - B) / (Max - Min);
		else if (G == Max)
			H = 2.0 + (B - R) / (Max - Min);
		else
			H = 4.0 + (R - G) / (Max - Min);

		H /= 6.0;
		if (H < 0.0)
			H += 1.0;
	}

	hsl.x = H * 360.0;
	hsl.y = S;
	hsl.z = L;

	return hsl;
}

//---------------------------------------------------------------------------------------//
vec3 HSL_HSLtoRGB(vec3 HSL)
{
	vec3 RGB;
	float H = HSL.x / 360.0;
	float S = HSL.y;
	float L = HSL.z;
	float R = 0.0;
	float G = 0.0;
	float B = 0.0;
	float var_1 = 0.0;
	float var_2 = 0.0;
	float tempr = 0.0;
	float tempg = 0.0;
	float tempb = 0.0;

	if (S == 0.0)
	{
		R = G = B = L;
	}
	else
	{
		if (L < 0.5)
			var_2 = L * (1.0 + S);
		else
			var_2 = (L + S) - (L * S);

		var_1 = 2.0 * L - var_2;

		tempr = H + 1.0 / 3.0;
		if (tempr > 1.0)
			tempr--;

		tempg = H;

		tempb = H - 1.0 / 3.0;
		if (tempb < 0.0)
			tempb++;

		// Red
		if (tempr < 1.0 / 6.0)
			R = var_1 + (var_2 - var_1) * 6.0 * tempr;
		else if (tempr < 0.5)
			R = var_2;
		else if (tempr < 2.0 / 3.0)
			R = var_1 + (var_2 - var_1) * ((2.0 / 3.0) - tempr) * 6.0;
		else
			R = var_1;
		// Green
		if (tempg < 1.0 / 6.0)
			G = var_1 + (var_2 - var_1) * 6.0 * tempg;
		else if (tempg < 0.5)
			G = var_2;
		else if (tempg < 2.0 / 3.0)
			G = var_1 + (var_2 - var_1) * ((2.0 / 3.0) - tempg) * 6.0;
		else
			G = var_1;
		// Blue
		if (tempb < 1.0 / 6.0)
			B = var_1 + (var_2 - var_1) * 6.0 * tempb;
		else if (tempb < 0.5)
			B = var_2;
		else if (tempb < 2.0 / 3.0)
			B = var_1 + (var_2 - var_1) * ((2.0 / 3.0) - tempb) * 6.0;
		else
			B = var_1;
	}

	RGB.r = R;
	RGB.g = G;
	RGB.b = B;

	return RGB;
}

//---------------------------------------------------------------------------------------//
float calTransperency(vec3 hsl_Val, float minDegreeVal, float maxDegreeVal)
{
	float adjustRadio = 0.0;

	if (minDegreeVal > maxDegreeVal)
	{
		if (hsl_Val.x >= minDegreeVal)
		{
			adjustRadio = (hsl_Val.x - minDegreeVal) / (360.0 - minDegreeVal);
		}
		else if (hsl_Val.x <= maxDegreeVal)
		{
			adjustRadio = 1.0 - hsl_Val.x / maxDegreeVal;
		}
		else
		{
			adjustRadio = 0.0;
		}
	}
	else
	{
		if (hsl_Val.x > minDegreeVal && hsl_Val.x < maxDegreeVal)
		{
			float dist = (hsl_Val.x - minDegreeVal);
			dist = 2.0 * dist / (maxDegreeVal - minDegreeVal);
			if (dist <= 1.0)
				adjustRadio = dist;
			else
				adjustRadio = 2.0 - dist;
		}
		else
		{
			adjustRadio = 0.0;
		}
	}

	return adjustRadio;
}

//---------------------------------------------------------------------------------------//
vec3 doAdjustHue(vec3 origColor, vec3 hsl_Val, float alphaVal, float hueVal)
{
	float hueValue = hueVal;
	if (hueValue > 0.0)
		hueValue *= 1.2;

	hsl_Val.x += hueValue;

	if (hsl_Val.x < 0.0)
		hsl_Val.x += 360.0;

	if (hsl_Val.x > 360.0)
		hsl_Val.x -= 360.0;

	vec3 color = HSL_HSLtoRGB(hsl_Val);

	float factor = 1.0 - alphaVal;

	vec3 ret = origColor * factor + color * alphaVal;
	ret = clamp(ret, vec3(0.0), vec3(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
vec3 doAdjustStaturation(vec3 origColor, vec3 hsl_Val, float alphaVal, float satRadio)
{
	hsl_Val.y *= (1.0 + satRadio);
	hsl_Val.y = clamp(hsl_Val.y, 0.0, 1.0);
	vec3 color = HSL_HSLtoRGB(hsl_Val);

	float factor = 1.0 - alphaVal;

	vec3 ret = origColor * factor + color * alphaVal;
	ret = clamp(ret, vec3(0.0), vec3(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
vec3 doAdjustBrightness(vec3 origColor, vec3 hsl_Val, float alphaVal, float brightnessRadio)
{
	hsl_Val.z *= (1.0 + brightnessRadio / 8.0);
	hsl_Val.z = clamp(hsl_Val.z, 0.0, 1.0);
	vec3 color = HSL_HSLtoRGB(hsl_Val);

	float factor = 1.0 - alphaVal;

	vec3 ret = origColor * factor + color * alphaVal;
	ret = clamp(ret, vec3(0.0), vec3(1.0));

	return ret;
}

//---------------------------------------------------------------------------------------//
// Vignette functions
//---------------------------------------------------------------------------------------//
float highLightColor(float color, float coff)
{
	float adjust = coff * color;
	float val = 1.0 - (1.0 - adjust) * (1.0 - color);

	return val;
}

//---------------------------------------------------------------------------------------//
float generateGradient(vec2 pos, vec2 center, float featherRatio, float sizeRatio, float roundNess)
{
	float maskVal = 1.0;
	float a = 0.2 * center.x * (1.0 + 4.0 * sizeRatio);
	float b = 0.2 * center.y * (1.0 + 4.0 * sizeRatio);
	float diff = abs(a - b) * roundNess;

	if (diff >= 0.0)
	{
		if (a > b)
			a -= diff;
		else if (a == b)
		{
			a = a;
			b = b;
		}
		else
			b -= diff;
	}

	float bandPixel = featherRatio * ((a > b ? a : b) / 2.0) + 3.0;
	float arguFactor = float(3.14159265358979323846) / bandPixel;

	float sa = a;
	float sb = b;
	float ea = a + bandPixel;
	float eb = b + bandPixel;

	float dx = abs(pos.x - center.x);
	float dy = abs(pos.y - center.y);
	float factor1 = dx / sa;
	float factor2 = dy / sb;
	float factor3 = dx / ea;
	float factor4 = dy / eb;
	float dist1 = factor1 * factor1 + factor2 * factor2 - 1.0;
	float dist2 = factor3 * factor3 + factor4 * factor4 - 1.0;

	if (dist1 <= 0.0)
	{
		maskVal = 1.0;
	}
	else if (dist2 >= 0.0)
	{
		maskVal = 0.0;
	}
	else
	{
		float k = dy / (dx + 0.000001);

		k *= k;
		float temp = k / (eb * eb) + 1.0 / (ea * ea);
		float xx = 1.0 / temp;
		float yy = k * xx;
		float dist = sqrt(xx + yy) - distance(pos, center);
		dist = bandPixel - dist;

		temp = arguFactor * dist;
		maskVal = 0.5 * (1.0 + cos(temp));
	}

	return maskVal;
}

//---------------------------------------------------------------------------------------//
vec4 vignettePixel(vec4 color, float maskValue, float amountVal, float exposureVal, float highlights)
{
	vec4 ret;

	float R = color.r;
	float G = color.g;
	float B = color.b;

	float outR = (1.0 + amountVal) * R;
	float outG = (1.0 + amountVal) * G;
	float outB = (1.0 + amountVal) * B;

	float factor1 = maskValue * exposureVal;
	float factor2 = maskValue * 2.0;
	factor2 = factor2 - 1.0;
	factor2 = 0.5 * (1.0 - factor2);

	R = float(R * factor1 + outR * factor2);
	G = float(G * factor1 + outG * factor2);
	B = float(B * factor1 + outB * factor2);

	if (maskValue < 1.0)
	{
		float factor = 1.0 - maskValue;
		factor = pow(factor, 2.0);
		float lumaince = factor * highlights * (1.0 - (R + G + B) / 3.0);
		R = highLightColor(R, lumaince);
		G = highLightColor(G, lumaince);
		B = highLightColor(B, lumaince);
	}

	ret.r = clamp(R, 0.0, 1.0);
	ret.g = clamp(G, 0.0, 1.0);
	ret.b = clamp(B, 0.0, 1.0);
	ret.a = color.a;

	return ret;
}

vec2 rotate_point(float cx,float cy,float angle,vec2 p)
{
  float s = sin(angle);
  float c = cos(angle);

  // translate point back to origin:
  p.x -= cx;
  p.y -= cy;

  // rotate point
  float xnew = p.x * c - p.y * s;
  float ynew = p.x * s + p.y * c;

  // translate point back:
  p.x = xnew + cx;
  p.y = ynew + cy;
  return p;
}

vec3 rgbToLab(vec3 inColor)
{
	float x;
	float y;
	float z;
	float l;
	float a;
	float b;

	//RGB to XYZ color
	if(inColor.r > 0.04045)
		x = pow(((inColor.r + 0.055) / 1.055), 2.4);
	else
		x = inColor.r / 12.92;

	if(inColor.g > 0.04045)
		y = pow(((inColor.g + 0.055) / 1.055), 2.4);
	else
		y = inColor.g / 12.92;

    if(inColor.b > 0.04045)
		z = pow(((inColor.b + 0.055) / 1.055), 2.4);
	else
		z = inColor.b / 12.92;

	//for final RGB to XYZ, multiply the previous x, y, and z by 100,
	//but since we will divide the final values by 95.047, 100, and
	//108.883, for optimizations I have combined that all below
	l = (x * 0.4124 + y * 0.3576 + z * 0.1805) / 0.95047;
	a = (x * 0.2126 + y * 0.7152 + z * 0.0722) / 1.00000;
	b = (x * 0.0193 + y * 0.1192 + z * 0.9505) / 1.08883;

	if(l > 0.008856)
		l = pow(l, (1.0 / 3.0));
	else
		l = (7.787 * l) + (16.0 / 116.0);

	if(a > 0.008856)
		a = pow(a, (1.0 / 3.0));
	else
		a = (7.787 * a) + (16.0 / 116.0);

    if(b > 0.008856)
		b = pow(b, (1.0 / 3.0));
	else
		b = (7.787 * b) + (16.0 / 116.0);

	//return L*, a*, b*
	return vec3((116.0 * a) - 16.0, 500.0 * (l - a), 200.0 * (a - b));
}

//CIE94
float deltaE(vec3 labA, vec3 labB)
{
	float deltaL = labA.x - labB.x;
	float deltaA = labA.y - labB.y;
	float deltaB = labA.z - labB.z;
	float c1 = sqrt(labA.y * labA.y + labA.z * labA.z);
	float c2 = sqrt(labB.y * labB.y + labB.z * labB.z);
	float deltaC = c1 - c2;
	float deltaH = deltaA * deltaA + deltaB * deltaB - deltaC * deltaC;
	if(deltaH < 0.0)
		deltaH = 0.0;
	else
		deltaH = sqrt(deltaH);
	float sc = 1.0 + 0.045 * c1;
	float sh = 1.0 + 0.015 * c1;
	float deltaLKlsl = deltaL / 1.0;
	float deltaCkcsc = deltaC / sc;
	float deltaHkhsh = deltaH / sh;
	float deltaE = deltaLKlsl * deltaLKlsl + deltaCkcsc * deltaCkcsc + deltaHkhsh * deltaHkhsh;
	if(deltaE < 0.0)
		deltaE = 0.0;
	else
		deltaE = sqrt(deltaE);
	return deltaE;
}

void main( )
{
    vec2 uv = textureCoord;
	vec4 color = texture2D(mSample0, uv);
	vec4 retColor = color;

    if (u_angle != 0.0)
    {
        float rotateDeta = PI/180.0*(-u_angle);
        uv=rotate_point(0.5, 0.5, rotateDeta, uv);
        retColor = texture2D(mSample0, uv);
    }

    if (u_hflip != 0)
    {
        uv.x = 1.0 - uv.x;
        retColor = texture2D(mSample0, uv);
    }

    if (u_vflip != 0)
    {
        uv.y = 1.0 - uv.y;
        retColor = texture2D(mSample0, uv);
    }
/*
	if (u_stabilization != 1.0) 
	{
        float width = u_resolution.x;
        float height = u_resolution.y;

        //color=retColor;
        color=vec4(0,0,0,0);

        float dToImgScale = u_stabilization;
        float rectToImg_width = width * dToImgScale;
        float rectToImg_height = height * dToImgScale;
        if (abs(gl_FragCoord.y - height / 2.) < rectToImg_height / 2. &&
                abs(gl_FragCoord.x - width / 2.) < rectToImg_width / 2.) 
        {
            //float ToI = (gl_FragCoord.x - width / 2.) / (dToImgScale) + width / 2.;
            float ToI = gl_FragCoord.x/(2.0*dToImgScale) + width/2.0 - width/(2.0*dToImgScale);
            //float ToJ = (gl_FragCoord.y - height / 2.) / dToImgScale + height / 2.;
            float ToJ = gl_FragCoord.y/(2.0*dToImgScale) + height/2.0 - height/(2.0*dToImgScale);
            color = texture2D(mSample0, vec2(ToI / width, ToJ / height));
        }
        retColor = color;
	}
*/
    if (u_transparence != 1.0)
    {
        retColor.a = u_transparence;
    }


	if (u_temperature != 11500) // temperature
	{
		float realTemperate = 0.0;
		if (u_temperature < 11500)
			realTemperate = float(26000 - u_temperature);
		else
		{
			realTemperate = float(23000 - u_temperature - 4900);
			if (realTemperate < 1700.0)
				realTemperate = 1700.0;
		}

		float ratio = calTemperatureRadio(realTemperate);
		vec3 tmkColor = temperatureToRGB(realTemperate);

		vec4 color = retColor;
		float mluminance = luminance(color.rgb);
		vec3 mulColor = color.rgb * tmkColor;
		vec3 blended = mixColorAndTmpColor(color.rgb, mulColor, ratio);
		vec3 hsl = RGBtoHSL(blended);
		hsl.b = mluminance;
		vec3 luminancePreservedRGB = HSLtoRGB(hsl);

		retColor.rgb = mixColorAndTmpColor(blended, luminancePreservedRGB, 1.0);
		retColor.a = color.a;
	}

	if (u_tint != 0.0) // tint
	{
		float val = (u_tint * 100.0) / 225.0;
		val = (val / 255.0);
		vec4 color = retColor;

		retColor.rgb = calNewTintPixel(color.rgb, val, -val, val);
		retColor.a = color.a;
	}

	if (u_exposure != 0.0) // exposure
	{
		float exposureValue = u_exposure / 50.0;
		exposureValue *= 0.6;
		float powValue = pow(2.0, exposureValue);
		vec4 color = retColor;

		retColor = calPowColor(color, powValue);
	}

	if (u_brightness != 0.0) // brightness
	{
		float brightValue = calBrightValue(u_brightness);
		vec4 color = retColor;

		retColor = newBrightness(color, brightValue);
	}

	if (u_contrast != 0.0) // contrast
	{
		vec3 resultVal = calContrastValue(u_contrast);
		vec4 color = retColor;

		retColor = calContrastColor(color, resultVal.x, resultVal.y, resultVal.z);
	}

	if (u_vib != 0.0) // vibrance
	{
		float vibrance = u_vib / 100.0;
		vibrance *= 0.8;
		vec4 color = retColor;
		float luma = luminance(color.rgb);
		vec4 mask = color - luma;
		mask = clamp(mask, vec4(0.0), vec4(1.0));
		float lumaMask = 1.0 - luminance(mask.rgb);

		retColor = mixLumaAndColor(color, luma, lumaMask, vibrance);
	}

	if (u_sat != 0.0) // saturation
	{
		float saturation = u_sat / 100.0;
		vec4 color = retColor;

		retColor = calNewColor(color, saturation);
	}

    if (u_chroma_transparence != 1.0)
    {
        //if(u_chroma_similarity==0.01)
        if(u_chroma_similarity==0.0)
        {
            if(retColor.r == u_chroma_r && retColor.g == u_chroma_g && retColor.b == u_chroma_b)
            {
                retColor.a = retColor.a * u_chroma_transparence;
                //retColor.a = retColor.r == u_chroma_r? u_chroma_transparence:0.5;
            }
        }
        else
        {
            vec3 labA = rgbToLab(retColor.rgb);
            vec3 labB = rgbToLab(vec3(u_chroma_r, u_chroma_g, u_chroma_b));
            float d = deltaE(labA, labB);
            if(d<=u_chroma_similarity*100.0)
            {
                retColor.a = retColor.a * u_chroma_transparence;
            }
        }
    }

#if 0
	if (u_highLight != 0.0) // highLight
	{
		float coff = u_highLight / 100.0;
		vec4 color = retColor;

		retColor = calHighlight(color, coff);
	}

	if (u_shadow != 0.0) // shadow
	{
		float coff = u_shadow / 100.0;
		if (u_shadow > 0.0)
			coff *= 2.0;
		else
			coff /= 2.0;

		vec4 color = retColor;

		retColor = calShadow(color, coff);
	}

	if (u_whiteLevel != 0.0) // white level
	{
		float whiteCoff = u_whiteLevel / 100.0;
		vec4 color = retColor;

		retColor = calWhiteLevelPixel(color, whiteCoff);
	}

	if (u_blackLevel != 0.0) // black level
	{
		float blackCoff = u_blackLevel / 100.0;

		if (u_blackLevel > 0.0)
			blackCoff /= 2.0;
		else
			blackCoff *= 2.0;

		vec4 color = retColor;

		retColor = calBlackLevelPixel(color, blackCoff);
	}

    float Red_hueVal = u_hsl_red_hue;
    float Red_satVal = u_hsl_red_sat;
    float Red_brightnessVal = u_hsl_red_brightness;
    const float Red_degreeMinVal = 338.0;
    const float Red_degreeMaxVal = 25.0;
	if (Red_hueVal != 0.0 || Red_satVal != 0.0 || Red_brightnessVal != 0.0) // Red HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Red_degreeMinVal, Red_degreeMaxVal);
		vec3 newColor;

		if (Red_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Red_hueVal);
		else
			newColor = color.rgb;

		if (Red_satVal != 0.0)
		{
			float satRadio = Red_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Red_brightnessVal != 0.0)
		{
			float brightnessRadio = Red_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

    float Orange_hueVal = u_hsl_orange_hue;
    float Orange_satVal = u_hsl_orange_sat;
    float Orange_brightnessVal = u_hsl_orange_brightness;
    const float Orange_degreeMinVal = 8.0;
    const float Orange_degreeMaxVal = 45.0;
	if (Orange_hueVal != 0.0 || Orange_satVal != 0.0 || Orange_brightnessVal != 0.0) // Orange HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Orange_degreeMinVal, Orange_degreeMaxVal);
		vec3 newColor;

		if (Orange_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Orange_hueVal);
		else
			newColor = color.rgb;

		if (Orange_satVal != 0.0)
		{
			float satRadio = Orange_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Orange_brightnessVal != 0.0)
		{
			float brightnessRadio = Orange_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

    float Yellow_hueVal = u_hsl_yellow_hue;
    float Yellow_satVal = u_hsl_yellow_sat;
    float Yellow_brightnessVal = u_hsl_yellow_brightness;
    const float Yellow_degreeMinVal = 35.0;
    const float Yellow_degreeMaxVal = 100.0;
	if (Yellow_hueVal != 0.0 || Yellow_satVal != 0.0 || Yellow_brightnessVal != 0.0) // Yellow HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Yellow_degreeMinVal, Yellow_degreeMaxVal);
		vec3 newColor;

		if (Yellow_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Yellow_hueVal);
		else
			newColor = color.rgb;

		if (Yellow_satVal != 0.0)
		{
			float satRadio = Yellow_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Yellow_brightnessVal != 0.0)
		{
			float brightnessRadio = Yellow_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

    float Green_hueVal = u_hsl_green_hue;
    float Green_satVal = u_hsl_green_sat;
    float Green_brightnessVal = u_hsl_green_brightness;
    const float Green_degreeMinVal = 65.5;
    const float Green_degreeMaxVal = 171.;
	if (Green_hueVal != 0.0 || Green_satVal != 0.0 || Green_brightnessVal != 0.0) // Green HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Green_degreeMinVal, Green_degreeMaxVal);
		vec3 newColor;

		if (Green_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Green_hueVal);
		else
			newColor = color.rgb;

		if (Green_satVal != 0.0)
		{
			float satRadio = Green_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Green_brightnessVal != 0.0)
		{
			float brightnessRadio = Green_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

    float Magenta_hueVal = u_hsl_magenta_hue;
    float Magenta_satVal = u_hsl_magenta_sat;
    float Magenta_brightnessVal = u_hsl_magenta_brightness;
    const float Magenta_degreeMinVal = 281.;
    const float Magenta_degreeMaxVal = 330.;
	if (Magenta_hueVal != 0.0 || Magenta_satVal != 0.0 || Magenta_brightnessVal != 0.0) // Magenta HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Magenta_degreeMinVal, Magenta_degreeMaxVal);
		vec3 newColor;

		if (Magenta_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Magenta_hueVal);
		else
			newColor = color.rgb;

		if (Magenta_satVal != 0.0)
		{
			float satRadio = Magenta_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Magenta_brightnessVal != 0.0)
		{
			float brightnessRadio = Magenta_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

    float Purple_hueVal = u_hsl_purple_hue;
    float Purple_satVal = u_hsl_purple_sat;
    float Purple_brightnessVal = u_hsl_purple_brightness;
    const float Purple_degreeMinVal = 258.;
    const float Purple_degreeMaxVal = 280.;
	if (Purple_hueVal != 0.0 || Purple_satVal != 0.0 || Purple_brightnessVal != 0.0) // Purple HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Purple_degreeMinVal, Purple_degreeMaxVal);
		vec3 newColor;

		if (Purple_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Purple_hueVal);
		else
			newColor = color.rgb;

		if (Purple_satVal != 0.0)
		{
			float satRadio = Purple_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Purple_brightnessVal != 0.0)
		{
			float brightnessRadio = Purple_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

    float Blue_hueVal = u_hsl_blue_hue;
    float Blue_satVal = u_hsl_blue_sat;
    float Blue_brightnessVal = u_hsl_blue_brightness;
    const float Blue_degreeMinVal = 195.;
    const float Blue_degreeMaxVal = 250.;
	if (Blue_hueVal != 0.0 || Blue_satVal != 0.0 || Blue_brightnessVal != 0.0) // Blue HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Blue_degreeMinVal, Blue_degreeMaxVal);
		vec3 newColor;

		if (Blue_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Blue_hueVal);
		else
			newColor = color.rgb;

		if (Blue_satVal != 0.0)
		{
			float satRadio = Blue_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Blue_brightnessVal != 0.0)
		{
			float brightnessRadio = Blue_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

    float Aqua_hueVal = u_hsl_aqua_hue;
    float Aqua_satVal = u_hsl_aqua_sat;
    float Aqua_brightnessVal = u_hsl_aqua_brightness;
    const float Aqua_degreeMinVal = 168.;
    const float Aqua_degreeMaxVal = 218.;
	if (Aqua_hueVal != 0.0 || Aqua_satVal != 0.0 || Aqua_brightnessVal != 0.0) // Aqua HSL
	{
		vec4 color = retColor;
		vec3 hsl_Val = HSL_RGBtoHSL(color.rgb);
		float pixelAlpha = calTransperency(hsl_Val, Aqua_degreeMinVal, Aqua_degreeMaxVal);
		vec3 newColor;

		if (Aqua_hueVal != 0.0)
			newColor = doAdjustHue(color.rgb, hsl_Val, pixelAlpha, Aqua_hueVal);
		else
			newColor = color.rgb;

		if (Aqua_satVal != 0.0)
		{
			float satRadio = Aqua_satVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustStaturation(newColor, hsl_Val, pixelAlpha, satRadio);
		}

		if (Aqua_brightnessVal != 0.0)
		{
			float brightnessRadio = Aqua_brightnessVal / 100.0;
			hsl_Val = HSL_RGBtoHSL(newColor.rgb);
			newColor = doAdjustBrightness(newColor, hsl_Val, pixelAlpha, brightnessRadio);
		}

		retColor = vec4(newColor, color.a);
	}

	if (u_vignette_amount != 0.0 || u_vignette_exposure != 0.0)
	{
		float amountRatio = u_vignette_amount / 100.0;
		float featherRatio = u_vignette_feather / 100.0;
		float coffHighlight = u_vignette_highlights / 100.0;
		float sizeRatio = u_vignette_size / 100.0;
		float roundRatio = u_vignette_roundness / 100.0;
		float exposureRatio = u_vignette_exposure / 100.0;
		vec2 position;
		vec2 center;

		coffHighlight = (2.0 * coffHighlight - 1.0) * abs(amountRatio);
		exposureRatio = pow(2.0, exposureRatio);

		position.x = uv.x * float(u_resolution.x);
		position.y = uv.y * float(u_resolution.y);

		center.x = float(u_resolution.x / 2.);
		center.y = float(u_resolution.y / 2.);

		float maskValue = generateGradient(position, center, featherRatio, sizeRatio, roundRatio);

		vec4 color = retColor;

		retColor = vignettePixel(color, maskValue, amountRatio, exposureRatio, coffHighlight);
	}
#endif
	gl_FragColor = retColor;
}
