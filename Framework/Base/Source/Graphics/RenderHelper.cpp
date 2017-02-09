#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "GraphicsUtility.h"
#include <iostream>
#include "../Others/Utility.h"

using namespace std;

//NOTE: Even tough lots of variables are of type TextureList, they are named texture instead of textureList.
//This is due to some backend change and name change. They should be named textureList if we want to be correct.
//But for now if they ain't broke, I'm not fucking with it until I need to.

//Constructor(s) & Destructor
RenderHelper::RenderHelper() {
	numLights = 0;
}

RenderHelper::~RenderHelper() {
}

//Updating Shader Stuff
void RenderHelper::LoadShader(const SHADERS shaderType) {
	currentShader = shaderType;
	switch (shaderType) {
		case SHADERS::PHONG:
			PhongShader::LoadShader("Phong", phongUniforms);
			break;
	}
}

void RenderHelper::DeleteShader(const SHADERS shaderType) {
	switch(shaderType) {
		case SHADERS::PHONG:
			PhongShader::DeleteShader("Phong");
			break;
	}
}

void RenderHelper::SetAlphaDiscardValue(const float alphaValue) {
	switch (currentShader) {
		case SHADERS::PHONG:
			GetShader()->Update(phongUniforms.alphaDiscardValue, alphaValue);
			break;
	}
}

void RenderHelper::EnableFog(const bool boolean) {
	switch (currentShader) {
		case SHADERS::PHONG:
			PhongShader::EnableFog(boolean, phongUniforms);
			break;
	}
}

void RenderHelper::EnableLight(const bool boolean) {
	switch (currentShader) {
		case SHADERS::PHONG:
			PhongShader::EnableLight(boolean, phongUniforms);
			break;
	}
}

void RenderHelper::SetNumLights(const unsigned int numLights) {
	switch (currentShader) {
		case SHADERS::PHONG:
			if (numLights <= PhongShader::Uniforms::MAX_LIGHTS) {
				PhongShader::SetNumLights(numLights, phongUniforms);
				this->numLights = numLights;
			} else {
				PhongShader::SetNumLights(PhongShader::Uniforms::MAX_LIGHTS, phongUniforms);
				this->numLights = PhongShader::Uniforms::MAX_LIGHTS;
			}
			break;
	}
}

void RenderHelper::UpdateLight(Light& light, Transform& transform, const unsigned int lightIndex) {
	switch (currentShader) {
		case SHADERS::PHONG:
			PhongShader::UpdateLight(light, transform, lightIndex, phongUniforms);
			break;
	}
}

void RenderHelper::TurnOffLight(const unsigned int lightIndex) {
	switch (currentShader) {
		case SHADERS::PHONG:
			PhongShader::TurnOffLight(lightIndex, phongUniforms);
			break;
	}
}

unsigned int RenderHelper::GetNumLights() {
	return numLights;
}

//Rendering
void RenderHelper::RenderMesh(Mesh& mesh, const bool enableLight) {
	static TextureList texture;
	RenderMesh(mesh, texture, enableLight);
}

void RenderHelper::RenderMesh(Mesh& mesh, const TextureList& textureList, const bool enableLight) {
	switch (currentShader) {
		case SHADERS::PHONG:
			PhongShader::RenderMesh(mesh, textureList, enableLight, phongUniforms);
			break;
	}
}

void RenderHelper::RenderText(Mesh& mesh, const TextureList& textureList, const string& text, Color color, bool lightEnabled, bool centralise) {
	switch (currentShader) {
		case SHADERS::PHONG:
			PhongShader::RenderText(mesh, textureList, text, color, lightEnabled, centralise, phongUniforms);
			break;
	}
}