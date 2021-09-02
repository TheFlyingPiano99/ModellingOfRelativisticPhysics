#pragma once
#include "framework.h"
#include "Caption.h"
#include <deque>
#include <memory>

class MessageQueue
{
	std::deque<std::shared_ptr<Caption*>> queue;
	const int maxSize = 10;
	float timeLeft = 500;
	const float displayTime = 800;
	const float transitTime = 200;
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

