#include "WorldLineView.h"

void GeodeticLineView::Draw()
{
}

void GeodeticLineView::DrawDiagram()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds4D);

}

void GeodeticLineView::drawEditorInfo(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
    (**locationAtZeroTCaption).setVisible(false);
}

void GeodeticLineView::drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
    vec3 pos = vec3(locationAtZeroT[settings.diagramX], locationAtZeroT[settings.diagramY], locationAtZeroT[settings.diagramZ]);
    (**locationAtZeroTCaption).setPos(pos);
    std::string str = std::string("(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")");
    (**locationAtZeroTCaption).setVisible(true);
    (**locationAtZeroTCaption).changeText(str.c_str());
    (**locationAtZeroTCaption).DrawDiagram(gpuProgram, camera);
}

void GeodeticLineView::update()
{
    locationAtZeroT = ((GeodeticLine*)model)->getLocationAtZeroT();
    fourVelocity = ((GeodeticLine*)model)->getVelocity();
}

void CompositeLineView::Draw()
{
}

void CompositeLineView::DrawDiagram()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds4D);
}

void CompositeLineView::drawEditorInfo(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
}

void CompositeLineView::drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const Settings& settings)
{
}

void CompositeLineView::update()
{
}

void WorldLineView::updateGeometry()
{
    noOfVds4D = model->getNoOfVds();
    vds4D = model->getVds();

    // OpenGL:
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, noOfVds4D * sizeof(vec4), &vds4D[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
}

WorldLineView::WorldLineView(const WorldLine* _model) : model(_model) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

WorldLineView::~WorldLineView() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
