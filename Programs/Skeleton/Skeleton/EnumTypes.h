#pragma once

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

enum ControlEvent {
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

/*
* All important settings mainly related to rendering.
*/
struct Settings {
	ViewMode viewMode = ViewMode::realTime3D;
	IntersectionMode intersectionMode = IntersectionMode::lightCone;
	DopplerMode dopplerMode = DopplerMode::mild;
	bool doLorentz = true;
	bool transformToProperFrame = true;
	bool doShading = true;
	bool running = false;
	int diagramX = 0;
	int diagramY = 1;
	int diagramZ = 3;
};

struct ObserverProperties {
	vec4 velocity;
	vec4 location;
	vec4 locationAtZero;
};