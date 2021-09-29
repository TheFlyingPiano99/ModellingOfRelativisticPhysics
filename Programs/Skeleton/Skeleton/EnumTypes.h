#pragma once

#include "Interpolatable.h"

enum ObjectType {
	earth,
	dice,
	spaceship,
	none
};

enum IntersectionMode {
	lightCone,
	hyperplane
};

enum ViewMode {
	realTime3D,
	diagram
};

enum DopplerMode {
	full,
	mild,
	off
};

enum ControlEventEnum {
	togglePause,
	toggleObserver,
	toggleViewMode,
	toggleIntersectionMode,
	toggleSelection,
	toggleDopplerEffect,
	toggleShading,
	moveCameraForward,
	moveCameraBackward,
	moveCameraUp,
	moveCameraLeft,
	moveCameraRight,
	zoomIn,
	zoomOut,
	windTime,
	rewindTime,
	toggleLorentz,
	toggleTransformToProperFrame,
	save,
	load
};

struct MouseState {
	bool mouseRightPrevDown = false;
	bool mouseLeftPrevDown = false;
	bool mouseRightDown = false;
	bool mouseLeftDown = false;
};

/*
* All important settings mainly related to rendering.
*/
struct Settings {
	ViewMode viewMode;
	InterpolationControl<IntersectionMode> intersectionMode;
	InterpolationControl<DopplerMode> dopplerMode;
	InterpolationControl<bool> doLorentz;
	InterpolationControl<bool> transformToProperFrame;
	bool doShading;
	bool running;
	int diagramX;
	int diagramY;
	int diagramZ;
	int diagramNotVisualised;
	bool editorMode = false;
	bool simultaneBoost = true;

	void loadOnGPU(GPUProgram& gpuProgram) {
		gpuProgram.setUniform(false, "textMode");
		gpuProgram.setUniform(dopplerMode.get(), "dopplerMode");

		gpuProgram.setUniform(doLorentz.get(), "doLorentz");
		gpuProgram.setUniform(doLorentz.isInterpolating(), "interpolateDoLorentz");
		gpuProgram.setUniform(doLorentz.getFraction(), "tDoLorentz");

		gpuProgram.setUniform(intersectionMode.get(), "intersectionMode");
		gpuProgram.setUniform(intersectionMode.isInterpolating(), "interpolateIntersectionMode");
		gpuProgram.setUniform(intersectionMode.getFraction(), "tIntersectionMode");

		gpuProgram.setUniform(transformToProperFrame.get(), "transformToProperFrame");
		gpuProgram.setUniform(transformToProperFrame.isInterpolating(), "interpolateTransformToProperFrame");
		gpuProgram.setUniform(transformToProperFrame.getFraction(), "tTransformToProperFrame");

		gpuProgram.setUniform(doShading, "doShading");
		gpuProgram.setUniform(viewMode, "viewMode");
		gpuProgram.setUniform(vec3(0.05, 0.05, 0.05), "La");

		gpuProgram.setUniform(diagramX, "diagramX");
		gpuProgram.setUniform(diagramY, "diagramY");
		gpuProgram.setUniform(diagramZ, "diagramZ");

		gpuProgram.setUniform(simultaneBoost, "simultaneBoost");
	}
};

struct ObserverProperties {
	vec4 velocity;
	vec4 location;
	vec4 locationAtZero;
};