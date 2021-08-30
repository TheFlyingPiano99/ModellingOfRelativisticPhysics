#pragma once
#include "framework.h"
#include "Camera.h"
#include "Message.h"
#include "TextEntry.h"

class HUD
{
	void* owner;
	std::vector<Caption*> captions;
	MessageQueue* messageQueue = NULL;
	TextEntry* entry;

public:

	HUD(void* _owner) : owner(_owner), messageQueue(new MessageQueue(this, vec3(0.6f, 0.95f, 0))) {
		Caption::setCaptionVectorReference(&captions);
	}

	~HUD() {
		delete messageQueue;

		for each (Caption * cap in captions)
		{
			delete cap;
		}
	}

	void Animate(float dt);

	void Draw(GPUProgram& gpuProgram, Camera& camera);

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);

	void pushMessage(const char* text);

	void removeCaption(Caption* captionToRemove);

	void createTextEntry(const char* tytle);

	void type(char c);
};

