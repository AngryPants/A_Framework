#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "../../ShaderProgram.h"
#include "../../../Component/Rendering/Light.h"
#include "../../../Component/Physics/Transform.h"
#include "../../../Mesh/Mesh.h"
#include "../../../Texture/TextureManager.h"
#include "../../../Texture/TextureList.h"

namespace PhongShader {

	struct Uniforms {
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
		static const unsigned int MAX_LIGHTS = 8;
		UniformID lightEnabled;
		UniformID numLights;		
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
	
	void LoadShader(const string& shaderName, PhongShader::Uniforms& uniforms);
	void DeleteShader(const string& shaderName);
	void SetAlphaDiscardValue(const float alphaValue, const PhongShader::Uniforms& uniforms);
	void EnableFog(const bool boolean, const PhongShader::Uniforms& uniforms);
	void EnableLight(const bool boolean, const PhongShader::Uniforms& uniforms);
	void SetNumLights(const int numLight, const PhongShader::Uniforms& uniforms);
	void UpdateLight(Light& light, Transform& transform, const unsigned int lightIndex, const PhongShader::Uniforms& uniforms);
	void TurnOffLight(const unsigned int lightIndex, const PhongShader::Uniforms& uniforms);

	void RenderText(Mesh& mesh, const TextureList& textureList, const string& text, Color color, bool _lightEnabled, const PhongShader::Uniforms& uniforms);
	void RenderMesh(Mesh& mesh, const TextureList& textureList, const bool& enableLight, const PhongShader::Uniforms& shadowUniforms);

}

#endif