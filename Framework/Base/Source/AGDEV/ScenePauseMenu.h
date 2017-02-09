#ifndef SCENE_PAUSE_MENU_H
#define SCENE_PAUSE_MENU_H

#include "../Scene/Scene.h"
#include "../Graphics/RenderHelper.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Others/Skybox.h"

class ScenePauseMenu : public Scene {

private:
	Skybox skybox;

protected:
	//Virtual Function(s)
	virtual void Init();
	virtual void Update(double _deltaTime);
	virtual void Render();
	virtual void Exit();

public:
	//Constructor(s) & Destructor
	ScenePauseMenu(const string& _name);
	virtual ~ScenePauseMenu();

};

#endif