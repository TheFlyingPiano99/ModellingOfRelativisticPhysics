#pragma once

#include "framework.h"
#include "Color.h"
#include "WorldLine.h"
#include <fstream>

struct VertexData {
	vec3 pos, norm;
	vec2 uv;
};

class Geometry {
protected:
	unsigned int vao, vbo;
	float overallRadius = 0;

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

	virtual void Draw() = 0;

	float getOverallRadius() {
		return overallRadius;
	}

};



class ParamSurface : public Geometry {
	unsigned int nVtxStrip, nStrip;
	bool depthShading = false;
	std::vector<VertexData> vds;

public:

	virtual void Eval(float u, float v, vec3& pos, vec3& norm) = 0;


	virtual VertexData getVertexData(float u, float v) {
		VertexData vd;
		Eval(u, v, vd.pos, vd.norm);
		vd.uv = vec2(u, v);
		if (length(vd.pos) > overallRadius) {
			overallRadius = length(vd.pos);
		}
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

	virtual void Draw() {
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

class PlaneSurface : public ParamSurface {
	vec3 normal;
	float width, height;
public:
	PlaneSurface(vec3 _n, float _width, float _height) :normal(_n), width(_width), height(_height) {
	}

	void Eval(float u, float v, vec3& pos, vec3& norm) {
		norm = normal;
		vec3 prefX;
		if (normal.x != 1 && normal.y != 0 && normal.z != 0) {
			prefX = vec3(1, 0, 0);
		}
		else {
			prefX = vec3(0, 1, 0);
		}
		vec3 yBase = cross(normal, prefX);
		vec3 xBase = cross(yBase, normal);

		pos = xBase * (width * u - width / 2.0f) + yBase * (height * v - height / 2.0f);
	}
};

class ConeSurface : public ParamSurface {
	float height = 50;
public:

	void Eval(float u, float v, vec3& pos, vec3& norm) {
		Dnum<vec2> Height = Dnum<vec2>(height, vec2(0, 0));
		Dnum<vec2> U = Dnum<vec2>(u * 2 * M_PI, vec2(1, 0));
		Dnum<vec2> V = Dnum<vec2>(v * 2 * height - height, vec2(0, 1));
		Dnum<vec2> X =  V * Cos(U);
		Dnum<vec2> Y = V * Sin(U);
		Dnum<vec2> Z = V;

		pos = vec3(X.f, Y.f, Z.f);
		norm = normalize(cross(vec3(X.d.x, Y.d.x, Z.d.x), vec3(X.d.y, Y.d.y, Z.d.y)));
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


	//virtual void DrawDiagram(GPUProgram& gpuProgram) {
	//	glBindVertexArray(vao);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	//gpuProgram.setUniform(depthShading, "depthShading");
	//	glLineWidth(5);
	//	glPointSize(10);
	//	glDrawArrays(GL_TRI, 0, vds.size());
	//	glLineWidth(1);
	//}
};

class OBJGeometry : public Geometry {
	unsigned int nVtxStrip, nStrip;
	std::vector<VertexData> vds;
	bool depthShading = false;


public:

	virtual void load(const char* route);

	virtual void Draw();

};
