
#include "glad/glad.h"
#include "Texture.h"

std::vector<vec4> Texture::load(std::string pathname, bool transparent, int& width, int& height) {
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

void Texture::create(std::string pathname, bool transparent) {
	int width, height;
	std::vector<vec4> image = load(pathname, transparent, width, height);
	if (image.size() > 0) create(width, height, image);
}

void Texture::create(int width, int height, const std::vector<vec4>& image, int sampling) {
	if (textureId == 0) glGenTextures(1, &textureId);  				// id generation
	glBindTexture(GL_TEXTURE_2D, textureId);    // binding

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &image[0]); // To GPU
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling); // sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
}

Texture::~Texture() {
	if (textureId > 0) glDeleteTextures(1, &textureId);
}
