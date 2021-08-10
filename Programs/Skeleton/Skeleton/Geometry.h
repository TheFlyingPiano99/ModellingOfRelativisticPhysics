#pragma once

#include "framework.h"
#include "Color.h"
#include "Hyperplane.h"
#include "WorldLine.h"

class Geometry {
protected:
	unsigned int vao, vbo, dopplerVao, dopplerVbo;

public:
	Geometry() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	~Geometry() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	virtual void Draw(GPUProgram& gpuProgram, mat4 M, mat4 V, mat4 P) = 0;

	virtual void updateBeforeDraw(
		vec4 observersVelocity,
		vec4 subjectsVelocity,
		vec4 observersLocation,
		Hyperplane& observersPlane,
		WorldLine& subjectsLine
		) = 0;

};

class ParamSurface : public Geometry {
protected:
	struct VertexData {
		vec3 pos, norm;
		vec2 uv;
		float doppler;
	};

	std::vector<VertexData> vds;
	unsigned int nVtxStrip, nStrip;
	unsigned int vdSize = 0;


	bool depthShading = false;

public:

	virtual void updateBeforeDraw(
		vec4 observersVelocity,
		vec4 subjectsVelocity,
		vec4 observersLocation,
		Hyperplane& observersPlane,
		WorldLine& subjectsLine) {

		std::vector<VertexData> transformedVds(vds.size());
		for (int i = 0; i < transformedVds.size(); i++)
		{
			vec4 offset = vec4(vds[i].pos.x, vds[i].pos.y, vds[i].pos.z, 0);
			WorldLine* offsettedLine = subjectsLine.getWorldLineWithOffset(vds[i].pos);
			vec4 vertexLocation = offsettedLine->intersectHyperplane(observersPlane);
			delete offsettedLine;
			float vertexDopplerShift = calculateDopplerShift(observersVelocity, subjectsVelocity,
				observersLocation, vertexLocation);
			transformedVds[i].doppler = vertexDopplerShift;
			transformedVds[i].norm = vds[i].norm;
			transformedVds[i].pos = vec3(vertexLocation.x, vertexLocation.y, vertexLocation.z);
			transformedVds[i].uv = vds[i].uv;
		}
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, transformedVds.size() * sizeof(VertexData), &transformedVds[0], GL_DYNAMIC_DRAW);

	}


	virtual void Eval(float u, float v, vec3& pos, vec3& norm) = 0;


	virtual VertexData getVertexData(float u, float v) {
		VertexData vd;
		Eval(u, v, vd.pos, vd.norm);
		vd.uv = vec2(u, v);
		return vd;
	}

	virtual void GenSurface(int n, int m) {
		nVtxStrip = (m + 1) * 2;
		nStrip = n;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j <= m; j++) {
				vds.push_back(getVertexData((float)j / m, (float)i / n));
				vds.push_back(getVertexData((float)j / m, (float)(i + 1) / n));
			}
		}

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vds.size() * sizeof(VertexData), &vds[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, norm));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
		
		//Doppler:
		//glBindBuffer(GL_ARRAY_BUFFER, dopplerVbo);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, doppler));
	}

	virtual void Draw(GPUProgram& gpuProgram, mat4 M, mat4 V, mat4 P) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//gpuProgram.setUniform(depthShading, "depthShading");
		for (int i = 0; i < nStrip; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i * nVtxStrip, nVtxStrip);
		}
	}

};


class SphereSurface : public ParamSurface {
	Dnum<vec2> R;
public:
	SphereSurface(float r) {
		R = Dnum<vec2>(r, vec2(0, 0));
	}

	void Eval(float u, float v, vec3& pos, vec3& norm) {
		Dnum<vec2> U = Dnum<vec2>(u * 2 * M_PI, vec2(1, 0));
		Dnum<vec2> V = Dnum<vec2>(v * M_PI, vec2(0, 1));
		Dnum<vec2> X = R * Cos(U) * Sin(V);
		Dnum<vec2> Y = R * Sin(U) * Sin(V);
		Dnum<vec2> Z = R * Cos(V);

		pos = vec3(X.f, Y.f, Z.f);
		norm = -normalize(cross(vec3(X.d.x, Y.d.x, Z.d.x), vec3(X.d.y, Y.d.y, Z.d.y)));
	}
};

class Stars : public ParamSurface {
	Dnum<vec2> R;
public:

	Stars() : ParamSurface() {
		//srand(0);
		R = Dnum<vec2>(1000, vec2(0, 0));
	}

	void Eval(float u, float v, vec3& pos, vec3& norm) {
		Dnum<vec2> U = Dnum<vec2>(u * 2 * M_PI, vec2(1, 0));
		Dnum<vec2> V = Dnum<vec2>(v * M_PI, vec2(0, 1));
		Dnum<vec2> X = R * Cos(U) * Sin(V);
		Dnum<vec2> Y = R * Sin(U) * Sin(V);
		Dnum<vec2> Z = R * Cos(V);

		pos = vec3(X.f, Y.f, Z.f);
		norm = normalize(cross(vec3(X.d.x, Y.d.x, Z.d.x), vec3(X.d.y, Y.d.y, Z.d.y)));
	}


	/*
		// Inherited via ParamSurface
	void Eval(float u, float v, vec3& pos, vec3& norm)
	{
		pos = normalize(vec3(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500));
		norm = -pos;
		pos = pos * 1000.0f;
	}
	*/


	//virtual void Draw(GPUProgram& gpuProgram, mat4 M, mat4 V, mat4 P) {
	//	glBindVertexArray(vao);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	//gpuProgram.setUniform(depthShading, "depthShading");
	//	glLineWidth(5);
	//	glPointSize(10);
	//	glDrawArrays(GL_TRI, 0, vds.size());
	//	glLineWidth(1);
	//}
};