#pragma once
#include "framework.h"
#include "Camera.h"
#include "Message.h"
#include "TextEntry.h"
#include <memory>

void pushCaptionHandler(std::shared_ptr<Caption*> captionToPush);
void ereaseCaptionHandler(std::shared_ptr<Caption*> captionToRemove);

class HUD
{
	static HUD* instance;
	void* owner;
	std::vector<std::shared_ptr<Caption*>> captions;
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
		if (entry != nullptr) {
			delete entry;
		}
	}

	void Animate(float dt);

	void Draw(GPUProgram& gpuProgram, Camera& camera);

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);

	void pushMessage(const char* text);

	void pushCaption(std::shared_ptr<Caption*> captionToPush);
	void ereaseCaption(std::shared_ptr<Caption*> captionToRemove);

	void createTextEntry(const char* tytle, void (*handler)(const char*));

	bool type(char c);

	void clearCaptions();
};

