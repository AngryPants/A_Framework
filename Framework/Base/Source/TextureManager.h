#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "SingletonTemplate.h"
#include "Texture.h"
#include <map>
#include <string>
#include <iostream>

using std::map;
using std::pair;
using std::string;
using std::cout;
using std::endl;

class TextureManager : public Singleton<TextureManager> {

	friend class Singleton<TextureManager>;

private:
	//Variable(s)
	map<string, Texture*> textureMap;

	//Constructor(s) & Destructor
	TextureManager();
	virtual ~TextureManager();

public:
	//User Interface
	bool Clear();
	TextureID GetTextureID(const string& textureName);
	bool CheckTextureExist(const string& textureName) const;
	TextureID AddTexture(const string& textureName, const string& filePath);
	bool RemoveTexture(const string& textureName);

};

#endif