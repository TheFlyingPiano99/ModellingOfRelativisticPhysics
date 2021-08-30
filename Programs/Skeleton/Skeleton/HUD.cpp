#include "HUD.h"

void HUD::Animate(float dt)
{
	for each (Caption * cap in captions)
	{
		cap->Animate();
	}
	messageQueue->Animate(dt);
}

void HUD::Draw(GPUProgram& gpuProgram, Camera& camera)
{
	for each (Caption * cap in captions)				// Captions
	{
		cap->Draw(gpuProgram, camera);
	}
}

void HUD::DrawDiagram(GPUProgram& gpuProgram, Camera& camera)
{
	for each (Caption * cap in captions)				// Captions
	{
		cap->DrawDiagram(gpuProgram, camera);
	}
}

void HUD::pushMessage(const char* text)
{
	messageQueue->push(text);
}

void HUD::removeCaption(Caption* captionToRemove)
{
	int index = -1;
	for (int i = 0; i < captions.size(); i++) {
		if (captions[i] == captionToRemove) {
			index = i;
		}
	}
	if (index > -1) {
		delete captionToRemove;
		captions.erase(captions.begin() + index);
	}
}

void HUD::createTextEntry(const char* tytle)
{
	entry = new TextEntry(this, vec2(0,0), tytle);
}

void HUD::type(char c)
{
	if (entry != nullptr) {
		entry->type(c);
	}
}
