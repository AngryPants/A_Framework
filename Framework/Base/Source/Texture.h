#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

using std::string;

typedef unsigned int TextureID;

struct Texture {

private:
	TextureID textureID;

public:
	Texture(const string& filePath);
	virtual ~Texture();

	TextureID GetTextureID() const;

};

#endif