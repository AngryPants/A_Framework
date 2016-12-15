#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "../Component.h"
#include "../../Mesh/Mesh.h"
#include "../../Texture/TextureList.h"
#include "Vector2.h"

class TextRenderer : public Component {

public:
	//Variable(s)
	Vector3 position;
	bool isUI;
	float rotation;
	Vector3 scale;
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