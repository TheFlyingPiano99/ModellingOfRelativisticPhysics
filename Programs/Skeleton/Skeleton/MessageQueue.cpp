#include "MessageQueue.h"
#include "Scene.h"
#include "HUD.h"

void MessageQueue::pop()
{
	std::shared_ptr<Caption*> caption = queue.front();
	queue.pop_front();
	(*caption)->erease();
}

MessageQueue::~MessageQueue() {
	for each (std::shared_ptr<Caption*> cap in queue) {
		(*cap)->erease();
	}
}

void MessageQueue::push(const char* text)
{
	vec3 pos = startPos + entryOffset * queue.size();
	std::shared_ptr<Caption*> caption = Caption::createSmallCameraSpaceCaption(vec2(pos.x, pos.y), text);
	if (queue.empty()) {
		timeLeft = displayTime;
		transitPhase = false;
	}
	queue.push_back(caption);
	if (queue.size() > maxSize) {
		pop();
	}
}

void MessageQueue::Animate(float dt) {
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
			transitPhase = true;
			pop();				// Pop queue
			timeLeft = transitTime;
		}
	}
	if (transitPhase) {		// Move entries
		for (int i = 0; i < queue.size(); i++) {			// Interpolate position between prev and next pos.
			(*queue[i])->setPos((startPos + entryOffset * (i + 1)) * timeLeft / transitTime
				+ (startPos + entryOffset * (i) ) * (1 - timeLeft / transitTime) );
		}
	}
}

void MessageQueue::clearQueue()
{
	queue.clear();
}
