#pragma once
#include <vector>
#include "Singleton.h"
using namespace std;

/* [SceneManager 클래스]
* 게임 콘텐츠에서 게임 시작시 전체 씬을 SceneManager에 등록시키고,
* 모든 Scene들을 sceneList로 관리하여 currentScene의 Update를 호출한다.
* SceneManager(this) -> Scene -> GameObject
*/

class Scene;
class SceneManager : public Singleton<SceneManager>
{
private:
	vector<Scene*> sceneList;
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

public:
	SceneManager() = default;
	~SceneManager() = default;

	void Init();
	void Update();
	void UnInit();

	template<typename T>
	void CreateScene()
	{
		Scene* pScene = new T();
		sceneList.push_back(pScene);
	}

	void SetCurrentScene(size_t index);
	Scene* GetCurrentScene();
	void ChangeScene(size_t index);
};

