#pragma once

#include "framework.h"
#include "Material.h"
#include "Camera.h"

class CoordinateSystem {
	unsigned int vao[3];
	unsigned int vbo[3];
	unsigned int noOfVds;
	vec3 origo = vec3(0, 0, 0);
	vec3 base[3];
	vec3 color[3];
	std::string name[3];

	void drawAxis(GPUProgram& gpuProgram, Camera& camera, const unsigned int idx, const vec3 center);
	void drawGrid(GPUProgram& gpuProgram, Camera& camera, const unsigned int idx0, const unsigned int idx1, vec3 center, const float density);
	void genGeometry(vec3 base, unsigned int* vao, unsigned int* vbo);

public:
	CoordinateSystem() {
		base[0] = vec3(1, 0, 0);
		base[1] = vec3(0, 1, 0);
		base[2] = vec3(0, 0, 1);

		color[0] = vec3(1, 0, 0);
		color[1] = vec3(0, 1, 0);
		color[2] = vec3(0, 0, 1);
		
		name[0] = "x";
		name[1] = "y";
		name[2] = "t";

		genGeometry(base[0], &vao[0], &vbo[0]);
		genGeometry(base[1], &vao[1], &vbo[1]);
		genGeometry(base[2], &vao[2], &vbo[2]);
	}

	~CoordinateSystem() {
		glDeleteBuffers(1, &vbo[0]);
		glDeleteBuffers(1, &vbo[1]);
		glDeleteBuffers(1, &vbo[2]);
		glDeleteVertexArrays(1, &vao[0]);
		glDeleteVertexArrays(1, &vao[1]);
		glDeleteVertexArrays(1, &vao[2]);
	}

	void Draw(GPUProgram& gpuProgram, Camera& camera);

};