#include "PhongShader.h"
#include "../../GraphicsUtility.h"
#include "../../GraphicsManager.h"
#include <string>

using namespace std;

namespace PhongShader {

	void LoadShader(const string& shaderName, PhongShader::Uniforms& uniforms) {
		if (!GraphicsManager::GetInstance().HasShader(shaderName)) {
			GraphicsManager::GetInstance().AddShader(shaderName, "Shader//Phong//Phong.vertexshader", "Shader//Phong//Phong.fragmentshader");
			GraphicsManager::GetInstance().SetActiveShader(shaderName);
			ShaderProgram* shaderProgram = GraphicsManager::GetInstance().GetActiveShader();

			uniforms.modelView = shaderProgram->GetUniformID("MV");
			uniforms.modelViewProjection = shaderProgram->GetUniformID("MVP");
			uniforms.modelViewInverseTranspose = shaderProgram->GetUniformID("MV_inverse_transpose");
			uniforms.ambient = shaderProgram->GetUniformID("material.kAmbient");
			uniforms.diffuse = shaderProgram->GetUniformID("material.kDiffuse");
			uniforms.specular = shaderProgram->GetUniformID("material.kSpecular");
			uniforms.shininess = shaderProgram->GetUniformID("material.kShininess");
			uniforms.colorTextureEnabled = shaderProgram->GetUniformID("colorTextureEnabled");
			uniforms.colorTexture = shaderProgram->GetUniformID("colorTexture");
			uniforms.textColor = shaderProgram->GetUniformID("textColor");
			uniforms.textEnabled = shaderProgram->GetUniformID("textEnabled");
			uniforms.fogEnabled = shaderProgram->GetUniformID("fogParam.enabled");
			uniforms.alphaDiscardValue = shaderProgram->GetUniformID("alphaDiscardValue");
			uniforms.textureOffset = shaderProgram->GetUniformID("textureOffset");
			uniforms.textureScale = shaderProgram->GetUniformID("textureScale");

			uniforms.numLights = shaderProgram->GetUniformID("numLights");
			uniforms.lightEnabled = shaderProgram->GetUniformID("lightEnabled");
			for (unsigned int i = 0; i < PhongShader::Uniforms::MAX_LIGHTS; ++i) {
				uniforms.lightOn[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].on");
				uniforms.lightType[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].type");
				uniforms.lightPosition[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].position_cameraspace");
				uniforms.lightColor[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].color");
				uniforms.lightPower[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].power");
				uniforms.lightKC[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].kC");
				uniforms.lightKL[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].kL");
				uniforms.lightKQ[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].kQ");
				uniforms.lightSpotDirection[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].spotDirection");
				uniforms.lightCosCutoff[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].cosCutoff");
				uniforms.lightCosInner[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].cosInner");
				uniforms.lightExponent[i] = shaderProgram->GetUniformID("lights[" + to_string(i) + "].exponent");
			}
		} else {
			GraphicsManager::GetInstance().SetActiveShader(shaderName);
		}
	}

	void DeleteShader(const string& shaderName) {
		if (GraphicsManager::GetInstance().HasShader(shaderName)) {
			GraphicsManager::GetInstance().RemoveShader(shaderName);
		}
	}

	void SetAlphaDiscardValue(const float alphaValue, const PhongShader::Uniforms& uniforms) {
		GetShader()->Update(uniforms.alphaDiscardValue, alphaValue);
	}

	void EnableFog(const bool boolean, const PhongShader::Uniforms& uniforms) {
		GetShader()->Update(uniforms.fogEnabled, boolean);;
	}

	void EnableLight(const bool boolean, const PhongShader::Uniforms& uniforms) {
		GetShader()->Update(uniforms.lightEnabled, boolean);;
	}

	void SetNumLights(const int numLight, const PhongShader::Uniforms& uniforms) {
		GetShader()->Update(uniforms.numLights, numLight);
	}

