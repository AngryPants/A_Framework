#include "RenderHelper.h"
#include "GraphicsManager.h"
#include <iostream>
#include "Utility.h"

using namespace std;

//NOTE: Even tough lots of variables are of type TextureList, they are named texture instead of textureList.
//This is due to some backend change and name change. They should be named textureList if we want to be correct.
//But for now if they ain't broke, I'm not fucking with it until I need to.

ShaderProgram* GetShader() {

	ShaderProgram* shaderProgram = GraphicsManager::GetInstance().GetActiveShader();
	if (shaderProgram == nullptr) {
		throw std::exception("Unable to render as no active shader has been set.");
	}
	return shaderProgram;
	
}

ShaderProgram::FloatMat4x4 ToOpenglStruct(const Mtx44& matrix) {

	ShaderProgram::FloatMat4x4 mat;

	for (unsigned int i = 0; i < 16; ++i) {
		mat.mat[i] = matrix.a[i];
	}

	return mat;

}

ShaderProgram::FloatVec3 ToOpenglStruct(const Color& color) {

	ShaderProgram::FloatVec3 col;

	col.vec[0] = color.r;
	col.vec[1] = color.g;
	col.vec[2] = color.b;

	return col;

}

ShaderProgram::FloatVec3 ToOpenglStruct(const MaterialComponent& component) {

	ShaderProgram::FloatVec3 comp;

	comp.vec[0] = component.r;
	comp.vec[1] = component.g;
	comp.vec[2] = component.b;

	return comp;

}

ShaderProgram::FloatVec3 ToOpenglStruct(const Vector3& vector) {

	ShaderProgram::FloatVec3 vect;

	vect.vec[0] = vector.x;
	vect.vec[1] = vector.y;
	vect.vec[2] = vector.z;

	return vect;

}

ShaderProgram::FloatVec3 ToOpenglStruct(const Position& position) {

	ShaderProgram::FloatVec3 pos;

	pos.vec[0] = position.x;
	pos.vec[1] = position.y;
	pos.vec[2] = position.z;

	return pos;

}

void SetNumLightsDelegate(const int num, const RenderHelper::ShadowUniforms& shadowUniforms) {

	ShaderProgram* shaderProgram = GetShader();

	shaderProgram->Update(shadowUniforms.numLights, num);

}

void EnableLightDelegate(const bool boolean, const RenderHelper::ShadowUniforms& shadowUniforms) {

	ShaderProgram* shaderProgram = GetShader();

	shaderProgram->Update(shadowUniforms.lightEnabled, boolean);

}

void EnableFogDelegate(const bool boolean, const RenderHelper::ShadowUniforms& shadowUniforms) {

	ShaderProgram* shaderProgram = GetShader();

	shaderProgram->Update(shadowUniforms.fogEnabled, boolean);

}

void RenderTextDelegate(Mesh& mesh, TextureList& texture, const string& text, Color color, const RenderHelper::ShadowUniforms& shadowUniforms) {

	ShaderProgram* shaderProgram = GetShader();

	shaderProgram->Update(shadowUniforms.textEnabled, true);
	shaderProgram->Update(shadowUniforms.textColor, ToOpenglStruct(color), 1);
	shaderProgram->Update(shadowUniforms.lightEnabled, false);

	int booleans[MAX_TEXTURES] = { 1, 0, 0, 0, 0, 0, 0, 0 };
	int ids[MAX_TEXTURES] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	shaderProgram->Update(shadowUniforms.colorTextureEnabled, booleans, MAX_TEXTURES);
	shaderProgram->Update(shadowUniforms.colorTexture, ids, MAX_TEXTURES);

	//Update Texture Coordinates
	ShaderProgram::FloatVec2 textureOffset;
	textureOffset.vec[0] = mesh.textureOffset[0];
	textureOffset.vec[1] = mesh.textureOffset[1];
	shaderProgram->Update(shadowUniforms.textureOffset, textureOffset);

	ShaderProgram::FloatVec2 textureScale;
	textureScale.vec[0] = mesh.textureScale[0];
	textureScale.vec[1] = mesh.textureScale[1];
	shaderProgram->Update(shadowUniforms.textureScale, textureScale);

	shaderProgram->SetActiveTexture(0);
	shaderProgram->BindTexture(texture.textureArray[0]);

	for (unsigned i = 0; i < text.length(); ++i) {

		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = GraphicsManager::GetInstance().projectionStack.Top() * GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top() * characterSpacing;
		shaderProgram->Update(shadowUniforms.modelViewProjection, false, ToOpenglStruct(MVP));

		mesh.Render((unsigned)text[i] * 6, 6);

	}
	shaderProgram->UnbindTexture();
	shaderProgram->Update(shadowUniforms.textEnabled, false);

}

