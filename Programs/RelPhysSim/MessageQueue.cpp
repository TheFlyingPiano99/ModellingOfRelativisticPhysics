#include "MessageQueue.h"
#include "Scene.h"
#include "HUD.h"

void MessageQueue::pop()
{
	std::shared_ptr<Caption*> caption = queue.front();
	queue.pop_front();
	(*caption)->erease();
	transitPhase = true;
	timeLeft = transitTime;
}

MessageQueue::~MessageQueue() {
	for (std::shared_ptr<Caption*> cap : queue) {
		(*cap)->erease();
	}
}

void MessageQueue::push(const char* text)
{
	vec3 pos = startPos + entryOffset * queue.size();
	std::shared_ptr<Caption*> caption = Caption::createSmallCameraSpaceCaption(vec2(pos.x, pos.y), text);
	if (queue.empty()) {
		timeLeft = firstDisplayTime;
		transitPhase = false;
	}
	else {
		timeLeft = displayTime;
	}
	queue.push_back(caption);
	if (queue.size() > maxSize) {
		pop();
	}
}

void MessageQueue::animate(float dt) {
	if (queue.empty()) {
		transitPhase = false;
		return;
	}
	timeLeft -= dt;
	if (timeLeft <= 0.0f) {		// Change state

		if (transitPhase) {		// Change state to display
			transitPhase = false;
			timeLeft = displayTime;
		}
		else {					// Change state to transit
			pop();				// Pop queue
		}
	}
	if (transitPhase) {		// Move entries
		for (int i = 0; i < queue.size(); i++) {			// Interpolate position between prev and next pos.
			(*queue[i])->setPos(remap<float, vec3>(0, transitTime, startPos + entryOffset * i, startPos + entryOffset * (i + 1), timeLeft));
		}
	}
}

void MessageQueue::clearQueue()
{
	queue.clear();
}
