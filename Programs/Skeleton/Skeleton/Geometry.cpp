#include "Geometry.h"

#include "StringOperations.h"

void OBJGeometry::load(const char* route) {
	std::ifstream file;
	file.open(route);
	if (file.is_open()) {
		std::vector<vec3> vertices;
		std::vector<vec2> textureCoords;
		std::vector<vec3> normals;

		std::string line;
		while (getline(file, line)) {
			std::vector<std::string> words;
			words = split(line, ' ');
			if (words.at(0).at(0) == '#') {					// Comment
				continue;
			}
			else if (words.at(0).compare("v") == 0) {		// vertex
				vec3 v;
				if (words.size() == 4) {		// 3 component vector	x y z
					v = vec3(std::stof(words.at(1)), std::stof(words.at(2)), std::stof(words.at(3)));
				}
				else if (words.size() == 5) {	// 4 component vector	x y z w
					v = vec3(std::stof(words.at(1)), std::stof(words.at(2)), std::stof(words.at(3)));	// Discard 4. component
				}
				vertices.push_back(v);
			}
			else if (words.at(0).compare("vt") == 0) {		// texture coordinate
				vec2 vt;
				if (words.size() == 2) {		// 1 component vector	u
					vt = vec2(std::stof(words.at(1)), 0.0f);					// v := 0 by default
				}
				else if (words.size() == 3) {	// 2 component vector	u v
					vt = vec2(std::stof(words.at(1)), 1 - std::stof(words.at(2)));
				}
				else if (words.size() == 4) {	// 3 component vector	u v w
					vt = vec2(std::stof(words.at(1)), 1 - std::stof(words.at(2)));	// Discard 3. component
				}
				textureCoords.push_back(vt);
			}
			else if (words.at(0).compare("vn") == 0) {		// normal
				vec3 vn;
				if (words.size() == 4) {		// 1 component vector	u
					vn = vec3(std::stof(words.at(1)), std::stof(words.at(2)), std::stof(words.at(3)));					// v := 0 by default
				}
				normals.push_back(vn);
			}
			else if (words.at(0).compare("f") == 0) {		// polygon face
				VertexData vd;
				std::vector<std::string> vertexDescription;
				for (int i = 1; i <= 3; i++) {				// 3 vertex triangles
					vertexDescription = split(words.at(i), '/');
					if (vertexDescription.size() == 3) {		// vertex / texture / normal
						vd.pos = vertices[std::stoi(vertexDescription[0]) - 1];
						vd.uv = textureCoords[std::stoi(vertexDescription[1]) - 1];
						vd.norm = normals[std::stoi(vertexDescription[2]) - 1];
						vds.push_back(vd);
					}
				}
			}
			else if (words.at(0).compare("l") == 0) {		// line
				;
			}
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

void OBJGeometry::Draw() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//gpuProgram.setUniform(depthShading, "depthShading");
	glDrawArrays(GL_TRIANGLES, 0, vds.size());
}