void RenderMeshDelegate(Mesh& mesh, TextureList& texture, const bool& enableLight, const RenderHelper::ShadowUniforms& shadowUniforms) {

	ShaderProgram* shaderProgram = GetShader();

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = GraphicsManager::GetInstance().projectionStack.Top() * GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top();

	shaderProgram->Update(shadowUniforms.modelViewProjection, false, ToOpenglStruct(MVP));
	modelView = GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top();
	shaderProgram->Update(shadowUniforms.modelView, false, ToOpenglStruct(modelView));
	modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
	shaderProgram->Update(shadowUniforms.modelViewInverseTranspose, false, ToOpenglStruct(modelView_inverse_transpose));

	shaderProgram->Update(shadowUniforms.lightEnabled, enableLight);

	//Update Texture Coordinates
	ShaderProgram::FloatVec2 textureOffset;
	textureOffset.vec[0] = mesh.textureOffset[0];
	textureOffset.vec[1] = mesh.textureOffset[1];
	shaderProgram->Update(shadowUniforms.textureOffset, textureOffset);

	ShaderProgram::FloatVec2 textureScale;
	textureScale.vec[0] = mesh.textureScale[0];
	textureScale.vec[1] = mesh.textureScale[1];
	shaderProgram->Update(shadowUniforms.textureScale, textureScale);

	if (enableLight) {
		//Load Material
		shaderProgram->Update(shadowUniforms.ambient, ToOpenglStruct(mesh.material.kAmbient));
		shaderProgram->Update(shadowUniforms.diffuse, ToOpenglStruct(mesh.material.kDiffuse));
		shaderProgram->Update(shadowUniforms.specular, ToOpenglStruct(mesh.material.kSpecular));
		shaderProgram->Update(shadowUniforms.shininess, mesh.material.kShininess);
	}

	int booleans[MAX_TEXTURES];
	unsigned ids[MAX_TEXTURES] = {0, 1, 2, 3, 4, 5, 6, 7};

	for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
		if (texture.textureArray[i] > 0) {
			shaderProgram->SetActiveTexture(i);
			shaderProgram->BindTexture(texture.textureArray[i]);
			booleans[i] = true;
		} else {
			booleans[i] = false;
		}
		shaderProgram->Update(shadowUniforms.colorTexture, ids, MAX_TEXTURES);
		shaderProgram->Update(shadowUniforms.colorTextureEnabled, booleans, MAX_TEXTURES);
	}

	mesh.Render();

	for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
		if (texture.textureArray[i] > 0) {
			shaderProgram->UnbindTexture();
		}
	}

}

RenderHelper::RenderHelper() {

	refreshUniforms = true;

}

RenderHelper::~RenderHelper() {
}

void RenderHelper::Update() {

	refreshUniforms = true;

}

