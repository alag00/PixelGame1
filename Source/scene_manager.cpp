#include "scene_manager.h"

SceneManager::SceneManager() {
	MainMenu* mainMenu = new MainMenu();
	currentScene = mainMenu;
	currentScene->LoadScene(gameInfo);
}
SceneManager::~SceneManager()
{
	delete currentScene;
	currentScene = nullptr;
}
void SceneManager::Update() {

	if (currentScene->Update())
	{
		ChangeScene(currentScene->GetNewScene());
	}
	currentScene->Render();
}
void SceneManager::ChangeScene(SCENE_TYPE sceneType)
{
	currentScene->LeaveScene(gameInfo);
	delete currentScene;
	currentScene = nullptr;

	switch (sceneType)
	{
	case SCENE_TYPE::MAIN_MENU:
		MainMenu* mainMenu;
		mainMenu = new MainMenu();
		currentScene = mainMenu;
		currentScene->LoadScene(gameInfo);
		break;
	case SCENE_TYPE::HUB:
	{

		Hub* hubLevel;
		hubLevel = new Hub();
		currentScene = hubLevel;
		currentScene->LoadScene(gameInfo);
		break;
	}
	case SCENE_TYPE::DUNGEON_LEVEL:
		Level* dungeonLevel;
		dungeonLevel = new Level();
		currentScene = dungeonLevel;
		currentScene->LoadScene(gameInfo);
		break;
	case SCENE_TYPE::NONE:
		// Something is wrong
		// Restart Game Or Terminate Application
		break;
	}
}
