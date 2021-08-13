#pragma once

#include "framework.h"
#include "CoordinateSystem.h"

class View {
protected:
	void* owner;

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
	CoordinateSystem system;
public:
	DiagramView(void* _owner) : View(_owner) {}

	void Draw(GPUProgram& gpuProgram);

};
