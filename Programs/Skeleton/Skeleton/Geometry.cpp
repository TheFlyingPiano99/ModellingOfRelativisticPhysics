#include "Geometry.h"

#include "StringOperations.h"

inline void OBJGeometry::load(const char* route) {
	std::ifstream file;
	file.open(route);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			std::vector<std::string> words;
			words = split(line, ' ');
			if (words.at(0).at(0) == '#') {
				continue;
			}
			//Todo
		}
	}
	else {
		throw CannotLoadGeometry();
	}

	file.close();

	//OpenGL:
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

inline void OBJGeometry::Draw() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//gpuProgram.setUniform(depthShading, "depthShading");
	for (int i = 0; i < nStrip; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * nVtxStrip, nVtxStrip);
	}
}
