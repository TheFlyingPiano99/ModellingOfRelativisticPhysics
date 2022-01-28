#pragma once

#include "Material.h"
#include "Camera.h"
#include "Caption.h"
#include <string>
#include <memory>

class CoordinateSystem {
	unsigned int vao[3];
	unsigned int vbo[3];
	unsigned int noOfVds;
	vec3 origo = vec3(0, 0, 0);
	vec3 base[3];
	vec3 color[3];
	std::string name[3];
	std::shared_ptr<Caption*> axisName[3];
	std::shared_ptr<Caption*> negAxisName[3];
	std::vector<std::shared_ptr<Caption*>> axisScale[3];

	void drawAxis(GPUProgram& gpuProgram, const Camera& camera, const unsigned int idx, const vec3 center);
	void drawGrid(GPUProgram& gpuProgram, const Camera& camera, const unsigned int idx0, const unsigned int idx1, vec3 center);
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

		int nameDist = 50;
		for (int i = -20; i <= 20; i++) {
			if (i * 5 != nameDist) {
				axisScale[0].push_back(Caption::createSmallCaption(base[0] * i * 5, std::to_string(i * 5).c_str(), vec3(0.4f, 0.4f, 0.4f)));
				axisScale[1].push_back(Caption::createSmallCaption(base[1] * i * 5, std::to_string(i * 5).c_str(), vec3(0.4f, 0.4f, 0.4f)));
				axisScale[2].push_back(Caption::createSmallCaption(base[2] * i * 5, std::to_string(i * 5).c_str(), vec3(0.4f, 0.4f, 0.4f)));
			}
		}

		for (int i = 0; i < 3; i++) {
			axisName[i] = Caption::createNormalCaption(base[i] * nameDist, name[i].c_str());
			(*axisName[i])->setColor(color[i] + vec3(0.5f, 0.5f, 0.5f));

			negAxisName[i] = Caption::createNormalCaption(base[i] * -nameDist, std::string("-").append(name[i]).c_str());
			(*negAxisName[i])->setColor(color[i] + vec3(0.5f, 0.5f, 0.5f));
		}

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

		for (int i = 0; i < 3; i++) {
			(*axisName[i])->erease();
			(*negAxisName[i])->erease();
		}
	}

	void draw(GPUProgram& gpuProgram, const Camera& camera);
	void drawDiagram(GPUProgram& gpuProgram, const Camera& camera);
};