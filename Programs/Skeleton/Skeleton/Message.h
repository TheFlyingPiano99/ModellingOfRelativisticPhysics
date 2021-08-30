#pragma once
#include "framework.h"
#include "Caption.h"
#include <deque>

class MessageQueue
{
	std::deque<Caption*> queue;
	float timeLeft = 500;
	const float displayTime = 800;
	const float transitTime = 200;
	bool transitPhase = false;
	void* owner;
	vec3 startPos;
	vec3 entryOffset = vec3(0, -0.05f, 0);

public:
	MessageQueue(void* _owner, vec3 pos): owner(_owner), startPos(pos) {

	}

	void push(const char* text);

	void Animate(float dt);

};

