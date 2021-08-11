#pragma once

const char* const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	layout(location = 0) in vec3 vp;
	layout(location = 1) in vec3 vn;
	layout(location = 2) in vec2 uv;
	
	uniform float speedOfLight;

	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 invM;

	uniform vec4 observersVelocity;
	uniform vec4 observersLocation;

	uniform int intersectionType;	// 0 = lightCone, 1 = hyperplane

	//WorldLine:
	uniform int worldLineType;	// 0 = geodetic

	out vec3 wPos;
	out vec3 norm;
	out vec2 texCoord;
	out float dopplerShift;


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
		return t;
	}


	float GeodeticIntersectHyperplane(vec4 offsetedStartPos, vec4 planeLocation, vec4 planeNormal) {
		return dot(planeLocation - offsetedStartPos, planeNormal)
				/ dot(subjectsVelocity, planeNormal);
	}

	vec4 GeodeticLocationAtAbsoluteTime(vec4 offsetedStartPos, float t) {
		return offsetedStartPos + subjectsVelocity * t;
	}

	vec4 GeodeticVelocityAtAbsoluteTime(vec4 offsetedStartPos, float t) {
		return subjectsVelocity;
	}
	//-----------------------------------------------------------------------

	/*
	* Calculates coeficient for Doppler shift of perceived color.
	*/
	float calculateDopplerShift(vec4 vertexVelocity, vec4 vertexLocation) {

		vec4 toSubject4 = vertexLocation - observersLocation;
		vec3 toSubject = normalize(vec3(toSubject4.xyz));

		vec4 relVelocity4 = vertexVelocity - observersVelocity;
		vec3 relVelocity = vec3(relVelocity4.xyz);

		float v = dot(toSubject, relVelocity);  //Approach speed
		return sqrt((speedOfLight + v) / (speedOfLight - v));
	}

	/*
		Calculates the intersection of a hyperplane and world line of this vertex
		Also calculates the Doppler shift for this vertex.
	*/
	void geodetic() {

        vec4 offsetedStartPos = subjectsStartPos + vec4(vp, 0);		// Start position of the world line of this vertex.
		
		
		//Intersect:
		float t = 0;
		if (intersectionType == 0) {
			//Light cone:
			vec4 coneLocation = observersLocation;
			t = GeodeticIntersectLightCone(offsetedStartPos, coneLocation);
		}
		else if (intersectionType == 1) {
			//Simultaneous hyperplane of the observer:
			vec4 planeLocation = observersLocation;
			vec4 planeNormal = normalize(vec4(-(observersVelocity.xyz), observersVelocity.w));
			t = GeodeticIntersectHyperplane(offsetedStartPos, planeLocation, planeNormal);			
		}

		vec4 vertexLocation = GeodeticLocationAtAbsoluteTime(offsetedStartPos, t);
		vec4 vertexVelocity = GeodeticVelocityAtAbsoluteTime(offsetedStartPos, t);
		
		dopplerShift = calculateDopplerShift(vertexVelocity, vertexLocation);
		wPos = vertexLocation.xyz;
	}



	void main() {
		geodetic();
		texCoord = vec2(uv.x, 1 - uv.y);
		norm = (invM * vec4(vn, 0)).xyz;
		gl_Position = vec4(wPos, 1) * MVP;
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

