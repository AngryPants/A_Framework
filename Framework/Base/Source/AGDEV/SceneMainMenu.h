#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "../Scene/Scene.h"
#include "../Graphics/RenderHelper.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Others/Skybox.h"

class SceneMainMenu : public Scene {

private:
	Skybox skybox;

protected:
	//Virtual Function(s)
	virtual void Init();
	virtual void Update(double _deltaTime);
	virtual void Render();
	virtual void Exit();

public:
	SceneMainMenu(const string& _name);
	virtual ~SceneMainMenu();

};

#endif