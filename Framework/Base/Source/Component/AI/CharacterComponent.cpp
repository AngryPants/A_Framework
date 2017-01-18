#include "CharacterComponent.h"
#include <iostream>
using std::cout;
using std::endl;

CharacterComponent::CharacterComponent(GameObject& _gameObject, PassKey<ComponentManager> _key)
	: Component("CharacterComponent", _gameObject, _key)
	, currentState(nullptr)
	, isMessagingObservers(false)
{
}

CharacterComponent::~CharacterComponent() {
	//Stop observing our subjects.
	for (set<CharacterComponent*>::iterator setIter = subjects.begin(); setIter != subjects.end(); ++setIter) {
		CharacterComponent* CharacterComponentPtr = *setIter;
		CharacterComponentPtr->RemoveObserver(*this);
	}
	subjects.clear();

	//Add our new observers.
	for (set<CharacterComponent*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		observers.insert(*setIter);
	}
	addQueue.clear();
	//Remove the observers.
	for (set<CharacterComponent*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		set<CharacterComponent*>::iterator removeIter = observers.find(*setIter);
		if (removeIter != observers.end()) {
			observers.erase(removeIter);
		}
	}
	removeQueue.clear();
	//Tell our observers to stop observing us.
	for (set<CharacterComponent*>::iterator setIter = observers.begin(); setIter != observers.end(); ++setIter) {
		CharacterComponent* CharacterComponentPtr = *setIter;
		CharacterComponentPtr->RemoveSubject(*this);
	}
	observers.clear();

	//Delete our states.
	currentState = nullptr;
	for (map<string, State*>::iterator mapIter = states.begin(); mapIter != states.end(); ++mapIter) {
		State* statePtr = mapIter->second;
		if (statePtr != nullptr) {
			delete statePtr;
		}
	}
	states.clear();
}

void CharacterComponent::AddObserver(CharacterComponent& observer) {
	if (&observer == this)
	{
		cout << "Cannot AddObserver(CharacterComponent& observer) self." << endl;
		return;
	}

	if (!isMessagingObservers)
		observers.insert(&observer);
	else
		addQueue.insert(&observer);
}

void CharacterComponent::RemoveObserver(CharacterComponent& observer) {

	if (&observer == this) {
		cout << "Cannot RemoveObserver(CharacterComponent& observer) self." << endl;
		return;
	}

	if (!isMessagingObservers)
	{
		set<CharacterComponent*>::iterator setIter = observers.find(&observer);
		if (setIter != observers.end())
			observers.erase(setIter);
	}
	else
		removeQueue.insert(&observer);
}

void CharacterComponent::AddSubject(CharacterComponent& subject) {
	if (&subject == this) {
		cout << "Cannot AddSubject(CharacterComponent& subject) self." << endl;
		return;
	}
	subject.AddObserver(*this);
	subjects.insert(&subject);
}

void CharacterComponent::RemoveSubject(CharacterComponent& subject) {
	if (&subject == this) {
		cout << "Cannot RemoveSubject(CharacterComponent& subject) self." << endl;
		return;
	}
	set<CharacterComponent*>::iterator setIter = subjects.find(&subject);
	if (setIter != subjects.end()) {
		//subject.RemoveObserver(*this);
		subjects.erase(setIter);
	}
}

void CharacterComponent::SendMessageToSubscribers(const Message* message) {
	isMessagingObservers = true;
	//Messages all our observers.
	for (set<CharacterComponent*>::iterator setIter = observers.begin(); setIter != observers.end(); ++setIter) {
		(*setIter)->ReceiveMessage(message);
	}
	//Add our new observers.
	for (set<CharacterComponent*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		observers.insert(*setIter);
	}
	addQueue.clear();
	//Remove the observers.
	for (set<CharacterComponent*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		set<CharacterComponent*>::iterator removeIter = observers.find(*setIter);
		if (removeIter != observers.end()) {
			observers.erase(removeIter);
		}
	}
	removeQueue.clear();
	isMessagingObservers = false;
}