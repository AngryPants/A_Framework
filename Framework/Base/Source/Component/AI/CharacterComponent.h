#ifndef CHARACTERCOMPONENT_H
#define CHARACTERCOMPONENT_H

#include "../Component.h"
#include <set>
#include <string>
#include <map>

using std::map;
using std::string;
using std::set;

class Message;
class State;

class CharacterComponent : public Component
{
private:
	//Variable(s)
	bool isMessagingObservers;

protected:
	//Varialbe(s)
	State* currentState;
	map<string, State*> states;
	set<CharacterComponent*> subjects; //Things that we subscribed to.
	set<CharacterComponent*> observers; //Things that subscribe to this.
	set<CharacterComponent*> addQueue; //Add Queue for observers.
	set<CharacterComponent*> removeQueue; //Remove Queue for observers.

public:
	//Constructor(s) & Destructor
	CharacterComponent(GameObject& _gameObject, PassKey<ComponentManager> _key);
	virtual ~CharacterComponent();

	/* This are Functions That Should be Inside our GameObject*/
	//Interface Function(s)
	void AddObserver(CharacterComponent& observer); //Another CharacterComponent subscribes to us.
	void RemoveObserver(CharacterComponent& observer); //Remove a subscribes to us.
	void AddSubject(CharacterComponent& subject); //This CharacterComponent observes the subject.
	void RemoveSubject(CharacterComponent& subject); //This CharacterComponent stops observing the subject.

	virtual void SendMessageToSubscribers(const Message* message); //send information to those that requires it
	virtual void ReceiveMessage(const Message* messagePtr) = 0; // retrieve information required
	virtual void SetNextState(const string& nextState) = 0; // Conditions to go to next state
};

#endif