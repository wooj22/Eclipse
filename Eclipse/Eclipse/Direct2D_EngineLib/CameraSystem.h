#pragma once
#include "Singleton.h"
#include <vector>
using namespace std;

/* CameraSystem의 역할
* 1. Camera 컴포넌트 역행렬 udpate
*/

class Camera;
class CameraSystem : public Singleton<CameraSystem>
{
private:
	vector<Camera*> components;
	vector<Camera*> pending_components;

public:
	void Regist(Camera* component);
	void Unregist(Camera* component);
	void Update();
};

