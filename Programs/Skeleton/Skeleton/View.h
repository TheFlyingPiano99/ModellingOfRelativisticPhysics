#pragma once

#include "framework.h"
#include "CoordinateSystem.h"
#include "WorldLine.h"

class View {
protected:
	void* owner;	// Must be of type Scene.

public:
	View(void* _owner): owner(_owner) {
	}

	~View() {	// Do not delete owner!
	}

	virtual void Draw(GPUProgram& gpuProgram) = 0;
};

class RealTime3DView : public View {

public:
	RealTime3DView(void* _owner): View(_owner) {}

	void Draw(GPUProgram& gpuProgram);
	
};

class DiagramView : public View {
	CoordinateSystem* system = NULL;
public:
	DiagramView(void* _owner) : View(_owner) {
		system = new CoordinateSystem();
	}

	~DiagramView() {
		delete system;
	}

	void Draw(GPUProgram& gpuProgram);

};
