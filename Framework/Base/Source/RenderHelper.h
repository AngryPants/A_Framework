#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "SingletonTemplate.h"
#include "Mesh.h"
#include "SpriteAnimation.h"
#include "TextureList.h"
#include "Vertex.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "Transform.h"

//Include Shader Stuff
#include "PhongShader.h"

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
	void UpdateLight(Light& light, Transform& transform, unsigned int lightIndex);		

	//Rendering
	void RenderMesh(Mesh& mesh, const bool enableLight = false);
	void RenderMesh(Mesh& mesh, TextureList& textureList, const bool enableLight = false);	
	void RenderText(Mesh& mesh, TextureList& textureList, const string& text, Color color);

};

#endif