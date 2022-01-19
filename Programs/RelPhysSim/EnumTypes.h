#pragma once

#include "Interpolatable.h"
#include "Geometry.h"

namespace RelTypes {
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
		bool displayIntersectable = true;
	};

	struct ObserverProperties {
		vec4 velocity;
		vec4 location;
		vec4 locationAtZero;
	};
}

