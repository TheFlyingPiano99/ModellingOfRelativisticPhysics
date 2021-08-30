#pragma once
#include "framework.h"
#include "Camera.h"
#include "Message.h"
#include "TextEntry.h"

void pushCaptionHandler(Caption* captionToPush);
void ereaseCaptionHandler(Caption* captionToRemove);

class HUD
{
	static HUD* instance;
	void* owner;
	std::vector<Caption*> captions;
	MessageQueue* messageQueue = NULL;
	TextEntry* entry = NULL;


public:

	HUD(void* _owner) : owner(_owner), messageQueue(new MessageQueue(this, vec3(0.6f, 0.95f, 0))) {
		if (instance != nullptr) {
			delete instance;
		}
		instance = this;
		Caption::setPushCaptionFunction(pushCaptionHandler);
		Caption::setEreaseCaptionFunction(ereaseCaptionHandler);
	}

	static HUD* getInstance() {
		return instance;
	}

	~HUD() {
		delete messageQueue;
		delete entry;
		for each (Caption * cap in captions)
		{
			delete cap;
		}
	}

	void Animate(float dt);

	void Draw(GPUProgram& gpuProgram, Camera& camera);

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);

	void pushMessage(const char* text);

	void pushCaption(Caption* captionToPush);
	void ereaseCaption(Caption* captionToRemove);

	void createTextEntry(const char* tytle, void (*handler)(const char*));

	bool type(char c);

	void clearCaptions();
};