	void UpdateLight(Light& light, Transform& transform, const unsigned int lightIndex, const PhongShader::Uniforms& uniforms) {
		ShaderProgram* shader = GetShader();
		shader->Update(uniforms.lightOn[lightIndex], light.IsActive());
		if (light.IsActive() == false) {
			return;
		}
		shader->Update(uniforms.lightType[lightIndex], static_cast<int>(light.type));
		shader->Update(uniforms.lightColor[lightIndex], ToOpenglStruct(light.color));
		shader->Update(uniforms.lightPower[lightIndex], light.power);
		shader->Update(uniforms.lightKC[lightIndex], light.kC);
		shader->Update(uniforms.lightKL[lightIndex], light.kL);
		shader->Update(uniforms.lightKQ[lightIndex], light.kQ);
		shader->Update(uniforms.lightCosCutoff[lightIndex], light.cosCutoff);
		shader->Update(uniforms.lightCosInner[lightIndex], light.cosInner);
		shader->Update(uniforms.lightExponent[lightIndex], light.exponent);

		if (light.type == Light::LIGHT_TYPE::LIGHT_DIRECTIONAL) { //Directional Light
			Vector3 lightDir = GraphicsManager::GetInstance().viewStack.Top() * -transform.GetForward();
			shader->Update(uniforms.lightPosition[lightIndex], ToOpenglStruct(lightDir));
		} else if (light.type == Light::LIGHT_TYPE::LIGHT_SPOT) { //Spotlight
			Vector3 lightPosition_cameraspace = GraphicsManager::GetInstance().viewStack.Top() * transform.GetPosition();
			shader->Update(uniforms.lightPosition[lightIndex], ToOpenglStruct(lightPosition_cameraspace));
			Vector3 spotDirection_cameraspace = GraphicsManager::GetInstance().viewStack.Top() * transform.GetForward();
			shader->Update(uniforms.lightSpotDirection[lightIndex], ToOpenglStruct(spotDirection_cameraspace));
		} else { //Point Light
			Vector3 lightPosition_cameraspace = GraphicsManager::GetInstance().viewStack.Top() * transform.GetPosition();
			shader->Update(uniforms.lightPosition[lightIndex], ToOpenglStruct(lightPosition_cameraspace));
		}
	}
			
	void TurnOffLight(const unsigned int lightIndex, const PhongShader::Uniforms& uniforms) {
		ShaderProgram* shader = GetShader();
		shader->Update(uniforms.lightOn[lightIndex], false);
	}

	/*void RenderText(Mesh& mesh, const TextureList& textureList, const string& text, Color color, bool _lightEnabled, const PhongShader::Uniforms& uniforms) {
		ShaderProgram* shaderProgram = GetShader();
		shaderProgram->Update(uniforms.textEnabled, true);
		shaderProgram->Update(uniforms.textColor, ToOpenglStruct(color), 1);
		shaderProgram->Update(uniforms.lightEnabled, _lightEnabled);
		
		int booleans[MAX_TEXTURES] = { 1, 0, 0, 0, 0, 0, 0, 0 }; // <-Might have problems here. Check later.
		int ids[MAX_TEXTURES] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		shaderProgram->Update(uniforms.colorTextureEnabled, booleans, MAX_TEXTURES);
		shaderProgram->Update(uniforms.colorTexture, ids, MAX_TEXTURES);
		
		//Update Texture Coordinates
		ShaderProgram::FloatVec2 textureOffset;
		textureOffset.vec[0] = mesh.textureOffset[0];
		textureOffset.vec[1] = mesh.textureOffset[1];
		shaderProgram->Update(uniforms.textureOffset, textureOffset);
		ShaderProgram::FloatVec2 textureScale;
		textureScale.vec[0] = mesh.textureScale[0];
		textureScale.vec[1] = mesh.textureScale[1];
		shaderProgram->Update(uniforms.textureScale, textureScale);
	
		shaderProgram->SetActiveTexture(0);
		shaderProgram->BindTexture(textureList.textureArray[0]);
	
		for (unsigned i = 0; i < text.length(); ++i) {
			Mtx44 characterSpacing;
			characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
			Mtx44 MVP = GraphicsManager::GetInstance().projectionStack.Top() * GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top() * characterSpacing;
			shaderProgram->Update(uniforms.modelViewProjection, false, ToOpenglStruct(MVP));
			mesh.Render((unsigned)text[i] * 6, 6);
		}
	
		shaderProgram->UnbindTexture();
		shaderProgram->Update(uniforms.textEnabled, false);
	}*/
	
	void RenderMesh(Mesh& mesh, const TextureList& textureList, const bool& enableLight, const PhongShader::Uniforms& uniforms) {
		ShaderProgram* shaderProgram = GetShader();	
		Mtx44 MVP, modelView, modelView_inverse_transpose;
		MVP = GraphicsManager::GetInstance().projectionStack.Top() * GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top();
	
		shaderProgram->Update(uniforms.textEnabled, false);
		shaderProgram->Update(uniforms.lightEnabled, enableLight);

		shaderProgram->Update(uniforms.modelViewProjection, false, ToOpenglStruct(MVP));
		modelView = GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top();
		shaderProgram->Update(uniforms.modelView, false, ToOpenglStruct(modelView));
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		shaderProgram->Update(uniforms.modelViewInverseTranspose, false, ToOpenglStruct(modelView_inverse_transpose));
			
		//Update Texture Coordinates
		ShaderProgram::FloatVec2 textureOffset;
		textureOffset.vec[0] = mesh.textureOffset[0];
		textureOffset.vec[1] = mesh.textureOffset[1];
		shaderProgram->Update(uniforms.textureOffset, textureOffset);
	
		ShaderProgram::FloatVec2 textureScale;
		textureScale.vec[0] = mesh.textureScale[0];
		textureScale.vec[1] = mesh.textureScale[1];
		shaderProgram->Update(uniforms.textureScale, textureScale);
	
		if (enableLight) {
			//Load Material
			shaderProgram->Update(uniforms.ambient, ToOpenglStruct(mesh.material.kAmbient));
			shaderProgram->Update(uniforms.diffuse, ToOpenglStruct(mesh.material.kDiffuse));
			shaderProgram->Update(uniforms.specular, ToOpenglStruct(mesh.material.kSpecular));
			shaderProgram->Update(uniforms.shininess, mesh.material.kShininess);
		}
	
		int booleans[MAX_TEXTURES];
		unsigned ids[MAX_TEXTURES] = {0, 1, 2, 3, 4, 5, 6, 7};	
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			if (textureList.textureArray[i] > 0) {
				shaderProgram->SetActiveTexture(i);
				shaderProgram->BindTexture(textureList.textureArray[i]);
				booleans[i] = true;
			} else {
				booleans[i] = false;
			}
			shaderProgram->Update(uniforms.colorTexture, ids, MAX_TEXTURES);
			shaderProgram->Update(uniforms.colorTextureEnabled, booleans, MAX_TEXTURES);
		}
	
