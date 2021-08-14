#pragma once

const char* const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	layout(location = 0) in vec3 vp;
	layout(location = 1) in vec3 vn;
	layout(location = 2) in vec2 uv;
	
	uniform float speedOfLight;
	uniform int dopplerMode;	// 0 = full, 1 = mild, 2 = off
	uniform int viewMode;	// 0 = realTime3D, 1 = diagram
	uniform bool doLorentz;
	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 invM;

	uniform vec3 wEye;
	uniform vec4 observersVelocity;
	uniform vec4 observersLocation;
	uniform vec4 observersStartPos;

	uniform int intersectionMode;	// 0 = lightCone, 1 = hyperplane

	//WorldLine:
	uniform int worldLineType;	// 0 = geodetic

	out vec3 wPos;
	out vec3 norm;
	out vec2 texCoord;
	out float dopplerShift;

	mat4 TranslateMatrix(vec3 t) {
		return mat4(vec4(1,   0,   0,   0),
					vec4(0,   1,   0,   0),
					vec4(0,   0,   1,   0),
					vec4(t.x, t.y, t.z, 1));
	}


	vec2 solveQuadraticFunction(float a, float b, float c, out int noOfRealSolutions) {
		if (a == 0.0f && b != 0.0f) {
			noOfRealSolutions = 1;
			return vec2(-c / b, -c / b);
		}
		float discriminant = b * b - 4 * a * c;
		vec2 results;
		if (discriminant > 0.0f) {
			noOfRealSolutions = 2;
			results.x = (-b + sqrt(discriminant)) / (2 * a);
			results.y = (-b - sqrt(discriminant)) / (2 * a);
		}
		else if (discriminant == 0.0f) {
			noOfRealSolutions = 1;
			results.x = (-b + sqrt(discriminant)) / (2 * a);
			results.y = results.x;
		}
		else {
			noOfRealSolutions = 0;
		}
		return results;
	}


	//if Geodetic:-----------------------------------------------------------
	uniform vec4 subjectsStartPos;
	uniform vec4 subjectsVelocity;

	float GeodeticIntersectLightCone(vec4 offsetedStartPos, vec4 coneLocation)
	{
		float a, b, c, t;
		a = dot(subjectsVelocity.xyz, subjectsVelocity.xyz) - subjectsVelocity.w * subjectsVelocity.w;
		vec3 temp = (offsetedStartPos.xyz * subjectsVelocity.xyz - subjectsVelocity.xyz * coneLocation.xyz);
		b = 2 * ((temp.x + temp.y + temp.z) - (offsetedStartPos.w * subjectsVelocity.w - subjectsVelocity.w * coneLocation.w));
		c = dot(coneLocation.xyz - offsetedStartPos.xyz, coneLocation.xyz - offsetedStartPos.xyz)
			- pow(coneLocation.w - offsetedStartPos.w, 2);

		int noOfSolutions;
		vec2 solutions = solveQuadraticFunction(a, b, c, noOfSolutions);

		t = solutions.x;	// Should be tested, whether its from the past!
		return t * subjectsVelocity.w;
	}


	float GeodeticIntersectHyperplane(vec4 offsetedStartPos, vec4 planeLocation, vec4 planeNormal) {
		return dot(planeLocation - offsetedStartPos, planeNormal)
				/ dot(subjectsVelocity, planeNormal) * subjectsVelocity.w;
	}

	vec4 GeodeticLocationAtAbsoluteTime(vec4 offsetedStartPos, float t) {
		return offsetedStartPos + subjectsVelocity / subjectsVelocity.w * t;
	}

	vec4 GeodeticVelocityAtAbsoluteTime(vec4 offsetedStartPos, float t) {
		return subjectsVelocity;
	}
	//-----------------------------------------------------------------------

	/*
	* Converts four-velocity [magnitude = c] to 3D velocity [m/m];
	*/
	vec3 To3DVelocity(const vec4 fv) {
		vec4 scaled = fv / fv.w;
		return vec3(scaled.xyz);
	}

	float lorentzFactor(float relVelocity) {
		return 1.0f / sqrt(1.0f - (relVelocity * relVelocity) / (speedOfLight * speedOfLight));
	}

	vec4 lorentzTransformation(vec4 toTransform, vec3 relVelocity) {
		if (length(relVelocity) >= speedOfLight) {
			return toTransform;
		}
		vec3 v = relVelocity;
		float vLength = length(v);
		float gamma = lorentzFactor(vLength);

		vec3 r = vec3(toTransform.xyz);
		float t = toTransform.w;
		vec3 n = normalize(v);
		float tTrans = gamma * (t - dot(vLength * n, r) / speedOfLight / speedOfLight);
		vec3 rTrans = r + (gamma - 1) * dot(r, n) * n - gamma * t * vLength * n;
		return vec4(rTrans, tTrans);
	}

	vec3 lorentzTransformationOfVelocity(vec3 toTransform, vec3 relVelocity) {
		if (length(relVelocity) >= speedOfLight || length(relVelocity) == 0.0f) {
			return toTransform;
		}
		float gammaV = 1 / sqrt(1 - dot(relVelocity, relVelocity) / speedOfLight / speedOfLight);
		return (1 / (1 - dot(relVelocity, toTransform) / speedOfLight / speedOfLight))
			* (toTransform / gammaV - relVelocity
			+ (1 / speedOfLight / speedOfLight) * (gammaV / (gammaV + 1))
			* dot(toTransform, relVelocity) * relVelocity);
	}

	/*
	* Calculates coeficient for Doppler shift of perceived color.
	*/
	float calculateDopplerShift(vec3 vertexVelocity, vec3 vertexLocation, vec3 observerLocationProperFrame) {

		vec3 toSubject = normalize(vertexLocation - observerLocationProperFrame);

		float v = dot(toSubject, vertexVelocity);  //Approach speed
		return sqrt((speedOfLight + v) / (speedOfLight - v));
	}

	/*
		Calculates the intersection of a hyperplane and world line of this vertex
		Also calculates the Doppler shift for this vertex.
	*/
	void geodetic() {

        // Start position of the world line of this vertex in absolute frame.	(Length contraction is applied)
		vec4 offsetedStartPos;
		if (doLorentz) {
			vec3 v = To3DVelocity(subjectsVelocity);
			float gamma = lorentzFactor(length(v));
			vec3 n = normalize(v);
			vec3 pParalel = dot(vp, n) * n;
			vec3 pPerpend = vp - pParalel;
			offsetedStartPos = vec4(pPerpend + pParalel / gamma, 0) + subjectsStartPos;
		}
		else {
			offsetedStartPos = vec4(vp, 0) + subjectsStartPos;
		}

		//Intersect:
		float t = 0;	// absolute time parametre
		if (intersectionMode == 0) {
			//Light cone:
			vec4 coneLocation = observersLocation;
			t = GeodeticIntersectLightCone(offsetedStartPos, coneLocation);
		}
		else if (intersectionMode == 1) {
			//Simultaneous hyperplane of the observer:
			vec4 planeLocation = observersLocation;
			vec4 planeNormal = normalize(vec4(-(observersVelocity.xyz), observersVelocity.w));
			t = GeodeticIntersectHyperplane(offsetedStartPos, planeLocation, planeNormal);			
		}

		// From here forward everything is in observers frame:
		
		vec3 vertexLocationProperFrame;
		vec3 vertexVelocityProperFrame;
		vec3 observersLocationProperFrame;
		if (doLorentz) {
			vec4 shiftedOrigoFrame = GeodeticLocationAtAbsoluteTime(offsetedStartPos, t) - observersStartPos;
			vertexLocationProperFrame = lorentzTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity)).xyz;
			vertexVelocityProperFrame = lorentzTransformationOfVelocity(To3DVelocity(GeodeticVelocityAtAbsoluteTime(offsetedStartPos, t)), To3DVelocity(observersVelocity));
			shiftedOrigoFrame = observersLocation - observersStartPos;
			observersLocationProperFrame = lorentzTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity)).xyz;
		}
		else {
			vertexLocationProperFrame = GeodeticLocationAtAbsoluteTime(offsetedStartPos, t).xyz;
			vertexVelocityProperFrame = To3DVelocity(GeodeticVelocityAtAbsoluteTime(offsetedStartPos, t));
			observersLocationProperFrame = observersLocation.xyz;
		}
		
		if (dopplerMode == 0) {	// Full
			dopplerShift = calculateDopplerShift(vertexVelocityProperFrame, vertexLocationProperFrame, observersLocationProperFrame);
		}
		else if (dopplerMode == 1) {	// Mild
			dopplerShift = calculateDopplerShift(vertexVelocityProperFrame, vertexLocationProperFrame, observersLocationProperFrame);
			dopplerShift = (dopplerShift - 1) * 0.1 + 1;
		}
		else if (dopplerMode == 2) {	// Off
			dopplerShift = 1.0f;			
		}

		wPos = GeodeticLocationAtAbsoluteTime(offsetedStartPos, t).xyz;
		texCoord = vec2(uv.x, 1 - uv.y);
		norm = (invM * vec4(vn, 0)).xyz;
		mat4 Mat = /* TranslateMatrix(-observersLocationProperFrame) * */ MVP;
		gl_Position = vec4(vertexLocationProperFrame, 1) * Mat;
	}

	void diagram() {
		wPos = (vec4(vp, 1) * M).xyz;
		texCoord = vec2(uv.x, 1 - uv.y);
		norm = (invM * vec4(vn, 0)).xyz;
		dopplerShift = 1.0f;			
		gl_Position = vec4(vp, 1) *  MVP;
	}

	void main() {
		if (viewMode == 0) {	// RealTime3D
			if (worldLineType == 0) {
				geodetic();
			}
		}
		else if (viewMode == 1) {	// Diagram
			diagram();
		}
	}
)";

