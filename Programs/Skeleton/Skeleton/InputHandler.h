#pragma once

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include "InputHandler_Native.h"

using namespace msclr::interop;

public ref class InputHandler {
private:
	NativeInputHandler* nativeHandler = nullptr;
	marshal_context mContext;

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

	inline void startSym(array<System::String^>^ args) {
		if (nativeHandler != nullptr) {
			int argc = args->Length;
			std::vector<const char*> argv;
			for (int i = 0; i < argc; i++) {
				argv.push_back(mContext.marshal_as<const char*>(args[i]));
			}
			nativeHandler->startSym(argc, &argv[0]);
		}
	}

	inline void closeSym() {
		if (nativeHandler != nullptr) {
			nativeHandler->closeSym();
		}
	}
};