		mesh.Render();
	
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			if (textureList.textureArray[i] > 0) {
				shaderProgram->UnbindTexture();
			}
		}
	}

	void RenderText(Mesh& mesh, const TextureList& textureList, const string& text, Color color, bool _lightEnabled, bool centralise, const PhongShader::Uniforms& uniforms) {
		ShaderProgram* shaderProgram = GetShader();	
		shaderProgram->Update(uniforms.textEnabled, true);
		shaderProgram->Update(uniforms.textColor, ToOpenglStruct(color), 1);
		shaderProgram->Update(uniforms.lightEnabled, _lightEnabled);

		Mtx44 MVP, modelView, modelView_inverse_transpose;
		MVP = GraphicsManager::GetInstance().projectionStack.Top() * GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top();

		shaderProgram->Update(uniforms.modelViewProjection, false, ToOpenglStruct(MVP));
		modelView = GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top();
		shaderProgram->Update(uniforms.modelView, false, ToOpenglStruct(modelView));
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		shaderProgram->Update(uniforms.modelViewInverseTranspose, false, ToOpenglStruct(modelView_inverse_transpose));
		shaderProgram->Update(uniforms.lightEnabled, _lightEnabled);

		//Update Texture Coordinates
		ShaderProgram::FloatVec2 textureOffset;
		textureOffset.vec[0] = mesh.textureOffset[0];
		textureOffset.vec[1] = mesh.textureOffset[1];
		shaderProgram->Update(uniforms.textureOffset, textureOffset);
	
		ShaderProgram::FloatVec2 textureScale;
		textureScale.vec[0] = mesh.textureScale[0];
		textureScale.vec[1] = mesh.textureScale[1];
		shaderProgram->Update(uniforms.textureScale, textureScale);
	
		if (_lightEnabled) {
			//Load Material
			shaderProgram->Update(uniforms.ambient, ToOpenglStruct(mesh.material.kAmbient));
			shaderProgram->Update(uniforms.diffuse, ToOpenglStruct(mesh.material.kDiffuse));
			shaderProgram->Update(uniforms.specular, ToOpenglStruct(mesh.material.kSpecular));
			shaderProgram->Update(uniforms.shininess, mesh.material.kShininess);
		}
	
		int booleans[MAX_TEXTURES];
		unsigned ids[MAX_TEXTURES] = {0, 1, 2, 3, 4, 5, 6, 7};	
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			if (textureList.textureArray[i] > 0) {
				shaderProgram->SetActiveTexture(i);
				shaderProgram->BindTexture(textureList.textureArray[i]);
				booleans[i] = true;
			} else {
				booleans[i] = false;
			}
			shaderProgram->Update(uniforms.colorTexture, ids, MAX_TEXTURES);
			shaderProgram->Update(uniforms.colorTextureEnabled, booleans, MAX_TEXTURES);
		}
	
		float spacingDistance = 1.0f;
		for (unsigned int i = 0; i < text.length(); ++i) {			
			Mtx44 characterSpacing;
			float offset;
			if (centralise) {
				offset = (i * spacingDistance) - (static_cast<float>(text.length() - 1) * 0.5f);
			} else {
				offset = static_cast<float>(i) * spacingDistance; //1.0f is the spacing of each character, you may change this value
			}
			characterSpacing.SetToTranslation(offset, 0, 0);
			Mtx44 MVP = GraphicsManager::GetInstance().projectionStack.Top() * GraphicsManager::GetInstance().viewStack.Top() * GraphicsManager::GetInstance().modelStack.Top() * characterSpacing;
			shaderProgram->Update(uniforms.modelViewProjection, false, ToOpenglStruct(MVP));
			mesh.Render((unsigned)text[i] * 6, 6);
		}
	
		for (unsigned int i = 0; i < MAX_TEXTURES; ++i) {
			if (textureList.textureArray[i] > 0) {
				shaderProgram->UnbindTexture();
			}
		}
	}

}