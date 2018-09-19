uniform sampler2D mSample0;
varying vec2 textureCoord;
uniform vec2 u_resolution;

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
// HighLight parameter
uniform float u_highLight;
// Shadow parameter
uniform float u_shadow;
// HDR white&black level parameter
uniform float u_whiteLevel;
uniform float u_blackLevel;

// Vignette parameter
uniform float u_vignette_amount;
uniform float u_vignette_feather;
uniform float u_vignette_highlights;
uniform float u_vignette_size;
uniform float u_vignette_roundness;
uniform float u_vignette_exposure;


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
void main( )
{
    vec2 uv = textureCoord;
	vec4 color = texture2D(mSample0, uv);
	vec4 retColor = color;

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

	gl_FragColor = retColor;
}
