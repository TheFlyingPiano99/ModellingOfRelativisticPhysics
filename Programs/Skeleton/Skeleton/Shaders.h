#pragma once

const char* const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	layout(location = 0) in vec3 vp;
	layout(location = 1) in vec3 vn;
	layout(location = 2) in vec2 uv;
	layout(location = 3) in float ds;

	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 invM;

	out vec3 wPos;
	out vec3 norm;	
	out vec2 texCoord;
	out float dopplerShift;

	void main() {		
		wPos = (vec4(vp, 1) * M).xyz;
		norm = (invM * vec4(vn, 0)).xyz;
		texCoord = vec2(uv.x, 1 - uv.y);
		dopplerShift = ds;
		gl_Position = vec4(vp, 1) * MVP;
	}
)";

const char* const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers

//Object:
	in vec3 wPos;
	in vec3 norm;
	in vec2 texCoord;
	in float dopplerShift;
	//uniform float dopplerShift;

	uniform mat4 M;
	uniform mat4 invM;
	uniform bool glow;
	
	uniform sampler2D textureUnit;
	uniform sampler2D normalMapUnit;
	uniform sampler2D specularMapUnit;

//Camera:
	uniform vec3 wEye;

//LightSources:	
	uniform vec3 La;	
	uniform vec3 lightPos0;
	uniform vec3 lightPos1;
	uniform vec3 lightL0;
	uniform vec3 lightL1;

//Material:	
	//uniform vec3 ka;
	uniform vec3 kd;
	//uniform vec3 ks;
	uniform float shininess;
	uniform bool depthShading;

	out vec4 outColor;

	/**
	 * Taken from Earl F. Glynn's web page:
	 * <a href="http://www.efg2.com/Lab/ScienceAndEngineering/Spectra.htm">Spectra Lab Report</a>
	 */
	vec3 waveLengthToRGB(float Wavelength) {
		float Gamma = 0.80;
		float IntensityMax = 255;
		float factor;
		float Red, Green, Blue;

		if ((Wavelength >= 380.0) && (Wavelength < 440.0)) {
			Red = -(Wavelength - 440.0) / (440.0 - 380.0);
			Green = 0.0;
			Blue = 1.0;
		}
		else if ((Wavelength >= 440.0) && (Wavelength < 490.0)) {
			Red = 0.0;
			Green = (Wavelength - 440.0) / (490.0 - 440.0);
			Blue = 1.0;
		}
		else if ((Wavelength >= 490.0) && (Wavelength < 510.0)) {
			Red = 0.0;
			Green = 1.0;
			Blue = -(Wavelength - 510.0) / (510.0 - 490.0);
		}
		else if ((Wavelength >= 510.0) && (Wavelength < 580.0)) {
			Red = (Wavelength - 510.0) / (580.0 - 510.0);
			Green = 1.0;
			Blue = 0.0;
		}
		else if ((Wavelength >= 580.0) && (Wavelength < 645.0)) {
			Red = 1.0;
			Green = -(Wavelength - 645.0) / (645.0 - 580.0);
			Blue = 0.0;
		}
		else if ((Wavelength >= 645.0) && (Wavelength < 781.0)) {
			Red = 1.0;
			Green = 0.0;
			Blue = 0.0;
		}
		else {
			Red = 0.0;
			Green = 0.0;
			Blue = 0.0;
		}

		// Let the intensity fall off near the vision limits

		if ((Wavelength >= 380) && (Wavelength < 420)) {
			factor = 0.3 + 0.7 * (Wavelength - 380.0) / (420.0 - 380.0);
		}
		else if ((Wavelength >= 420) && (Wavelength < 701)) {
			factor = 1.0;
		}
		else if ((Wavelength >= 701) && (Wavelength < 781)) {
			factor = 0.3 + 0.7 * (780.0 - Wavelength) / (780.0 - 700.0);
		}
		else {
			factor = 0.0;
		}

		vec3 rgb;
		// Don't want 0^x = 1 for x <> 0
		rgb.x = Red == 0.0 ? 0 : round(IntensityMax * pow(Red * factor, Gamma));
		rgb.y = Green == 0.0 ? 0 : round(IntensityMax * pow(Green * factor, Gamma));
		rgb.z = Blue == 0.0 ? 0 : round(IntensityMax * pow(Blue * factor, Gamma));
		rgb = rgb / IntensityMax;
		return rgb;
	}

	void main() {
		vec4 rawColor = texture(textureUnit, texCoord);		
		
		float attitude = 1.0;
		if (!glow) {
			attitude = dot(vec3(-1,1,1), norm);
		}
		vec4 shaded = rawColor * ((attitude > 0.1)? attitude : 0.1);
	

		vec3 redShifted = waveLengthToRGB(625.0 * dopplerShift);	// Red
		vec3 greenShifted = waveLengthToRGB(525.0 * dopplerShift);	// Green
		vec3 blueShifted = waveLengthToRGB(460.0 * dopplerShift);	// Blue
		vec3 sumShifted = (shaded.x * redShifted + shaded.y * greenShifted + shaded.z * blueShifted);


		outColor = vec4(sumShifted, rawColor.w);
	}
)";


//GLSL lighting function:

	/*
vec3 DirectLight() {
	vec3 eyeDir = normalize(wEye - wPos);

	vec3 outRadiance = ka * La;

	//Light0:
	vec3 lightDir = normalize(lightPos0 - wPos);
	float dist = length(lightPos0 - wPos);
	vec3 radiance = lightL0 / dist / dist;
	float cosTheta = dot(norm, lightDir);
	vec3 halfway;
	float cosDelta;
	if (cosTheta > 0) {
		outRadiance = outRadiance + radiance * kd * cosTheta;
		halfway = normalize(eyeDir + lightDir);
		cosDelta = dot(norm, halfway);
		if (cosDelta > 0) {
			outRadiance = outRadiance + radiance * ks * pow(cosDelta, shininess);
		}
	}

	//Light1:
	lightDir = normalize(lightPos1 - wPos);
	dist = length(lightPos1 - wPos);
	radiance = lightL1 / dist / dist;
	cosTheta = dot(norm, lightDir);
	if (cosTheta > 0) {
		outRadiance = outRadiance + radiance * kd * cosTheta;
		halfway = normalize(eyeDir + lightDir);
		cosDelta = dot(norm, halfway);
		if (cosDelta > 0) {
			outRadiance = outRadiance + radiance * ks * pow(cosDelta, shininess);
		}
	}
	if (depthShading) {
		int depth = int(-wPos.z * 3.0f);
		if (depth > 0) {
			outRadiance /= float(depth);
		}
	}
	return outRadiance;
}
*/

