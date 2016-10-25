#ifndef ITEM_INDEX_H
#define ITEM_INDEX_H

#include <vector>
#include <set>
#include <string>
#include <exception>
#include <iostream>

using namespace std;

template <class Type>
class ItemIndex {

private:
	//Variable(s)
	vector<Type*> items;
	set<unsigned int> validSlots;
	set<unsigned int> freeSlots;

public:
	//Constructor(s) & Destructor
	ItemIndex() {}
	virtual ~ItemIndex() {
		Clear(); //Clear all the items when deleted.
	}

	//Interface Function(s)
	//Adds the item into the ItemIndex. 
	unsigned int AddItem(Type& item) {
		//Get the index first.
		unsigned int index = validSlots.size();
		//Check for any freeSlots indexes. If there are, use those indexes first.
		if (!freeSlots.empty()) {
			index = *freeSlots.begin();
			freeSlots.erase(freeSlots.begin());
		}
		//Make sure that there is enough space to add the item.
		if (index >= items.size()) {
			items.push_back(nullptr);
		}
		//Insert the thingys.
		validSlots.insert(index);
		items[index]= &item;
		//Return the index.
		return index;
	}
	//Retrieves the item from the specified index and returns a pointer to it. The item is not removed from ItemIndex.
	Type& GetItem(unsigned int index) {
		if(index >= items.size() || items[index] == nullptr) {
			throw exception("ItemIndex invalidSlots index.");
		}
		return *(items[index]);
	}
	//Retrieves the item from the specified index and returns a pointer to it. The item is removed from ItemIndex.
	Type* RemoveItem(const unsigned int index) {
		Type* itemPtr = nullptr;
		//Ensure that the index is legit.
		if (index >= items.size()) {
			throw exception("ItemIndex - index out of range.");
		}
		//Check if there is an item in the index's slot.
		if (items[index] != nullptr) {
			itemPtr = items[index];
			items[index] = nullptr;
			validSlots.erase(validSlots.find(index));
			freeSlots.insert(index);
		}
		//If there are no validSlots items, it means that our vector is empty.
		if (validSlots.empty()) {
			freeSlots.clear();
			items.clear();
		}
		//Remove the slots that we don't need.
		while (!freeSlots.empty() && !validSlots.empty() && *freeSlots.rbegin() > *validSlots.rbegin()) {
			freeSlots.erase(--freeSlots.end());
			items.pop_back();
		}
		//Return the item.
		return itemPtr;
	}
	//Delete the item in the specified index.
	void DeleteItem(const unsigned int index) {
		Type* itemPtr = RemoveItem(index);
		if (itemPtr != nullptr) {
			delete itemPtr;
		}
	}		
	//Clear everything. The items are deleted.
	void Clear() {
		while (validSlots.size() > 0) {
			DeleteItem(*validSlots.begin());
		}
	}

	//Get the list of validSlots indexes.
	const set<unsigned int>& GetValidIndexes() const {
		return validSlots;
	}
	//Get the list of freeSlots indexes.
	const set<unsigned int>& GetFreeIndexes() const {
		return freeSlots;
	}
	//Get the number of validSlots indexes.
	unsigned int GetNumValid() const {
		return validSlots.size();
	}
	//Get the number of freeSlots indexes.
	unsigned int GetNumFree() const {
		return freeSlots.size();
	}

};

#endif