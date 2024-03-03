	#version 330 core			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers

	uniform bool textMode;
	uniform vec3 transparentColor;

	uniform int viewMode;	// 0 = realTime3D, 1 = diagram
	uniform int dopplerMode;	// 0 = full, 1 = mild, 2 = off
	uniform bool doShading;

//Object:
	in vec3 wPos;
	in vec3 norm;
	in vec2 texCoord;
	in float dopplerShift;

	uniform mat4 M;
	uniform mat4 invM;
	uniform bool glow;
	uniform bool noTexture;
	uniform bool outline;
	
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
//Direction light:
	uniform vec3 lightPos2;
	uniform vec3 lightL2;


//Material:	
	uniform vec3 ka;
	uniform vec3 kd;
	uniform vec3 ks;
	uniform float shininess;
	uniform bool depthShading;
	uniform float transparency;

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

	vec3 DirectLight(vec3 rawColor) {
		vec3 eyeDir;
		if (viewMode == 0) {		// Real3D
			eyeDir = normalize(-wPos);			// eye is in (0,0,0)
		}
		else {
			eyeDir = normalize(wEye - wPos);
		}
		 

		vec3 outRadiance = ka * La;

		//Light0:	(Point light)

		vec3 lightDir = normalize(lightPos0 - wPos);
		float dist = length(lightPos0 - wPos);
		vec3 radiance = lightL0 / dist / dist;
		float cosTheta = dot(norm, lightDir);
		vec3 halfway;
		float cosDelta;
		if (cosTheta > 0) {
			outRadiance = outRadiance + radiance * kd * cosTheta * rawColor;
			halfway = normalize(eyeDir + lightDir);
			cosDelta = dot(norm, halfway);
			if (cosDelta > 0) {
				outRadiance = outRadiance + radiance * ks * pow(cosDelta, shininess);
			}
		}

		//Light1:	(Point light)
		lightDir = normalize(lightPos1 - wPos);
		dist = length(lightPos1 - wPos);
		radiance = lightL1 / dist / dist;
		cosTheta = dot(norm, lightDir);
		if (cosTheta > 0) {
			outRadiance = outRadiance + radiance * kd * cosTheta * rawColor;
			halfway = normalize(eyeDir + lightDir);
			cosDelta = dot(norm, halfway);
			if (cosDelta > 0) {
				outRadiance = outRadiance + radiance * ks * pow(cosDelta, shininess);
			}
		}

		//Light2:	(Direction light)
		lightDir = normalize(lightPos2);
		radiance = lightL2;
		cosTheta = dot(norm, lightDir);
		if (cosTheta > 0) {
			outRadiance = outRadiance + radiance * kd * cosTheta * rawColor;
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

	vec3 shading(vec4 rawColor) {
		
		vec3 shaded;
		if (glow || !doShading) {
			if (noTexture) {
				shaded = kd;
			}
			else {
				shaded = rawColor.xyz;
			}
		}
		else {
			if (noTexture) {
				shaded = DirectLight(kd.xyz);
			}
			else {
				shaded = DirectLight(rawColor.xyz);
			}
		}
		return shaded;
	}

	void realTime3D() {
		if (outline) {
			if (dot(normalize(-wPos), norm) < 0.3) {		// Eye position is in (0,0,0), when using intersections
				outColor = vec4(0,1,0,1);							// green outline
				return;
			}
		}
		vec4 rawColor = texture(textureUnit, texCoord);		
		vec3 shaded = shading(rawColor);

		vec3 redShifted = waveLengthToRGB(625.0 * dopplerShift);	// Red
		vec3 greenShifted = waveLengthToRGB(526.0 * dopplerShift);	// Green
		vec3 blueShifted = waveLengthToRGB(444.0 * dopplerShift);	// Blue
		vec3 sumShifted = (shaded.x * redShifted + shaded.y * greenShifted + shaded.z * blueShifted);		
		outColor = vec4(sumShifted, rawColor.w);
	}

	void diagram() {
		vec4 rawColor = texture(textureUnit, texCoord);		
		vec3 shaded = shading(rawColor);
		if (transparency < 1.0) {
			outColor = vec4(shaded, transparency * (1 - dot(norm, normalize(wEye - wPos)) * dot(norm, normalize(wEye - wPos))));
		}
		else {
			outColor = vec4(shaded, 1);
		}
	}

	void text() {
		vec4 raw = texture(textureUnit, texCoord);
		outColor = vec4(raw.xyz * kd, (transparentColor.x == raw.x && transparentColor.y == raw.y && transparentColor.z == raw.z)? 0 : 1);
	}

	void main() {
		if (textMode) {
			text();
		}
		else {
			if (viewMode == 0) {		// RealTime3D
				realTime3D();
			}
			else if (viewMode == 1) {	// Diagram
				diagram();
			}
		}
	}
