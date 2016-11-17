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
	TextRenderer(GameObject& gameObject) : Component("Text Renderer", gameObject) {
		mesh = nullptr;
		lightEnabled = false;
		textColor.Set(0, 0, 0);
	}
	virtual ~TextRenderer() {}

};