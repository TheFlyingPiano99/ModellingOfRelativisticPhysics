#pragma once
#include "Threading.h"
#include "framework.h"


class NativeInputHandler {
public:

	inline void startSym(int argc, char* argv[]) {
		if (coreThread == nullptr) {
			coreThread = new std::thread(mainSym, argc, argv);
		}
	}

	inline void closeSym() {
		if (coreThread != nullptr) {
			terminateSym = true;
			coreThread->join();
			delete coreThread;
			coreThread = nullptr;
		}
	}

};