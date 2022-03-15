#include "CoordinateSystem.h"

void CoordinateSystem::drawAxis(GPUProgram& gpuProgram, const Camera& camera, const unsigned int idx, const vec3 center)
{
    glBindVertexArray(vao[idx]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[idx]);
    mat4 m = TranslateMatrix(center);
    mat4 invM = TranslateMatrix(-center);
    gpuProgram.setUniform(m * camera.getTranslationMatrix() * camera.getViewMatrix() * camera.getActiveProjectionMatrix(), "MVP");
    gpuProgram.setUniform(m, "m");
    gpuProgram.setUniform(invM, "invM");
    gpuProgram.setUniform(false, "outline");
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);
}

void CoordinateSystem::drawGrid(GPUProgram& gpuProgram, const Camera& camera, const unsigned int idx0, const unsigned int idx1, vec3 center)
{
    glLineWidth(1);
    gpuProgram.setUniform(vec3(0.4f, 0.4f, 0.4f), "kd");
    for (int i = 0; i <= 40; i++) {
        drawAxis(gpuProgram, camera, 0, base[1] * (i * 5 - 100)  + center);
    }

    for (int i = 0; i <= 40; i++) {
        drawAxis(gpuProgram, camera, 1, base[0] * (i * 5 - 100) + center);
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

void CoordinateSystem::draw(GPUProgram& gpuProgram, const Camera& camera)
{
    for (int i = 0; i < 3; i++) {
        (*axisName[i])->setVisible(false);
        (*negAxisName[i])->setVisible(false);
    }
    for (int i = 0; i < axisScale[0].size(); i++) {
        (*axisScale[0][i].get())->setVisible(false);
        (*axisScale[1][i].get())->setVisible(false);
        (*axisScale[2][i].get())->setVisible(false);
    }
}

void CoordinateSystem::drawDiagram(GPUProgram& gpuProgram, const Camera& camera)
{
    gpuProgram.setUniform(UnitMatrix(), "m");
    gpuProgram.setUniform(UnitMatrix(), "invM");
    gpuProgram.setUniform(true, "glow");
    gpuProgram.setUniform(true, "noTexture");
    gpuProgram.setUniform(false, "outline");
    gpuProgram.setUniform(true, "directRenderMode");
    glLineWidth(2);
    gpuProgram.setUniform(color[0], "kd");
    drawAxis(gpuProgram, camera, 0, origo);
    gpuProgram.setUniform(color[1], "kd");
    drawAxis(gpuProgram, camera, 1, origo);
    gpuProgram.setUniform(color[2], "kd");
    drawAxis(gpuProgram, camera, 2, origo);
    drawGrid(gpuProgram, camera, 0, 1, origo);
    for (int i = 0; i < 3; i++) {
        (*axisName[i])->setVisible(true);
        (*negAxisName[i])->setVisible(true);
    }
    for (int i = 0; i < axisScale[0].size(); i++) {
        (*axisScale[0][i].get())->setVisible(true);
        (*axisScale[1][i].get())->setVisible(true);
        (*axisScale[2][i].get())->setVisible(true);
    }
}

