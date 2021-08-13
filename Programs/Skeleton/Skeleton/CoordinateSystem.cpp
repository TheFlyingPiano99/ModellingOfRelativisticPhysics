#include "CoordinateSystem.h"

void CoordinateSystem::Draw(GPUProgram& gpuProgram, Camera& camera)
{
    gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
    //gpuProgram.setUniform(M(), "M");
    gpuProgram.setUniform(UnitMatrix(), "invM");
    gpuProgram.setUniform(true, "glow");
    gpuProgram.setUniform(true, "noTexture");
    gpuProgram.setUniform(color, "kd");

    glBindVertexArray(vao);
    glLineWidth(2);

    glBindBuffer(GL_ARRAY_BUFFER, vboX);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);

    glBindBuffer(GL_ARRAY_BUFFER, vboY);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);
 
    glBindBuffer(GL_ARRAY_BUFFER, vboZ);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);
}
