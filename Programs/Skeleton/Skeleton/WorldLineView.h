#pragma once

#include "framework.h"
#include "WorldLine.h"
#include "Caption.h"

class WorldLineView
{
protected:
	unsigned int vao, vbo;
	const WorldLine* model;
	std::vector<vec4> vds4D;
	unsigned int noOfVds4D;

public:
	WorldLineView(const WorldLine* _model);
	~WorldLineView();

	virtual void Draw() = 0;
	virtual void DrawDiagram() = 0;
	virtual void drawEditorInfo(GPUProgram& gpuProgram, Camera& camera, const Settings& settings) = 0;
	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const Settings& settings) = 0;
	virtual void update() = 0;
	void updateGeometry();

};


class GeodeticLineView : public WorldLineView {
	std::shared_ptr<Caption*> locationAtZeroTCaption;
	vec4 locationAtZeroT, fourVelocity;
public:
	GeodeticLineView(const GeodeticLine* line): WorldLineView(line) {
		update();
		updateGeometry();
	}

	~GeodeticLineView() {
		(**locationAtZeroTCaption).erease();
	}

	// Inherited via WorldLineView
	virtual void Draw() override;

	virtual void DrawDiagram() override;

	virtual void drawEditorInfo(GPUProgram& gpuProgram, Camera& camera, const Settings& settings) override;

	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const Settings& settings) override;

	virtual void update() override;

};

class CompositeLineView : public WorldLineView {
public:
	CompositeLineView(const CompositeLine* line) : WorldLineView(line) {
		update();
		updateGeometry();
	}

	// Inherited via WorldLineView
	virtual void Draw() override;

	virtual void DrawDiagram() override;

	virtual void drawEditorInfo(GPUProgram& gpuProgram, Camera& camera, const Settings& settings) override;

	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const Settings& settings) override;

	virtual void update() override;
};