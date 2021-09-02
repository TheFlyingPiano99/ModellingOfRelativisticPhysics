#pragma once
#include "framework.h"
#include "Camera.h"
#include "MessageQueue.h"
#include "TextEntry.h"
#include <memory>

/*
* Wrapper of function
*/
void pushCaptionHandler(std::shared_ptr<Caption*> captionToPush);

/*
* Wrapper of function
*/
void ereaseCaptionHandler(std::shared_ptr<Caption*> captionToRemove);

/*
* Represents a "Head-up Display" type of visualisation of captions and other visual help.
* Follows singleton pattern.
*/
class HUD
{
	static HUD* instance;	// singleton
	void* owner;			// The Scene
	std::vector<std::shared_ptr<Caption*>> captions;	// Every caption present in scene.
	MessageQueue* messageQueue = NULL;
	TextEntry* entry = NULL;							// Created, when needed.


public:

	HUD(void* _owner) : owner(_owner), messageQueue(new MessageQueue(this, vec3(0.6f, 0.95f, 0))) {
		if (instance != nullptr) {
			delete instance;
		}
		instance = this;
		Caption::setPushCaptionFunction(pushCaptionHandler);
		Caption::setEreaseCaptionFunction(ereaseCaptionHandler);
	}

	~HUD() {
		delete messageQueue;
		if (entry != nullptr) {
			delete entry;
		}
	}

	static HUD* getInstance() {
		return instance;
	}

	/*
	* Responsible for movement and fading of captions.
	*/
	void Animate(float dt);

	/*
	* DrawDiagram every caption and other in RealTime3D view mode.
	*/
	void Draw(GPUProgram& gpuProgram, Camera& camera);

	/*
	* DrawDiagram every caption and other in Diagram view mode.
	*/
	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);

	/*
	* Push a new caption in the message queue.
	*/
	void pushMessage(const char* text);

	/*
	* Push a new caption. Used only through wrapped function pointer by Caption constructor.
	*/
	void pushCaption(std::shared_ptr<Caption*> captionToPush);

	/*
	* Erease a caption. Used only through wrapped function pointer by Caption destructor.
	*/
	void ereaseCaption(std::shared_ptr<Caption*> captionToRemove);

	/*
	* Initialise a new TextEntry.
	*/
	void createTextEntry(const char* tytle, void (*handler)(const char*));

	/*
	* Forward typed char to the text entry.
	*/
	bool type(char c);

	/*
	* Remove all captions.
	*/
	void clearCaptions();
};

