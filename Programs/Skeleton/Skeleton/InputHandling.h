#pragma once

#include "InputHandler_Native.h"

public ref class InputHandler {
private:
	NativeInputHandler* nativeHandler = nullptr;

public:
	InputHandler() {
		nativeHandler = new NativeInputHandler();
	}

	~InputHandler() {
		if (nullptr != nativeHandler)
		{
			delete nativeHandler;
			nativeHandler = nullptr;
		}
	}

	!InputHandler()
	{
		if (nullptr != nativeHandler) {
			delete nativeHandler;
			nativeHandler = nullptr;
		}
	}

	inline void startSym(int argc, char* argv[]) {
		if (nativeHandler != nullptr) {
			nativeHandler->startSym(argc, argv);
		}
	}

	inline void closeSym() {
		if (nativeHandler != nullptr) {
			nativeHandler->closeSym();
		}
	}
};