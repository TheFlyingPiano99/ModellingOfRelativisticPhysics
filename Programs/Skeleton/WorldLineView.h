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
	virtual ~WorldLineView();

	virtual void Draw() = 0;
	virtual void DrawDiagram() = 0;
	virtual void disableEditorInfo(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) = 0;
	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) = 0;
	virtual void update() = 0;
	void updateGeometry();

};


class GeodeticLineView : public WorldLineView {
	std::shared_ptr<Caption*> locationAtZeroTCaption;
	std::shared_ptr<Caption*> velocityCaption;
	vec4 locationAtZeroT, fourVelocity;

public:
	GeodeticLineView(const GeodeticLine* line): WorldLineView(line) {
		update();
		updateGeometry();
		locationAtZeroTCaption = Caption::createSmallCaption(vec3(), "");
		velocityCaption = Caption::createSmallCaption(vec3(), "");
	}

	~GeodeticLineView() {
		(**locationAtZeroTCaption).erease();
		(**velocityCaption).erease();
	}

	// Inherited via WorldLineView
	virtual void Draw() override;

	virtual void DrawDiagram() override;

	virtual void disableEditorInfo(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) override;

	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) override;

	virtual void update() override;

};

class CompositeLineView : public WorldLineView {
	std::vector<vec4> controlPoints;
	std::vector<std::shared_ptr<Caption*>> pointCaptions;
	std::vector<std::shared_ptr<Caption*>> velocityCaptions;

public:
	CompositeLineView(const CompositeLine* line) : WorldLineView(line) {
		update();
		updateGeometry();
	}

	~CompositeLineView() {
		for (int i = 0; i < controlPoints.size(); i++) {
			(**pointCaptions[i]).erease();
		}
		for (int i = 0; i < controlPoints.size() - 1; i++) {
			(**velocityCaptions[i]).erease();
		}
	}

	// Inherited via WorldLineView
	virtual void Draw() override;

	virtual void DrawDiagram() override;

	virtual void disableEditorInfo(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) override;

	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) override;

	virtual void update() override;
};

class SpiralLineView : public WorldLineView {
	std::shared_ptr<Caption*> locationAtZeroTCaption;
	std::shared_ptr<Caption*> velocityCaption;
	vec4 locationAtZeroT, fourVelocity;

public:
	SpiralLineView(const SpiralLine* line) : WorldLineView(line) {
		update();
		updateGeometry();
		locationAtZeroTCaption = Caption::createSmallCaption(vec3(), "");
		velocityCaption = Caption::createSmallCaption(vec3(), "");
	}

	~SpiralLineView() {
		(**locationAtZeroTCaption).erease();
		(**velocityCaption).erease();
	}

	// Inherited via WorldLineView
	virtual void Draw() override;

	virtual void DrawDiagram() override;

	virtual void disableEditorInfo(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) override
	{
		(**locationAtZeroTCaption).setVisible(false);
		(**velocityCaption).setVisible(false);
	}

	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, Camera& camera, const RelTypes::Settings& settings) override;

	virtual void update() override;

};
