#pragma once

#include "framework.h"
#include "Color.h"
#include "Hyperplane.h"
#include "WorldLine.h"

class Geometry {
protected:
	unsigned int vao, vbo;

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

	virtual void Draw(GPUProgram& gpuProgram) = 0;

	virtual void updateBeforeDraw(
		vec4 observersVelocity,
		vec4 observersLocation,
		WorldLine& subjectsLine
		) = 0;

};

class MeshSurface : public Geometry {
protected:
	struct VertexData {
		vec3 pos, norm;
		vec2 uv;
	};
	std::vector<VertexData> vds;

public:

	virtual void updateBeforeDraw(
		vec4 observersVelocity,
		vec4 observersLocation,
		WorldLine& subjectsLine) {

		std::vector<VertexData> transformedVds(vds.size());
		for (int i = 0; i < transformedVds.size(); i++)
		{
			WorldLine* offsettedLine = subjectsLine.getWorldLineWithOffset(vds[i].pos);
			Hyperplane observersPlane = Hyperplane::simultaneousHyperplane(observersLocation, observersVelocity);
			float t = offsettedLine->intersectHyperplane(observersPlane);

			vec4 vertexLocation = offsettedLine->getLocationAtAbsoluteTime(t);
			vec4 vertexVelocity = offsettedLine->getVelocityAtAbsoluteTime(t);

			float vertexDopplerShift = calculateDopplerShift(
				observersVelocity,
				vertexVelocity,
				observersLocation,
				vertexLocation);
			transformedVds[i].norm = vds[i].norm;
			transformedVds[i].pos = vec3(vertexLocation.x, vertexLocation.y, vertexLocation.z);
			transformedVds[i].uv = vds[i].uv;
			delete offsettedLine;
		}

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, transformedVds.size() * sizeof(VertexData), &transformedVds[0], GL_DYNAMIC_DRAW);
	}


};

class ParamSurface : public MeshSurface {
	unsigned int nVtxStrip, nStrip;
	bool depthShading = false;

public:

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
	}

	virtual void Draw(GPUProgram& gpuProgram) {
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


	//virtual void Draw(GPUProgram& gpuProgram) {
	//	glBindVertexArray(vao);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	//gpuProgram.setUniform(depthShading, "depthShading");
	//	glLineWidth(5);
	//	glPointSize(10);
	//	glDrawArrays(GL_TRI, 0, vds.size());
	//	glLineWidth(1);
	//}
};