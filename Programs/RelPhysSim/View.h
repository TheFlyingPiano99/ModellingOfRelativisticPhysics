#pragma once

#include "WorldLine.h"

class View {
protected:
	void* owner;	// Must be of type Scene.

public:
	View(void* _owner): owner(_owner) {
	}

	~View() {	// Do not delete owner!
	}


	virtual void transitionFrom() = 0;
	virtual void transitionTo() = 0;
	virtual void draw(GPUProgram& gpuProgram) = 0;
};

class RealTime3DView : public View {

public:
	RealTime3DView(void* _owner): View(_owner) {}

	void draw(GPUProgram& gpuProgram);
	void transitionFrom();
	void transitionTo();
};

class DiagramView : public View {
public:
	DiagramView(void* _owner) : View(_owner) {
	}

	~DiagramView() {
	}

	void draw(GPUProgram& gpuProgram);
	void transitionFrom();
	void transitionTo();
};
