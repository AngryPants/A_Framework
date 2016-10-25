#include "Texture.h"
#include "GL\glew.h"
#include "LoadTGA.h"

Texture::Texture(const string& filePath) {
	textureID = LoadTGA(filePath.c_str());
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

TextureID Texture::GetTextureID() const {
	return this->textureID;
}