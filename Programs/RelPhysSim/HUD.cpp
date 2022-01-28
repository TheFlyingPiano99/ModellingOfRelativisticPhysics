#include "HUD.h"

HUD* HUD::instance = NULL;


void HUD::Animate(float dt)
{
	for each (std::shared_ptr<Caption*> cap in captions)
	{
		(*cap)->Animate();
	}
	messageQueue->Animate(dt);
}

void HUD::Draw(GPUProgram& gpuProgram, Camera& camera)
{
	if (!visible) {
		return;
	}
	for each (std::shared_ptr<Caption*> cap in captions)				// Captions
	{
		(*cap)->DrawDiagram(gpuProgram, camera);
	}
}

void HUD::DrawDiagram(GPUProgram& gpuProgram, Camera& camera)
{
	if (!visible) {
		return;
	}
	for each (std::shared_ptr<Caption*> cap in captions)				// Captions
	{
		(*cap)->DrawDiagram(gpuProgram, camera);
	}
}

void HUD::pushMessage(const char* text)
{
	messageQueue->push(text);
}

void HUD::updateSettings(const RelTypes::Settings& settings)
{
	settingsBar->updateSettings(settings);
}

void HUD::pushCaption(std::shared_ptr<Caption*> captionToPush)
{
	captions.push_back(captionToPush);
}

void HUD::ereaseCaption(std::shared_ptr<Caption*> captionToRemove)
{
	int index = -1;
	for (int i = 0; i < captions.size(); i++) {
		if (captions[i] == captionToRemove) {
			index = i;
			break;
		}
	}
	if (index > -1) {
		captions.erase(captions.begin() + index);
	}
}

void HUD::createTextEntry(const char* tytle, void (*handler)(const char*))
{
	entry = new TextEntry(this, vec2(0,0), tytle, handler);
}

bool HUD::type(char c)
{
	if (entry != nullptr) {
		bool isFinished = entry->type(c);
		if (isFinished) {
			if (entry != nullptr) {
				delete entry;
				entry = nullptr;
			}
			return true;
		}
	}
	return false;
}

void HUD::clearCaptions()
{
	messageQueue->clearQueue();

	captions.clear();
}

void pushCaptionHandler(std::shared_ptr<Caption*> captionToPush)
{
	HUD::getInstance()->pushCaption(captionToPush);
}

void ereaseCaptionHandler(std::shared_ptr<Caption*> captionToRemove)
{
	HUD::getInstance()->ereaseCaption(captionToRemove);
}