void RenderHelper::LoadShadowShader() {

	if (!GraphicsManager::GetInstance().HasShader("Shadow")) {
		GraphicsManager::GetInstance().AddShader("Shadow", "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
		GraphicsManager::GetInstance().SetActiveShader("Shadow");
		ShaderProgram* shaderProgram = GraphicsManager::GetInstance().GetActiveShader();

		shadowUniforms.modelView = shaderProgram->GetUniformID("MV");
		shadowUniforms.modelViewProjection = shaderProgram->GetUniformID("MVP");
		shadowUniforms.modelViewInverseTranspose = shaderProgram->GetUniformID("MV_inverse_transpose");
		shadowUniforms.ambient = shaderProgram->GetUniformID("material.kAmbient");
		shadowUniforms.diffuse = shaderProgram->GetUniformID("material.kDiffuse");
		shadowUniforms.specular = shaderProgram->GetUniformID("material.kSpecular");
		shadowUniforms.shininess = shaderProgram->GetUniformID("material.kShininess");
		shadowUniforms.colorTextureEnabled = shaderProgram->GetUniformID("colorTextureEnabled");
		shadowUniforms.colorTexture = shaderProgram->GetUniformID("colorTexture");
		shadowUniforms.textColor = shaderProgram->GetUniformID("textColor");
		shadowUniforms.textEnabled = shaderProgram->GetUniformID("textEnabled");
		shadowUniforms.fogEnabled = shaderProgram->GetUniformID("fogParam.enabled");
		shadowUniforms.alphaDiscardValue = shaderProgram->GetUniformID("alphaDiscardValue");
		shadowUniforms.textureOffset = shaderProgram->GetUniformID("textureOffset");
		shadowUniforms.textureScale = shaderProgram->GetUniformID("textureScale");

		shadowUniforms.numLights = shaderProgram->GetUniformID("numLights");
		shadowUniforms.lightEnabled = shaderProgram->GetUniformID("lightEnabled");
		for (unsigned int i = 0; i < ShadowUniforms::MAX_LIGHTS; ++i) {	
			shadowUniforms.lightOn[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].on");
			shadowUniforms.lightType[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].type");
			shadowUniforms.lightPosition[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].position_cameraspace");
			shadowUniforms.lightColor[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].color");
			shadowUniforms.lightPower[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].power");
			shadowUniforms.lightKC[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].kC");
			shadowUniforms.lightKL[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].kL");
			shadowUniforms.lightKQ[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].kQ");
			shadowUniforms.lightSpotDirection[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].spotDirection");
			shadowUniforms.lightCosCutoff[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].cosCutoff");
			shadowUniforms.lightCosInner[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].cosInner");
			shadowUniforms.lightExponent[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].exponent");
		}
	} else {
		GraphicsManager::GetInstance().SetActiveShader("Shadow");
	}

	currentShader = SHADERS::SHADOW;

}

//For Shadow Shader
void UpdateLightDelegate(Light& light, unsigned int lightIndex, const RenderHelper::ShadowUniforms& shadowUniforms) {

	ShaderProgram* shader = GetShader();

	shader->Update(shadowUniforms.lightOn[lightIndex], light.on);
	if (light.on == false) {
		return;
	}

	shader->Update(shadowUniforms.lightType[lightIndex], static_cast<int>(light.type));
	shader->Update(shadowUniforms.lightColor[lightIndex], ToOpenglStruct(light.color));
	shader->Update(shadowUniforms.lightPower[lightIndex], light.power);
	shader->Update(shadowUniforms.lightKC[lightIndex], light.kC);
	shader->Update(shadowUniforms.lightKL[lightIndex], light.kL);
	shader->Update(shadowUniforms.lightKQ[lightIndex], light.kQ);
	shader->Update(shadowUniforms.lightCosCutoff[lightIndex], light.cosCutoff);
	shader->Update(shadowUniforms.lightCosInner[lightIndex], light.cosInner);
	shader->Update(shadowUniforms.lightExponent[lightIndex], light.exponent);

	if (light.type == Light::LIGHT_TYPE::LIGHT_DIRECTIONAL) { //Directional Light
		Vector3 lightDir = GraphicsManager::GetInstance().viewStack.Top() * -light.transform.GetForward();
		shader->Update(shadowUniforms.lightPosition[lightIndex], ToOpenglStruct(lightDir));
		//cout << lightDir << endl;
	} else if (light.type == Light::LIGHT_TYPE::LIGHT_SPOT) { //Spotlight
		Vector3 lightPosition_cameraspace = GraphicsManager::GetInstance().viewStack.Top() * light.transform.GetPosition();
		shader->Update(shadowUniforms.lightPosition[lightIndex], ToOpenglStruct(lightPosition_cameraspace));
		Vector3 spotDirection_cameraspace = GraphicsManager::GetInstance().viewStack.Top() * light.transform.GetForward();
		shader->Update(shadowUniforms.lightSpotDirection[lightIndex], ToOpenglStruct(spotDirection_cameraspace));
	} else { //Point Light
		Vector3 lightPosition_cameraspace = GraphicsManager::GetInstance().viewStack.Top() * light.transform.GetPosition();
		shader->Update(shadowUniforms.lightPosition[lightIndex], ToOpenglStruct(lightPosition_cameraspace));
		//cout << "Light Position CameraSpace" << lightPosition_cameraspace << endl;
	}

}

void RenderHelper::UpdateLight(Light& light, unsigned int lightIndex) {

	switch (currentShader) {
		case SHADERS::SHADOW:
			UpdateLightDelegate(light, lightIndex, shadowUniforms);
			break;
	}

}

void RenderHelper::SetNumLights(const unsigned num) {

	switch (currentShader) {
		case SHADERS::SHADOW:
			SetNumLightsDelegate(num, shadowUniforms);
			break;
	}

}

void RenderHelper::EnableLight(const bool boolean) {

	switch (currentShader) {

	case SHADERS::SHADOW:
		EnableLightDelegate(boolean, shadowUniforms);
		break;

	}

}

void RenderHelper::EnableFog(const bool boolean) {

	switch (currentShader) {

	case SHADERS::SHADOW:
		EnableFogDelegate(boolean, shadowUniforms);
		break;

	}

}

void RenderHelper::SetAlphaDiscardValue(const float alphaValue) {

	switch (currentShader) {
		case SHADERS::SHADOW:
			GetShader()->Update(shadowUniforms.alphaDiscardValue, alphaValue);
			break;
	}

}

void RenderHelper::RenderMesh(Mesh& mesh, TextureList& texture, const bool& enableLight) {

	switch (currentShader) {
		case SHADERS::SHADOW:
			RenderMeshDelegate(mesh, texture, enableLight, shadowUniforms);
			break;
	}

}

void RenderHelper::RenderMesh(Mesh& mesh, const bool& enableLight) {

	static TextureList texture;
	RenderMesh(mesh, texture, enableLight);

}

void RenderHelper::RenderText(Mesh& mesh, TextureList& texture, const string& text, Color color) {

	switch (currentShader) {
		case SHADERS::SHADOW:
			RenderTextDelegate(mesh, texture, text, color, shadowUniforms);
			break;
	}

}