#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "../Component.h"
#include "../../Mesh/Mesh.h"
#include "../../Texture/TextureList.h"

class TextRenderer : public Component {

public:
	//Variable(s)
	Mesh* mesh;
	TextureList textureList;
	bool lightEnabled;
	Color textColor;
	string text;

	//Constructor(s) & Destructor
	TextRenderer(GameObject& _gameObject, PassKey<ComponentManager> _key);
	virtual ~TextRenderer();

};

#endif