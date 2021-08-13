#pragma once

#include "framework.h"
#include "Material.h"
#include "Camera.h"

class CoordinateSystem {
	unsigned int vao, vboX, vboY, vboZ, noOfVds;
	vec3 origo = vec3(0, 0, 0);
	vec3 xBase = vec3(1, 0, 0);
	vec3 yBase = vec3(0, 1, 0);
	vec3 zBase = vec3(0, 0, 1);
	vec3 color = vec3(0,1,0);

public:
	CoordinateSystem() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		// X axis:
		glGenBuffers(1, &vboX);
		glBindBuffer(GL_ARRAY_BUFFER, vboX);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		std::vector<vec3> vds;
		for (int i = 0; i <= 20; i++) {
			vds.push_back(origo + xBase * (i * 10 - 100));
		}
		glBufferData(GL_ARRAY_BUFFER, noOfVds * sizeof(vec3), &vds[0], GL_STATIC_DRAW);
		vds.clear();

		// Y axis:
		glGenBuffers(1, &vboY);
		glBindBuffer(GL_ARRAY_BUFFER, vboY);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		for (int i = 0; i <= 20; i++) {
			vds.push_back(origo + yBase * (i * 10 - 100));
		}
		glBufferData(GL_ARRAY_BUFFER, noOfVds * sizeof(vec3), &vds[0], GL_STATIC_DRAW);
		vds.clear();

		// Z axis:
		glGenBuffers(1, &vboZ);
		glBindBuffer(GL_ARRAY_BUFFER, vboZ);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		for (int i = 0; i <= 20; i++) {
			vds.push_back(origo + zBase * (i * 10 - 100));
		}
		glBufferData(GL_ARRAY_BUFFER, noOfVds * sizeof(vec3), &vds[0], GL_STATIC_DRAW);
	}

	~CoordinateSystem() {
		glDeleteBuffers(1, &vboX);
		glDeleteBuffers(1, &vboY);
		glDeleteBuffers(1, &vboZ);
		glDeleteVertexArrays(1, &vao);
	}


	void Draw(GPUProgram& gpuProgram, Camera& camera);

};