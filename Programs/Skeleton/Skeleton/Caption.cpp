#include "Caption.h"

mat4 Caption::M(vec3 norm, vec3 prefUp) {
	vec3 right = normalize(cross(prefUp, norm));
	vec3 up = normalize(cross(norm, right));

	return ScaleMatrix(vec3(1, fontSize, fontSize))
		* mat4(
			-norm.x, -norm.y, -norm.z, 0,
			-right.x, -right.y, -right.z, 0,
			up.x, up.y, up.z, 0,
			0, 0, 0, 1)
		* TranslateMatrix(pos);
}

mat4 Caption::invM(vec3 norm, vec3 prefUp) {
	vec3 right = normalize(cross(prefUp, norm));
	vec3 up = normalize(cross(norm, right));

	return TranslateMatrix(-pos)
		* mat4(
			-norm.x, -right.x, up.x, 0,
			-norm.y, -right.y, up.y, 0,
			-norm.z, -right.z, up.z, 0,
			0, 0, 0, 1)
		* ScaleMatrix(vec3(1, 1 / fontSize, 1 / fontSize));
}

void Caption::Animate()
{
	if (animation != nullptr) {
		//Todo
	}
}

void Caption::Draw(GPUProgram& gpuProgram, Camera& camera)
{	
	gpuProgram.setUniform(true, "textMode");
	gpuProgram.setUniform(color, "kd");
	font->loadOnGPU(gpuProgram);

	vec3 normal = -camera.getLookDir();
	mat4 M = this->M(normal, camera.getPrefUp());
	mat4 invM = this->invM(normal, camera.getPrefUp());
	gpuProgram.setUniform(M * camera.Translate() * camera.V() * camera.P(), "MVP");


	gpuProgram.setUniform(M, "M");
	gpuProgram.setUniform(invM, "invM");

	glBindVertexArray(vao);
	glBindBuffer(GL_VERTEX_ARRAY, vbo);

	glDrawArrays(GL_TRIANGLES, 0, noOfVds);

	gpuProgram.setUniform(false, "textMode");
}


void Caption::genGeometry()
{
	// X front, Z up, right handed system:
	vec3 charPos;
	vec3 norm = vec3(-1, 0, 0);		// Normal vector
	vec3 up = vec3(0, 0, 1);
	vec3 right = vec3(0, -1, 0);
	//UVs:	(named in viewers orientation)
	vec2 topLeftUV, topRightUV, bottomLeftUV, bottomRightUV;	

	std::vector<VertexData> vds;

	for (int i = 0; i < text.length(); i++)
	{
		font->getCharUVs(text[i], topLeftUV, topRightUV, bottomRightUV, bottomLeftUV);
		charPos = right * (i - text.length() / 2.0f);		// Pos is in the center of the string.

		// (named in viewers orientation)
		VertexData bottomLeft;
		bottomLeft.pos = charPos + vec3(0, 0.5f, -0.5f);
		bottomLeft.norm = norm;
		bottomLeft.uv = bottomLeftUV;
		VertexData bottomRight;
		bottomRight.pos = charPos + vec3(0, -0.5f, -0.5f);
		bottomRight.norm = norm;
		bottomRight.uv = bottomRightUV;
		VertexData topRight;
		topRight.pos = charPos + vec3(0, -0.5f, 0.5f);
		topRight.norm = norm;
		topRight.uv = topRightUV;
		VertexData topLeft;
		topLeft.pos = charPos + vec3(0, 0.5f, 0.5f);
		topLeft.norm = norm;
		topLeft.uv = topLeftUV;

		// Top left triangle in right handed order:
		vds.push_back(bottomLeft);
		vds.push_back(topRight);
		vds.push_back(topLeft);
		// Bottom right triangle in right handed order:
		vds.push_back(bottomLeft);
		vds.push_back(bottomRight);
		vds.push_back(topRight);
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vds.size() * sizeof(VertexData), &vds[0], GL_STATIC_DRAW);
	noOfVds = vds.size();
}

void Caption::changeText(const char* str)
{
	text = std::string(str);
	genGeometry();
}