const char* const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers

	uniform int viewMode;	// 0 = realTime3D, 1 = diagram

//Object:
	in vec3 wPos;
	in vec3 norm;
	in vec2 texCoord;
	in float dopplerShift;

	uniform mat4 M;
	uniform mat4 invM;
	uniform bool glow;
	uniform bool noTexture;
	
	
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
	uniform vec3 ka;
	uniform vec3 kd;
	uniform vec3 ks;
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

	vec3 DirectLight(vec3 rawColor) {
		vec3 eyeDir = normalize(wEye - wPos);

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

	void realTime3D() {
		vec4 rawColor = texture(textureUnit, texCoord);		
		
		vec3 shaded;
		if (glow) {
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
		
		if (dopplerShift != 1.0) {
			vec3 redShifted = waveLengthToRGB(625.0 * dopplerShift);	// Red
			vec3 greenShifted = waveLengthToRGB(525.0 * dopplerShift);	// Green
			vec3 blueShifted = waveLengthToRGB(460.0 * dopplerShift);	// Blue
			vec3 sumShifted = (shaded.x * redShifted + shaded.y * greenShifted + shaded.z * blueShifted);		
			outColor = vec4(sumShifted, rawColor.w);
		}
		else {
			outColor = vec4(shaded, rawColor.w);;
		}
	}

	void main() {
		realTime3D();
	}
)";


