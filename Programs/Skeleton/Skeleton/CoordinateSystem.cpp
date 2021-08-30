#include "CoordinateSystem.h"

void CoordinateSystem::drawAxis(GPUProgram& gpuProgram, Camera& camera, const unsigned int idx, const vec3 center)
{
    glBindVertexArray(vao[idx]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[idx]);
    mat4 M = TranslateMatrix(center);
    mat4 invM = TranslateMatrix(-center);
    gpuProgram.setUniform(M * camera.Translate() * camera.V() * camera.P(), "MVP");
    gpuProgram.setUniform(M, "M");
    gpuProgram.setUniform(invM, "invM");
    gpuProgram.setUniform(false, "outline");
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);
}

void CoordinateSystem::drawGrid(GPUProgram& gpuProgram, Camera& camera, const unsigned int idx0, const unsigned int idx1, vec3 center, const float density)
{
    glLineWidth(1);
    gpuProgram.setUniform(vec3(0.4f, 0.4f, 0.4f), "kd");
    for (int i = 0; i <= 200 * density; i++) {
        drawAxis(gpuProgram, camera, 0, base[1] * (i / density - 100)  + center);
    }

    for (int i = 0; i <= 200 * density; i++) {
        drawAxis(gpuProgram, camera, 1, base[0] * (i / density - 100) + center);
    }
}

void CoordinateSystem::genGeometry(vec3 base, unsigned int* vao, unsigned int* vbo) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    std::vector<vec3> vds;
    for (int i = 0; i <= 20; i++) {
        vds.push_back(origo + base * (i * 10 - 100));
    }
    noOfVds = vds.size();
    glBufferData(GL_ARRAY_BUFFER, noOfVds * sizeof(vec3), &vds[0], GL_STATIC_DRAW);
}

void CoordinateSystem::Draw(GPUProgram& gpuProgram, Camera& camera)
{
    for (int i = 0; i < 3; i++) {
        axisName[i]->setVisible(false);
        negAxisName[i]->setVisible(false);
    }
}

void CoordinateSystem::DrawDiagram(GPUProgram& gpuProgram, Camera& camera)
{
    gpuProgram.setUniform(UnitMatrix(), "M");
    gpuProgram.setUniform(UnitMatrix(), "invM");
    gpuProgram.setUniform(true, "glow");
    gpuProgram.setUniform(true, "noTexture");
    gpuProgram.setUniform(false, "outline");
    glLineWidth(2);
    gpuProgram.setUniform(color[0], "kd");
    drawAxis(gpuProgram, camera, 0, origo);
    gpuProgram.setUniform(color[1], "kd");
    drawAxis(gpuProgram, camera, 1, origo);
    gpuProgram.setUniform(color[2], "kd");
    drawAxis(gpuProgram, camera, 2, origo);
    drawGrid(gpuProgram, camera, 0, 1, origo, 0.9);
    for (int i = 0; i < 3; i++) {
        axisName[i]->setVisible(true);
        negAxisName[i]->setVisible(true);
    }
}

