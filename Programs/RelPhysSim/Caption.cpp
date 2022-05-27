#include "Caption.h"


void (*Caption::pushCaption)(std::shared_ptr<Caption*>) = NULL;
void (*Caption::ereaseCaption)(std::shared_ptr<Caption*>) = NULL;

mat4 Caption::getModellMatrix(vec2 cPos, vec3 preferedUp, float asp) const {
	vec3 norm = vec3(0, 0, 1);
	vec3 right = normalize(cross(preferedUp, norm));
	vec3 up = normalize(cross(norm, right));

	return ScaleMatrix(vec3(1, fontSize / asp, fontSize))
		* mat4(
			-norm.x, -norm.y, -norm.z, 0,
			-right.x, -right.y, -right.z, 0,
			up.x, up.y, up.z, 0,
			0, 0, 0, 1)
		* TranslateMatrix(vec3(cPos.x, cPos.y, 0));
}

void Caption::animate()
{
	if (animation != nullptr) {
		//Todo
	}
}

void Caption::draw(GPUProgram& gpuProgram, const Camera& camera) const
{	
	if (!visible) {
		return;
	}
	gpuProgram.setUniform(true, "textMode");
	gpuProgram.setUniform(color, "kd");
	gpuProgram.setUniform(true, "directRenderMode");
	fontTexture->loadOnGPU(gpuProgram);
	
	vec2 cPos;
	if (cameraSpace) {
		cPos = vec2(pos.x, pos.y);
	}
	else {
		vec4 cPos4D = vec4(pos.x, pos.y, pos.z, 1) * camera.getTranslationMatrix() * camera.getViewMatrix() * camera.getActiveProjectionMatrix();
		cPos4D = cPos4D / cPos4D.w;
		if (cPos4D.z > 1.0f || cPos4D.z < -1.0f || cPos4D.x > 1.0f || cPos4D.x < -1.0f || cPos4D.y > 1.0f || cPos4D.y < -1.0f) {
			gpuProgram.setUniform(false, "textMode");
			glDepthFunc(GL_LESS);
			return;
		}
		cPos = vec2(cPos4D.x, cPos4D.y);
	}
	mat4 m = this->getModellMatrix(cPos, vec3(0, 1, 0), camera.getAspectRatio());
	gpuProgram.setUniform(m, "MVP");	// In real time 3D space there is no camera traslation to origo in MVP matrix.

	gpuProgram.setUniform(UnitMatrix(), "M");

	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(vao);
	glBindBuffer(GL_VERTEX_ARRAY, vbo);

	glDrawArrays(GL_TRIANGLES, 0, noOfVds);			// <- DrawDiagram call

	//Reset to "normal":
	gpuProgram.setUniform(false, "textMode");
	glDepthFunc(GL_LESS);
}

void Caption::genGeometry()
{
	// X front, Z vUp, vRight handed system:
	vec3 charPos;
	vec3 vNorm = vec3(-1, 0, 0);		// Normal vector
	vec3 up = vec3(0, 0, 1);
	vec3 right = vec3(0, -1, 0);
	//UVs:	(named in viewers orientation)
	vec2 topLeftUV, topRightUV, bottomLeftUV, bottomRightUV;	

	std::vector<VertexData> vds;

	
	int firstLineLength = 0;
	for (int i = 0; i < text.length(); i++) {
		if (text[i] == '\n') {	// New line
			firstLineLength = i;
			break;
		}
	}
	if (firstLineLength == 0 && text.size() > 0) {	// if no end of line char found in text. (The first line is the whole text.)
		firstLineLength = text.size();
	}

	int line = 0;
	int posInLine = 0;
	float lineSpacing = 1.2f;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == '\n') {	// New line
			line++;
			posInLine = 0;
			continue;
		}
		fontTexture->getCharUVs(text[i], topLeftUV, topRightUV, bottomRightUV, bottomLeftUV);
		charPos = right * (posInLine++ - firstLineLength / 2.0f) + line * vec3(0, 0, -lineSpacing);		// Pos is in the center of the string.

		// (named in viewers orientation)
		VertexData bottomLeft;
		vec3 pos3 = charPos + vec3(0, 0.5f, -0.5f);
		bottomLeft.pos = vec4(pos3.x, pos3.y, pos3.z, 0);
		bottomLeft.norm = vNorm;
		bottomLeft.uv = bottomLeftUV;
		VertexData bottomRight;
		pos3 = charPos + vec3(0, -0.5f, -0.5f);
		bottomRight.pos = vec4(pos3.x, pos3.y, pos3.z, 0);
		bottomRight.norm = vNorm;
		bottomRight.uv = bottomRightUV;
		VertexData topRight;
		pos3 = charPos + vec3(0, -0.5f, 0.5f);
		topRight.pos = vec4(pos3.x, pos3.y, pos3.z, 0);
		topRight.norm = vNorm;
		topRight.uv = topRightUV;
		VertexData topLeft;
		pos3 = charPos + vec3(0, 0.5f, 0.5f);
		topLeft.pos = vec4(pos3.x, pos3.y, pos3.z, 0);
		topLeft.norm = vNorm;
		topLeft.uv = topLeftUV;

		// Top left triangle in vRight handed order:
		vds.push_back(bottomLeft);
		vds.push_back(topRight);
		vds.push_back(topLeft);
		// Bottom vRight triangle in vRight handed order:
		vds.push_back(bottomLeft);
		vds.push_back(bottomRight);
		vds.push_back(topRight);
	}

	noOfVds = vds.size();
	if (noOfVds > 0) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vds.size() * sizeof(VertexData), &vds[0], GL_STATIC_DRAW);
	}
}

void Caption::changeText(const char* str)
{
	text = std::string(str);
	genGeometry();
}

