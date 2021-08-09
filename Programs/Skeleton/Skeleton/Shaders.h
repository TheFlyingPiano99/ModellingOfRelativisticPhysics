#pragma once

const char* const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	layout(location = 0) in vec3 vp;
	layout(location = 1) in vec3 vn;

	uniform mat4 MVP;
	uniform mat4 M;

	out vec3 wPos;
	out vec3 norm;	

	void main() {
		gl_Position = vec4(vp, 1) * MVP;
		wPos = (vec4(vp, 1) * M).xyz;
		norm = vn;
	}
)";

const char* const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers

//Object:
	in vec3 wPos;
	in vec3 norm;

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

	void main() {
		outColor = vec4(kd, 1);	// computed color is the color of the primitive
		//outColor = vec4(kd, 1);	// computed color is the color of the primitive
	}
)";
