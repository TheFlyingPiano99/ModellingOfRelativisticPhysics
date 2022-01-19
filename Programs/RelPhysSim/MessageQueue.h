#pragma once
#include "Caption.h"
#include <deque>
#include <memory>

class MessageQueue
{
	std::deque<std::shared_ptr<Caption*>> queue;
	const int maxSize = 10;
	float timeLeft;
	const float displayTime = 800;
	const float transitTime = 200;
	const float firstDisplayTime = 2000;
	bool transitPhase = false;
	void* owner;
	vec3 startPos;
	vec3 entryOffset = vec3(0, -0.03f, 0);

	void pop();

public:
	MessageQueue(void* _owner, vec3 pos): owner(_owner), startPos(pos) {
	}

	~MessageQueue();

	void push(const char* text);

	void Animate(float dt);

	void clearQueue();

};

