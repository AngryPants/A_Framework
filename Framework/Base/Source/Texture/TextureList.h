#ifndef TEXTURE_LIST_H
#define TEXTURE_LIST_H

#include "Texture.h"

const unsigned int MAX_TEXTURES = 8;

struct TextureList {

public:
	//Variable(s)
	TextureID textureArray[MAX_TEXTURES];

	//Constructor(s) & Destructor
	TextureList() {
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			textureArray[i] = 0;
		}
	}
	virtual ~TextureList() {}
	
	//Function(s)
	void Reset() {
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			textureArray[i] = 0;
		}
	}

	//Operator Overload.
	bool operator==(const TextureList& rhs) const {		
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			if (textureArray[i] != rhs.textureArray[i]) {
				return false;
			}
		}
		return true;
	}
	TextureList& operator=(const TextureList& rhs) {
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			textureArray[i] = rhs.textureArray[i];
		}
		return *this;
	}

};

#endif