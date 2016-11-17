#include "TextureManager.h"

//Constructor(s) & Destructor
TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	Clear();
}

//User Interface
bool TextureManager::Clear() {

	for (map<string, Texture*>::iterator mapIter = textureMap.begin(); mapIter != textureMap.end(); ++mapIter) {
		delete mapIter->second;
	}
	textureMap.clear();

	return true;
	
}

TextureID TextureManager::GetTextureID(const string& textureName) {

	map<string, Texture*>::iterator mapIter = textureMap.find(textureName);
	if (mapIter == textureMap.end()) {
		cout << "Missing Texture: " << textureName << endl;
		return 0;
	}

	return (*mapIter->second).GetTextureID();

}

bool TextureManager::CheckTextureExist(const string& textureName) const {

	return textureMap.count(textureName) != 0;

}

TextureID TextureManager::AddTexture(const string& textureName, const string& filePath) {

	map<string, Texture*>::iterator mapIter = textureMap.find(textureName);
	if (mapIter != textureMap.end()) {
		return (*mapIter->second).GetTextureID();
	}

	Texture* texture = new Texture(filePath);
	textureMap.insert(pair<string, Texture*>(textureName, texture));

	return texture->GetTextureID();

}

bool TextureManager::RemoveTexture(const string& textureName) {

	map<string, Texture*>::iterator mapIter = textureMap.find(textureName);
	if (mapIter == textureMap.end()) {
		cout << "Unable to remove texture: " << textureName << " as it doesn't exist in TextureManager." << endl;
		return false;
	}

	delete mapIter->second;
	textureMap.erase(mapIter);

	return true;

}