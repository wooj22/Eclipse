#include "Platform.h"
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "../Direct2D_EngineLib/SceneManager.h"

Platform::Platform()
{
	// [ ground ]
	ground = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
	ground->name = "Ground";
	ground->tag = "EndLine";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -1050.0f);

	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 2560.0f, 300.0f };
	ground_col->isFlatform = true;

	// ¸Ê ¹Ù±ù °æ°è 
	mapEdge_l = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
	mapEdge_l->AddComponent<Transform>()->SetPosition(-1280.0f - 150.0f, 0.0f);
	mapEdge_l_col = mapEdge_l->AddComponent<BoxCollider>();
	mapEdge_l_col->size = { 300.0f, 1920.0f };

	mapEdge_r = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
	mapEdge_r->AddComponent<Transform>()->SetPosition(1280.0f + 150.0f, 0.0f);
	mapEdge_r_col = mapEdge_r->AddComponent<BoxCollider>();
	mapEdge_r_col->size = { 300.0f, 1920.0f };


	// [ Platform ] : ¿ÞÂÊ ÇÏ´ÜºÎÅÍ

	// 1
	box_object1 = SceneManager::Get().GetCurrentScene()->CreateObject<BoxObject>({ -700.0f, -835.0f });

	box_object2 = SceneManager::Get().GetCurrentScene()->CreateObject<BoxObject>({ 900.0f, -835.0f });


	// 2
	platform_long1 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Long_Grass>({ -800.0f, -650.0f });
	cemetery1 = SceneManager::Get().GetCurrentScene()->CreateObject<Cemetery>({ -1000.0f, -550.0f });
	platform_middle1 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Middle>({ 350.0f, -650.0f });


	// 3
	platform_short1 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Short_Grass>({ -150.0f, -500.0f });
	platform_middle2 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Middle_Grass>({ 850.0f, -500.0f });
	platform_wall1 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Wall_Grass>({ 1050.0f, -280.0f });


	// 4
	platform_middle3 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Middle_Grass>({ -600.0f, -320.0f });
	cemetery2 = SceneManager::Get().GetCurrentScene()->CreateObject<Cemetery>({ -750.0f, -220.0f });
	platform_middle4 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Middle_Grass>({ 250.0f, -320.0f });
	box_object3 = SceneManager::Get().GetCurrentScene()->CreateObject<BoxObject>({ 110.0f, -235.0f });


	// 5
	platform_short2 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Short>({ 750.0f, -170.0f });

	// 6
	platform_short3 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Short>({ -1000.0f, -100.0f });
	platform_wall2 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Wall>({ -1110.0f, 80.0f });
	platform_wall2->GetComponent<Transform>()->SetScale(0.5, 0.4);
	platform_wall2->GetComponent<BoxCollider>()->size = { 100.0f, 720.0f };
	platform_short4 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Short>({ 300.0f, -20.0f });


	// 7
	platform_middle5 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Middle>({ -550.0f, 120.0f });
	box_object4 = SceneManager::Get().GetCurrentScene()->CreateObject<BoxObject>({ -400.0f, 205.0f });
	platform_long2 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Long_Grass>({ 950.0f, 120.0f });
	cemetery3 = SceneManager::Get().GetCurrentScene()->CreateObject<Cemetery>({ 770.0f, 220.0f });


	// 8
	platform_short5 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Short>({ -850.0f, 350.0f });
	platform_middle6 = SceneManager::Get().GetCurrentScene()->CreateObject<Platform_Middle>({ 500.0f, 350.0f });
}