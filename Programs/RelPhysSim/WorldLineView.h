#pragma once

#include "WorldLine.h"
#include "Caption.h"

class WorldLineView
{
public:
	WorldLineView(const WorldLine* _model);
	virtual ~WorldLineView();

	virtual void draw(GPUProgram& gpuProgram) const = 0;
	virtual void drawDiagram(GPUProgram& gpuProgram) const = 0;
	virtual void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const = 0;
	virtual void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const = 0;
	virtual void update() = 0;
	void updateGeometry();
	vec3 getColor();
	void setColor(const vec3 color);

protected:
	unsigned int vao, vbo;
	const WorldLine* model;
	std::vector<vec4> vds4D;
	unsigned int noOfVds4D;
	vec3 color;
};


class GeodeticLineView : public WorldLineView {
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
	void draw(GPUProgram& gpuProgram) const override;

	void drawDiagram(GPUProgram& gpuProgram) const override;

	void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void update() override;
private:
	std::shared_ptr<Caption*> locationAtZeroTCaption;
	std::shared_ptr<Caption*> velocityCaption;
	vec4 locationAtZeroT, fourVelocity;
};

class CompositeLineView : public WorldLineView {

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
	void draw(GPUProgram& gpuProgram) const override;

	void drawDiagram(GPUProgram& gpuProgram) const override;

	void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void update() override;
private:
	std::vector<vec4> controlPoints;
	std::vector<std::shared_ptr<Caption*>> pointCaptions;
	std::vector<std::shared_ptr<Caption*>> velocityCaptions;
};

class SpiralLineView : public WorldLineView {

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
	void draw(GPUProgram& gpuProgram) const override;

	void drawDiagram(GPUProgram& gpuProgram) const override;

	void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override
	{
		(**locationAtZeroTCaption).setVisible(false);
		(**velocityCaption).setVisible(false);
	}

	void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void update() override;

private:
	std::shared_ptr<Caption*> locationAtZeroTCaption;
	std::shared_ptr<Caption*> velocityCaption;
	vec4 locationAtZeroT, fourVelocity;

};

class FreeLineView : public WorldLineView {

public:
	FreeLineView(const FreeWordLine* line) : WorldLineView(line) {
		update();
		updateGeometry();
	}

	~FreeLineView() {
	}

	// Inherited via WorldLineView
	void draw(GPUProgram& gpuProgram) const override;

	void drawDiagram(GPUProgram & gpuProgram) const override;

	void disableEditorInfo(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void drawEditorInfoDiagram(GPUProgram& gpuProgram, const Camera& camera, const RelTypes::Settings& settings) const override;

	void update() override;

};