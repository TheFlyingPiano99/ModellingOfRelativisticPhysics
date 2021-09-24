#pragma once
#include "Threading.h"
#include "framework.h"


class NativeInputHandler {
public:

	inline void startSym(int argc, const char* argv[]) {
		if (coreThread == nullptr) {
			std::remove_const<const char**>::type t;
			t = argv;
			coreThread = new std::thread(mainSym, argc, (char**)t);
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