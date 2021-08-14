#include "CoordinateSystem.h"

void CoordinateSystem::drawAxis(GPUProgram& gpuProgram, const unsigned int idx)
{
    gpuProgram.setUniform(color[idx], "kd");
    glBindVertexArray(vao[idx]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[idx]);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);
}

void CoordinateSystem::Draw(GPUProgram& gpuProgram, Camera& camera)
{
    gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
    gpuProgram.setUniform(UnitMatrix(), "M");
    gpuProgram.setUniform(UnitMatrix(), "invM");
    gpuProgram.setUniform(true, "glow");
    gpuProgram.setUniform(true, "noTexture");
    glLineWidth(2);
    
    drawAxis(gpuProgram, 0);
    drawAxis(gpuProgram, 1);
    drawAxis(gpuProgram, 2);
}
