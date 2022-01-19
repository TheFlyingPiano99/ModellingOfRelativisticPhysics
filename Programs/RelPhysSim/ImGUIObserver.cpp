#include "ImGUIObserver.h"

void ImGUIObserver::addBinding(IObservedVariable* variable) {
	bindings.push_back(variable);
}

void ImGUIObserver::clearBindings() {
	for (auto b : bindings) {
		delete b;
	}
	bindings.clear();
}

void ImGUIObserver::checkChanges()
{
	for (auto b : bindings) {
		b->action();
	}
}
