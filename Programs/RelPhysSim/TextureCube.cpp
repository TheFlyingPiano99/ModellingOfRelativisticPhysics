#include "TextureCube.h"

TextureCube::TextureCube(std::vector<std::string>& images, GLuint unit)
{
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + unit);
	this->unit = unit;
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	
	for (int i = 0; i < 6; i++) {
		std::vector<vec4> imgBytes = load(images[i], false, widthImg, heightImg);
		if (imgBytes.empty()) {
			throw std::exception("Failed to load texture cube!");
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, &imgBytes[0]);
	}

	dimensions.x = widthImg;
	dimensions.y = heightImg;

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void TextureCube::texUnit(GPUProgram& program, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(program.getId(), uniform);
	// Shader needs to be activated before changing the value of a uniform
	program.Use();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void TextureCube::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void TextureCube::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureCube::Delete()
{
	glDeleteTextures(1, &ID);
}

std::vector<vec4> TextureCube::load(std::string pathname, bool transparent, int& width, int& height) {
	FILE* file = fopen(pathname.c_str(), "r");
	if (!file) {
		printf("%s does not exist\n", pathname.c_str());
		width = height = 0;
		return std::vector<vec4>();
	}
	unsigned short bitmapFileHeader[27];					// bitmap header
	fread(&bitmapFileHeader, 27, 2, file);
	if (bitmapFileHeader[0] != 0x4D42) printf("Not bmp file\n");
	if (bitmapFileHeader[14] != 24) printf("Only true color bmp files are supported\n");
	width = bitmapFileHeader[9];
	height = bitmapFileHeader[11];
	unsigned int size = (unsigned long)bitmapFileHeader[17] + (unsigned long)bitmapFileHeader[18] * 65536;
	fseek(file, 54, SEEK_SET);
	std::vector<unsigned char> bImage(size);
	fread(&bImage[0], 1, size, file); 	// read the pixels
	fclose(file);
	std::vector<vec4> image(width * height);
	int i = 0;
	for (unsigned int idx = 0; idx < size; idx += 3) { // Swap R and B since in BMP, the order is BGR
		float alpha = (transparent) ? (bImage[idx] + bImage[idx + 1] + bImage[idx + 2]) / 3.0f / 256.0f : 1.0f;
		image[i++] = vec4(bImage[idx + 2] / 256.0f, bImage[idx + 1] / 256.0f, bImage[idx] / 256.0f, alpha);
	}
	return image;
}
