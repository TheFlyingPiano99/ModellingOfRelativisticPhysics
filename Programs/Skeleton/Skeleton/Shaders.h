#pragma once

const char* const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	layout(location = 0) in vec4 vp;
	layout(location = 1) in vec3 vn;
	layout(location = 2) in vec2 uv;

	uniform	vec4 worldLineNodes[1000];
	uniform int noOfWorldLineNodes;
	
	uniform int diagramX;
	uniform int diagramY;
	uniform int diagramZ;

	uniform float speedOfLight;
	uniform int dopplerMode;	// 0 = full, 1 = mild, 2 = off
	uniform int viewMode;	// 0 = realTime3D, 1 = diagram
	uniform bool doLorentz;
	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 invM;
	uniform bool textMode;
	uniform bool directRenderMode;
	uniform vec3 wEye;
	uniform vec4 observersVelocity;
	uniform vec4 observersLocation;
	uniform vec4 observersStartPos;
	uniform bool transformToProperFrame;

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

	float GeodeticIntersectLightCone(vec4 offsetedStartPos, vec4 tangentVelocity, vec4 coneLocation)
	{
		float a, b, c, t;
		a = dot(tangentVelocity.xyz, tangentVelocity.xyz) - tangentVelocity.w * tangentVelocity.w;
		vec3 temp = (offsetedStartPos.xyz * tangentVelocity.xyz - tangentVelocity.xyz * coneLocation.xyz);
		b = 2 * ((temp.x + temp.y + temp.z) - (offsetedStartPos.w * tangentVelocity.w - tangentVelocity.w * coneLocation.w));
		c = dot(coneLocation.xyz - offsetedStartPos.xyz, coneLocation.xyz - offsetedStartPos.xyz)
			- pow(coneLocation.w - offsetedStartPos.w, 2);

		int noOfSolutions;
		vec2 solutions = solveQuadraticFunction(a, b, c, noOfSolutions);

		t = solutions.x;	// Should be tested, whether its from the past!
		return t * tangentVelocity.w;
	}


	float GeodeticIntersectHyperplane(vec4 offsetedStartPos, vec4 tangentVelocity, vec4 planeLocation, vec4 planeNormal) {
		return dot(planeLocation - offsetedStartPos, planeNormal)
				/ dot(tangentVelocity, planeNormal) * tangentVelocity.w;
	}

	vec4 GeodeticLocationAtAbsoluteTime(vec4 offsetedStartPos, vec4 tangentVelocity, float t) {
		return offsetedStartPos + tangentVelocity / tangentVelocity.w * t;
	}

	vec4 GeodeticVelocityAtAbsoluteTime(vec4 offsetedStartPos, vec4 tangentVelocity, float t) {
		return tangentVelocity;
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
	* Calculates coeficient for Doppler shift of perceived color. Given, that observer is positioned in (0,0,0).
	*/
	float calculateDopplerShift(vec3 vertexVelocity, vec3 vertexLocation) {

		vec3 toSubject = normalize(vertexLocation);

		float v = dot(toSubject, vertexVelocity);  //Approach speed
		return sqrt((speedOfLight + v) / (speedOfLight - v));
	}

	/*
		Calculates the intersection of a hyperplane and world line of this vertex
		Also calculates the Doppler shift for this vertex.
	*/
	void geodetic() {
		float t = 0;		// absolute time parametre
		vec4 offsetedStartPos;
		vec4 tangentVelocity;
		for (int i = 0; i < noOfWorldLineNodes - 1; i++) {									// Iterate over world line segments
			tangentVelocity = normalize(worldLineNodes[i + 1] - worldLineNodes[i]) * speedOfLight;
			// Start position of tangential geodetic world line of world line of this vertex in absolute frame:
			if (doLorentz) {			// (Length contraction is applied)
				vec3 v = To3DVelocity(tangentVelocity);
				float gamma = lorentzFactor(length(v));
				vec3 n = normalize(v);
				vec3 pParalel = dot(vp.xyz, n) * n;
				vec3 pPerpend = vp.xyz - pParalel;
				offsetedStartPos = vec4(pPerpend + pParalel / gamma, 0) + worldLineNodes[i] - tangentVelocity / tangentVelocity.w * worldLineNodes[i].w;
			}
			else {		// Don't do Lorentz
				offsetedStartPos = vp + worldLineNodes[i] - tangentVelocity / tangentVelocity.w * worldLineNodes[i].w;
			}

			//Intersect:
			if (intersectionMode == 0) {					// Light cone
				vec4 coneLocation = observersLocation;
				t = GeodeticIntersectLightCone(offsetedStartPos, tangentVelocity, coneLocation);
			}
			else if (intersectionMode == 1) {				// Simultaneous hyperplane of the observer
				vec4 planeLocation = observersLocation;
				vec4 planeNormal = normalize(vec4(-(observersVelocity.xyz), observersVelocity.w));
				t = GeodeticIntersectHyperplane(offsetedStartPos, tangentVelocity, planeLocation, planeNormal);		
			}

			if (
            (i == 0 && t < worldLineNodes[1].w)															// First section
            || (i < noOfWorldLineNodes - 2 && t < worldLineNodes[i + 1].w && t >= worldLineNodes[i].w)		// Middle section
            || (i == noOfWorldLineNodes - 2 && t >= worldLineNodes[i].w)											// Last section
            ) {
	            break;
		    }
		}
		
		vec3 vertexLocationProperFrame;
		vec3 vertexVelocityProperFrame;
		if (doLorentz) {
			vec4 shiftedOrigoFrame = GeodeticLocationAtAbsoluteTime(offsetedStartPos, tangentVelocity, t) - observersStartPos;	// The absolute observers frame, but with shifted origo.
			vertexLocationProperFrame = lorentzTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity)).xyz;	// In the current observer's frame.
			vertexVelocityProperFrame = lorentzTransformationOfVelocity(To3DVelocity(GeodeticVelocityAtAbsoluteTime(offsetedStartPos, tangentVelocity, t)), To3DVelocity(observersVelocity));
		}
		else {
			vertexLocationProperFrame = GeodeticLocationAtAbsoluteTime(offsetedStartPos, tangentVelocity, t).xyz - observersLocation.xyz;	// Euclidean transformation
			vertexVelocityProperFrame = To3DVelocity(GeodeticVelocityAtAbsoluteTime(offsetedStartPos, tangentVelocity, t)) - To3DVelocity(observersVelocity);	// Euclidean transformation
		}
		
		if (dopplerMode == 0) {	// Full
			dopplerShift = calculateDopplerShift(vertexVelocityProperFrame, vertexLocationProperFrame);
		}
		else if (dopplerMode == 1) {	// Mild
			dopplerShift = calculateDopplerShift(vertexVelocityProperFrame, vertexLocationProperFrame);
			dopplerShift = (dopplerShift - 1) * 0.2 + 1;
			if (dopplerShift > 1.6) {
				dopplerShift = 1.6;
			}
		}
		else if (dopplerMode == 2) {	// Off
			dopplerShift = 1.0f;			
		}

		wPos = vertexLocationProperFrame;
		texCoord = vec2(uv.x, 1 - uv.y);
		norm = (invM * vec4(vn, 0)).xyz;
		gl_Position = vec4(vertexLocationProperFrame, 1) * MVP;			// Now the MVP doesn't contain translation by -eye! Because eye is in (0,0,0)
	}

	void diagram() {
		vec4 transformed;
		if (transformToProperFrame) {
			if (doLorentz) {
				vec4 shiftedOrigoFrame = vp - observersStartPos;	// The absolute observers frame, but with shifted origo.
				transformed = lorentzTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity));	// In the current observer's frame.
			}
			else {
				transformed = vp - observersStartPos;	// Euclidean transformation
			}
		}
		else {		// "Transformed" is the original.
			transformed = vp;
		}

		wPos = (vec4(transformed[diagramX], transformed[diagramY], transformed[diagramZ], 1) * M).xyz;
		texCoord = vec2(uv.x, 1 - uv.y);
		norm = (invM * vec4(vn, 0)).xyz;
		dopplerShift = 1.0f;			
		gl_Position = vec4(wPos, 1) * MVP;		// Now the MVP should contain the translation to -eye!
	}

	void directRender() {					// Classic render mode, without any relativistic transformation. Only MVP is applied.
		wPos = (vec4(vp.xyz, 1) * M).xyz;
		texCoord = vec2(uv.x, 1 - uv.y);
		norm = (invM * vec4(vn, 0)).xyz;
		dopplerShift = 1.0f;			
		gl_Position = vec4(vp.xyz, 1) * MVP;		// Now the MVP should contain the translation to -eye!
	}

	void main() {
		if (viewMode == 0 && !directRenderMode) {	// RealTime3D
			if (worldLineType == 0) {
				geodetic();
			}
		}
		else if (viewMode == 1 && !directRenderMode) {	// Diagram
			diagram();
		}
		else if (directRenderMode) {
			directRender();
		}
	}
)";

const char* const fragmentSource = R"(
	#version 330			// Shader 3.3
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
)";


