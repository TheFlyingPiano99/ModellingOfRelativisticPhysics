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
	uniform bool interpolateDoLorentz;
	uniform float tDoLorentz;

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
	uniform bool interpolateTransformToProperFrame;
	uniform float tTransformToProperFrame;


	uniform int intersectionMode;	// 0 = lightCone, 1 = hyperplane
	uniform bool interpolateIntersectionMode;
	uniform float tIntersectionMode;

	uniform bool simultaneBoost;

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

	vec4 ToFourVelocity(const vec3 v) {
		return normalize(vec4(v.x, v.y, v.z, 1)) * speedOfLight;
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
		float tTrans = gamma * (t - dot(v, r) / speedOfLight / speedOfLight);
		vec3 rTrans = r + (gamma - 1) * dot(r, n) * n - gamma * t * v;
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

	vec4 galileanTransformation(vec4 toTransform, vec3 relVelocity) {
		vec3 r = toTransform.xyz;
		float t = toTransform.w;
		vec3 v = relVelocity;
		vec3 rTrans = r - v * t;
		float tTrans = t;
		return vec4(rTrans, tTrans);
	}

	/*
	* Calculates coeficient for Doppler shift of perceived color. Given, that observer is positioned in (0,0,0).
	*/
	float calculateDopplerShift(vec3 vertexVelocity, vec3 vertexLocation) {

		vec3 toSubject = normalize(vertexLocation);

		float v = dot(toSubject, vertexVelocity);  //Approach speed
		return sqrt((speedOfLight + v) / (speedOfLight - v));
	}

	vec4 lerp(vec4 startVal, vec4 endVal, float t) {
		return startVal * (1 - t) + endVal * t;
	}

	vec3 lerp(vec3 startVal, vec3 endVal, float t) {
		return startVal * (1 - t) + endVal * t;
	}

	float lerp(float startVal, float endVal, float t) {
		return startVal * (1 - t) + endVal * t;
	}


	/*
		Calculates the intersection of a hyperplane and world line of this vertex
		Also calculates the Doppler shift for this vertex.
	*/
	void realTime() {
		float tLorentz;						// absolute time parametre, where the world line of vertex intersects hyperplane or light cone.
		float tGalilean;					// absolute time parametre, where the world line of vertex intersects hyperplane or light cone.
		vec4 tangentVelocityLorentz;		// Velocity of geodetic world line tangent to the world line, where the hyperplane or light cone intersects it.
		vec4 tangentVelocityGalilean;																		// Velocity of geodetic world line tangent to the world line, where the hyperplane or light cone intersects it.

		vec4 geodeticSectionStart;					// <- To make Wigner rotation happen.
		vec4 geodeticSectionEnd;					// <- To make Wigner rotation happen.

		vec4 offsetedStartPosLorentz;
		vec4 offsetedStartPosGalilean;

		// Find tLorentz:
		for (int i = 0; i < noOfWorldLineNodes - 1; i++) {									// Iterate over world line segments
			tangentVelocityLorentz = normalize(worldLineNodes[i + 1] - worldLineNodes[i]) * speedOfLight;
			vec4 nextTangentVelocityLorentz = tangentVelocityLorentz;
			vec3 v = To3DVelocity(tangentVelocityLorentz);
			float gamma = lorentzFactor(length(v));
			vec3 n = normalize(v);
			vec3 pParalel = dot(vp.xyz, n) * n;
			vec3 pPerpend = vp.xyz - pParalel;
			if (i < noOfWorldLineNodes - 2) {
				nextTangentVelocityLorentz = normalize(worldLineNodes[i + 2] - worldLineNodes[i + 1]) * speedOfLight;
			}
			if (i > 0) {							// Get geodeticSectionStart:
				geodeticSectionStart = geodeticSectionEnd;
			}
			else {			// if i == 0
				geodeticSectionStart = vec4(pPerpend + pParalel / gamma, 0) + worldLineNodes[0] - tangentVelocityLorentz / tangentVelocityLorentz.w * worldLineNodes[0].w;	// Length Contraction(vp) + object worldLine start pos;
			}

			offsetedStartPosLorentz = geodeticSectionStart - tangentVelocityLorentz / tangentVelocityLorentz.w * geodeticSectionStart.w;

			// Get geodeticSectionEnd:
			vec4 boostPlaneLocation = worldLineNodes[i + 1];
			vec4 boostPlaneNormal;
			if (simultaneBoost) {
				boostPlaneNormal = normalize(vec4(-(tangentVelocityLorentz.xyz), tangentVelocityLorentz.w));
			}
			else {
				boostPlaneNormal = vec4(0, 0, 0, 1);
			}
			float tSectionEnd = GeodeticIntersectHyperplane(offsetedStartPosLorentz, tangentVelocityLorentz, boostPlaneLocation, boostPlaneNormal);
			geodeticSectionEnd = GeodeticLocationAtAbsoluteTime(offsetedStartPosLorentz, tangentVelocityLorentz, tSectionEnd);					// <- To make Wigner rotation happen.

			//---------------------------------------------------------------------------------------------------------------------------------
			//Intersect:
			// Cone:
			vec4 coneLocation = observersLocation;
			
			float tConeLorentz = GeodeticIntersectLightCone(offsetedStartPosLorentz, tangentVelocityLorentz, coneLocation);
			
			// Hyperplane:
			vec4 planeLocation = observersLocation;
			vec4 planeNormalLorentz = normalize(vec4(-(observersVelocity.xyz), observersVelocity.w));

			float tPlaneLorentz = GeodeticIntersectHyperplane(offsetedStartPosLorentz, tangentVelocityLorentz, planeLocation, planeNormalLorentz);
			
			if (interpolateIntersectionMode) {
				if (intersectionMode == 0) {					// Light cone
					tLorentz = lerp(tPlaneLorentz, tConeLorentz, tIntersectionMode);
				}
				else if (intersectionMode == 1) {				// Simultaneous hyperplane of the observer
					tLorentz = lerp(tConeLorentz, tPlaneLorentz, tIntersectionMode);
				}
			}
			else {
				if (intersectionMode == 0) {					// Light cone
					tLorentz = tConeLorentz;
				}
				else if (intersectionMode == 1) {				// Simultaneous hyperplane of the observer
					tLorentz = tPlaneLorentz;
				}
			}
			if (
            (i == 0 && tLorentz < geodeticSectionEnd.w)															// First section
            || (i < noOfWorldLineNodes - 2 && tLorentz < geodeticSectionEnd.w && tLorentz >= geodeticSectionStart.w)		// Middle section
            || (i == noOfWorldLineNodes - 2 && tLorentz >= geodeticSectionStart.w)											// Last section
            ) {
	            break;
		    }
		}

//----------------------------------------------------------------------------------------------------------------------------------------------------------

		// Find tGalilean:
		for (int i = 0; i < noOfWorldLineNodes - 1; i++) {									// Iterate over world line segments
			tangentVelocityGalilean = normalize(worldLineNodes[i + 1] - worldLineNodes[i]) * speedOfLight;
			// Start position of tangential geodetic world line of world line of this vertex in absolute frame:
			vec3 v = To3DVelocity(tangentVelocityGalilean);
			float gamma = lorentzFactor(length(v));
			vec3 n = normalize(v);
			vec3 pParalel = dot(vp.xyz, n) * n;
			vec3 pPerpend = vp.xyz - pParalel;
			offsetedStartPosGalilean = vp + worldLineNodes[i] - tangentVelocityGalilean / tangentVelocityGalilean.w * worldLineNodes[i].w;	// vp + tangent worldLine start pos	also no Wigner rotation (not using "sectorStart").

			//---------------------------------------------------------------------------------------------------------------------------------
			//Intersect:
			// Cone:
			vec4 coneLocation = observersLocation;
			
			float tConeGalilean = GeodeticIntersectLightCone(offsetedStartPosGalilean, tangentVelocityGalilean, coneLocation);
			
			// Hyperplane:
			vec4 planeLocation = observersLocation;
			vec4 planeNormalGalilean = vec4(0, 0, 0, 1);

			float tPlaneGalilean = GeodeticIntersectHyperplane(offsetedStartPosGalilean, tangentVelocityGalilean, planeLocation, planeNormalGalilean);
			
			if (interpolateIntersectionMode) {
				if (intersectionMode == 0) {					// Light cone
					tGalilean = lerp(tPlaneGalilean, tConeGalilean, tIntersectionMode);
				}
				else if (intersectionMode == 1) {				// Simultaneous hyperplane of the observer
					tGalilean = lerp(tConeGalilean, tPlaneGalilean, tIntersectionMode);
				}
			}
			else {
				if (intersectionMode == 0) {					// Light cone
					tGalilean = tConeGalilean;
				}
				else if (intersectionMode == 1) {				// Simultaneous hyperplane of the observer
					tGalilean = tPlaneGalilean;
				}
			}

			if (
            (i == 0 && tGalilean < worldLineNodes[1].w)															// First section
            || (i < noOfWorldLineNodes - 2 && tGalilean < worldLineNodes[i + 1].w && tGalilean >= worldLineNodes[i].w)		// Middle section
            || (i == noOfWorldLineNodes - 2 && tGalilean >= worldLineNodes[i].w)											// Last section
            ) {
	            break;
		    }
		}
		
		vec3 vertexLocationProperFrameLorentz;
		vec3 vertexVelocityProperFrameLorentz;

		vec3 vertexLocationProperFrameGalilean;
		vec3 vertexVelocityProperFrameGalilean;

		vec3 vertexLocationProperFrame;
		vec3 vertexVelocityProperFrame;

		// Lorentz:
		vec4 shiftedOrigoFrame = GeodeticLocationAtAbsoluteTime(offsetedStartPosLorentz, tangentVelocityLorentz, tLorentz) - observersStartPos;	// The absolute observers frame, but with shifted origo.
		vertexLocationProperFrameLorentz = lorentzTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity)).xyz;	// In the current observer's frame.
		vertexVelocityProperFrameLorentz = lorentzTransformationOfVelocity(To3DVelocity(GeodeticVelocityAtAbsoluteTime(offsetedStartPosLorentz, tangentVelocityLorentz, tLorentz)), To3DVelocity(observersVelocity));

		// Galilean:
		shiftedOrigoFrame = GeodeticLocationAtAbsoluteTime(offsetedStartPosGalilean, tangentVelocityGalilean, tGalilean) - observersStartPos;	// The absolute observers frame, but with shifted origo.
		vertexLocationProperFrameGalilean = galileanTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity)).xyz;	// In the current observer's frame.
		vertexVelocityProperFrameGalilean = To3DVelocity(GeodeticVelocityAtAbsoluteTime(offsetedStartPosGalilean, tangentVelocityGalilean, tGalilean)) - To3DVelocity(observersVelocity);
		
		if (interpolateDoLorentz) {
			if (doLorentz) {
				vertexLocationProperFrame = lerp(vertexLocationProperFrameGalilean, vertexLocationProperFrameLorentz, tDoLorentz);
				vertexVelocityProperFrame = lerp(vertexVelocityProperFrameGalilean, vertexVelocityProperFrameLorentz, tDoLorentz);
			}
			else {
				vertexLocationProperFrame = lerp(vertexLocationProperFrameLorentz, vertexLocationProperFrameGalilean, tDoLorentz);
				vertexVelocityProperFrame = lerp(vertexVelocityProperFrameLorentz, vertexVelocityProperFrameGalilean, tDoLorentz);
			}
		}
		else {
			if (doLorentz) {
				vertexLocationProperFrame = vertexLocationProperFrameLorentz;
				vertexVelocityProperFrame = vertexVelocityProperFrameLorentz;
			}
			else {
				vertexLocationProperFrame = vertexLocationProperFrameGalilean;
				vertexVelocityProperFrame = vertexVelocityProperFrameGalilean;
			}
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
		vec4 transformedProperFrame;
		vec4 shiftedOrigoFrame = vp - observersStartPos;	// The absolute observers frame, but with shifted origo.
		vec4 transformedLorentz = lorentzTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity));	// In the current observer's frame.

		shiftedOrigoFrame = vp - observersStartPos;	// The absolute observers frame, but with shifted origo.
		vec4 transformedGalilean = galileanTransformation(shiftedOrigoFrame, To3DVelocity(observersVelocity));	// In the current observer's frame.
		if (interpolateDoLorentz) {
			if (doLorentz) {
				transformedProperFrame = lerp(transformedGalilean, transformedLorentz, tDoLorentz);
			}
			else {
				transformedProperFrame = lerp(transformedLorentz, transformedGalilean, tDoLorentz);
			}
		}
		else {
			if (doLorentz) {
				transformedProperFrame = transformedLorentz;
			}
			else {
				transformedProperFrame = transformedGalilean;
			}
		}
		if (interpolateTransformToProperFrame) {
			if (transformToProperFrame) {
				transformed = lerp(vp, transformedProperFrame, tTransformToProperFrame);
			}
			else {
				transformed = lerp(transformedProperFrame, vp, tTransformToProperFrame);
			}
		}
		else {
			if (transformToProperFrame) {
				transformed = transformedProperFrame;
			}
			else {
				transformed = vp;
			}
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
				realTime();
			}
		}
		else if (viewMode == 1 && !directRenderMode) {	// Diagram
			diagram();
		}
		else if (directRenderMode) {
			directRender();
		}
	}
