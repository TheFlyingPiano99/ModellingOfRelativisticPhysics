#pragma once

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

	virtual void draw() const = 0;
	virtual void drawDiagram() const = 0;
	virtual void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const = 0;
	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const = 0;
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
	void draw() const override;

	void drawDiagram() const override;

	void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void update() override;

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
	void draw() const override;

	void drawDiagram() const override;

	void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void update() override;
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
	void draw() const override;

	void drawDiagram() const override;

	void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override
	{
		(**locationAtZeroTCaption).setVisible(false);
		(**velocityCaption).setVisible(false);
	}

	void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void update() override;

};
