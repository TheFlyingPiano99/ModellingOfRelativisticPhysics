#include "WorldLineView.h"

#include "RelPhysics.h"

void GeodeticLineView::draw() const
{
}

void GeodeticLineView::drawDiagram() const
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds4D);
}

void GeodeticLineView::disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const
{
    (**locationAtZeroTCaption).setVisible(false);
    (**velocityCaption).setVisible(false);
}

void GeodeticLineView::drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const
{
    vec3 pos = vec3(locationAtZeroT[settings.diagramX], locationAtZeroT[settings.diagramY], locationAtZeroT[settings.diagramZ]);
    (**locationAtZeroTCaption).setPos(pos);
    std::string str = std::string("(").append(std::to_string(pos.x)).append(", ").append(std::to_string(pos.y)).append(", ").append(std::to_string(pos.z)).append(")");
    (**locationAtZeroTCaption).setVisible(true);
    (**locationAtZeroTCaption).changeText(str.c_str());
    (**locationAtZeroTCaption).draw(gpuProgram, camera);


    pos = vec3(locationAtZeroT[settings.diagramX], locationAtZeroT[settings.diagramY], locationAtZeroT[settings.diagramZ]) + vec3(fourVelocity[settings.diagramX], fourVelocity[settings.diagramY], fourVelocity[settings.diagramZ]) * 10.0f;
    (**velocityCaption).setPos(pos);
    str = std::string("v = ").append(std::to_string(length(RelPhysics::To3DVelocity(fourVelocity)))).append("*c m/m");
    (**velocityCaption).changeText(str.c_str());
    (**velocityCaption).setVisible(true);
}
void GeodeticLineView::update()
{
    locationAtZeroT = ((GeodeticLine*)model)->getLocationAtZeroT();
    fourVelocity = ((GeodeticLine*)model)->getVelocity();
}

void CompositeLineView::draw() const
{
}

void CompositeLineView::drawDiagram() const
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds4D);
}

void CompositeLineView::disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const
{
    for (int i = 0; i < controlPoints.size(); i++) {
        (**pointCaptions[i]).setVisible(false);
    }
    for (int i = 0; i < controlPoints.size() - 1; i++) {
        (**velocityCaptions[i]).setVisible(false);
    }
}

void CompositeLineView::drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const
{
    // ControlPoint captions:
    for (int i = 0; i < controlPoints.size(); i++) {
        vec3 pos = vec3(controlPoints[i][settings.diagramX], controlPoints[i][settings.diagramY], controlPoints[i][settings.diagramZ]);
        (**pointCaptions[i]).setPos(pos);
        std::string str = std::string("(").append(std::to_string(pos.x)).append(", ").append(std::to_string(pos.y)).append(", ").append(std::to_string(pos.z)).append(")");
        (**pointCaptions[i]).changeText(str.c_str());
        (**pointCaptions[i]).setVisible(true);
    }
    // Velocity captions:
    for (int i = 0; i < controlPoints.size() - 1; i++) {
        vec3 pos = vec3(controlPoints[i][settings.diagramX], controlPoints[i][settings.diagramY], controlPoints[i][settings.diagramZ])
            + (vec3(controlPoints[i + 1][settings.diagramX], controlPoints[i + 1][settings.diagramY], controlPoints[i + 1][settings.diagramZ])
                - vec3(controlPoints[i][settings.diagramX], controlPoints[i][settings.diagramY], controlPoints[i][settings.diagramZ])) / 2.0f;
        vec4 velocity = normalize(controlPoints[i + 1] - controlPoints[i]) * RelPhysics::speedOfLight;
        (**velocityCaptions[i]).setPos(pos);
        std::string str = std::string("v = ").append(std::to_string(length(RelPhysics::To3DVelocity(velocity)))).append("*c m/m");
        (**velocityCaptions[i]).changeText(str.c_str());
        (**velocityCaptions[i]).setVisible(true);
    }

}

void CompositeLineView::update()
{
    int prevSize = controlPoints.size();
    controlPoints = ((CompositeLine*)model)->getControlPoints();

    // Different amount of captions is needed, than regenerate all captions:
    if (prevSize != controlPoints.size()) {     
        // ControlPoint captions:
        for (std::shared_ptr<Caption*>& cap : pointCaptions) {
            (**cap).erease();
        }
        pointCaptions.clear();
        for (int i = 0; i < controlPoints.size(); i++) {
            pointCaptions.push_back(Caption::createSmallCaption(vec3(), ""));
        }

        // Velocity captions:
        for (std::shared_ptr<Caption*>& cap : velocityCaptions) {
            (**cap).erease();
        }
        velocityCaptions.clear();
        for (int i = 0; i < controlPoints.size() - 1; i++) {
            velocityCaptions.push_back(Caption::createSmallCaption(vec3(), ""));
        }
    }
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

void SpiralLineView::draw() const
{
}

void SpiralLineView::drawDiagram() const
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds4D);
}

void SpiralLineView::drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const
{
}

void SpiralLineView::update()
{
}
