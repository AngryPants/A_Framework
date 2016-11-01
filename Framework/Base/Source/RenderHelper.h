#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "SingletonTemplate.h"
#include "Mesh.h"
#include "SpriteAnimation.h"
#include "TextureList.h"
#include "Vertex.h"
#include "ShaderProgram.h"
#include "Light.h"

enum class SHADERS {
	SHADOW
};

class RenderHelper : public Singleton<RenderHelper> {

	friend class Singleton<RenderHelper>;

public:
	struct ShadowUniforms {
		UniformID modelView;
		UniformID modelViewProjection;
		UniformID modelViewInverseTranspose;
		UniformID ambient;
		UniformID diffuse;
		UniformID specular;
		UniformID shininess;
		UniformID colorTextureEnabled;
		UniformID colorTexture;
		UniformID textColor;
		UniformID textEnabled;
		UniformID fogEnabled;
		UniformID alphaDiscardValue;
		UniformID textureOffset;
		UniformID textureScale;

		//Lights
		UniformID lightEnabled;
		UniformID numLights;
		static const unsigned int MAX_LIGHTS = 8;
		UniformID lightOn[MAX_LIGHTS];
		UniformID lightType[MAX_LIGHTS];
		UniformID lightPosition[MAX_LIGHTS];
		UniformID lightColor[MAX_LIGHTS];
		UniformID lightPower[MAX_LIGHTS];
		UniformID lightKC[MAX_LIGHTS];
		UniformID lightKL[MAX_LIGHTS];
		UniformID lightKQ[MAX_LIGHTS];
		UniformID lightSpotDirection[MAX_LIGHTS];
		UniformID lightCosCutoff[MAX_LIGHTS];
		UniformID lightCosInner[MAX_LIGHTS];
		UniformID lightExponent[MAX_LIGHTS];
	};

private:
	//Variable(s)
	bool refreshUniforms;
	ShadowUniforms shadowUniforms;
	SHADERS currentShader;

	//Constructor(s) & Destructor
	RenderHelper();
	virtual ~RenderHelper();

	//Shader Loading
	void LoadShadowShader();

public:
	template<SHADERS>
	void LoadShader() {
		throw std::exception("Unrecognisable shader!");
	}

	template<>
	void LoadShader<SHADERS::SHADOW>() {
		LoadShadowShader();
	}

	void Update();
	void SetNumLights(const unsigned num);
	void UpdateLight(Light& light, unsigned int lightIndex);
	void SetAlphaDiscardValue(const float alphaValue);
	void EnableLight(const bool boolean);
	void EnableFog(const bool boolean);

	//Rendering
	void RenderMesh(Mesh& mesh, TextureList& texture, const bool& enableLight= false);
	void RenderMesh(Mesh& mesh, const bool& enableLight = false);
	void RenderText(Mesh& mesh, TextureList& texture, const string& text, Color color);

};

#endif