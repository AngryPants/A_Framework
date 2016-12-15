#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "SingletonTemplate.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/Vertex.h"
#include "../Animation/SpriteAnimation.h"
#include "../Texture/TextureList.h"
#include "ShaderProgram.h"
#include "../Component/Rendering/Light.h"
#include "../Component/Physics/Transform.h"

//Include Shader Stuff
#include "./Shader/Phong/PhongShader.h"

enum class SHADERS {
	PHONG,
};

class RenderHelper : public Singleton<RenderHelper> {

	friend class Singleton<RenderHelper>;

private:
	//Shader Specific Stuff
	PhongShader::Uniforms phongUniforms;
	
	//Variable(s)
	SHADERS currentShader;
	unsigned int numLights;

	//Constructor(s) & Destructor
	RenderHelper();
	virtual ~RenderHelper();

public:
	void LoadShader(const SHADERS shaderType);
	void DeleteShader(const SHADERS shaderType);
	void SetAlphaDiscardValue(const float alphaValue);
	void EnableFog(const bool boolean);	
	void EnableLight(const bool boolean);	
	void SetNumLights(const unsigned numLights);
	void UpdateLight(Light& light, Transform& transform, const unsigned int lightIndex);
	void TurnOffLight(const unsigned int lightIndex);
	unsigned int GetNumLights();

	//Rendering
	void RenderMesh(Mesh& mesh, const bool enableLight = false);
	void RenderMesh(Mesh& mesh, const TextureList& textureList, const bool enableLight = false);	
	void RenderText(Mesh& mesh, const TextureList& textureList, const string& text, Color color);

};

#endif