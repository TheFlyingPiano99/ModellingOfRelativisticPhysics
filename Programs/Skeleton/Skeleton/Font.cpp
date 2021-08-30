#include "Font.h"

void Font::getCharUVs(const char c, vec2& topLeft, vec2& topRight, vec2& bottomRight, vec2& bottomLeft)
{
    int dec = (int)c;   // Decimal representation of ascii char.
    int row = ((dec <= 95) ? dec - 1 : dec) / noOfColumns;     // int division
    int column = ((dec <= 95)? dec - 1 : dec) % noOfColumns;    //<- dirty hack to compensate for extra space in image.

    vec2 sizeScaler(1.0f / (float)(noOfColumns * charW), 1.0f / (float)(noOfRows * charH));

    topLeft = vec2(column * charW, row * charH) * sizeScaler;
    topRight = vec2(column * charW + charW, row * charH) * sizeScaler;
    bottomRight = vec2(column * charW + charW, row * charH + charH) * sizeScaler;
    bottomLeft = vec2(column * charW, row * charH + charH) * sizeScaler;
}

void Font::loadOnGPU(GPUProgram& gpuProgram)
{
    std::cout << "Loading font on GPU." << std::endl;
    int id = bitmap->getID();
    gpuProgram.setUniform(true, "doLorentz");
    gpuProgram.setUniform(*bitmap, "textureUnit");
    gpuProgram.setUniform(transparentColor, "transparentColor");
}
