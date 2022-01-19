#pragma once
#include <functional>

class ImGUIObserver
{

public:
	class IObservedVariable {
	public:
		virtual void action() = 0;
	};
	template<typename T>
	class  ObservedVariable : public IObservedVariable {
		T prevVal;
		T* val;
		std::function<void(T)> callback;
	public:
		ObservedVariable(T* bindingVariable, std::function<void(T)> callback);

		void action() override;
	};

private:
	std::vector<IObservedVariable*> bindings;
public:
	
	void addBinding(IObservedVariable* variable);
	void clearBindings();
	void checkChanges();

};

template<typename T>
ImGUIObserver::ObservedVariable<T>::ObservedVariable(T* bindingVariable, std::function<void(T)> callback) {
	val = bindingVariable;
	prevVal = *val;
	this->callback = callback;
}

template<typename T>
void ImGUIObserver::ObservedVariable<T>::action() {
	if (prevVal != *val) {
		callback(*val);
		prevVal = *val;
	}
}
