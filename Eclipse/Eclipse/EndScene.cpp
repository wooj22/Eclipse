#include "EndScene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void EndScene::Awake()
{
	// camera init
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	cam->AddComponent<Camera>(1920, 1080);

	// 화면 아래에서 시작하도록 초기 Y 위치 설정 (화면 밖 아래쪽)
	float startY = -600.0f;  // 화면 아래쪽 밖에서 시작

	// create credits - 기획 (모든 텍스트 같은 크기)
	auto planningTitle = CreateObject<UI_Text>();
	planningTitle->rectTransform->SetPosition(0, startY);
	planningTitle->rectTransform->SetSize(1920, 80);
	planningTitle->screenTextRenderer->SetFontSize(60);  // 이름과 같은 크기
	planningTitle->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	planningTitle->screenTextRenderer->SetText(L"기획");
	planningTitle->screenTextRenderer->SetFontName(L"덕온공주체");
	credits.push_back(planningTitle);

	auto planningTeam = CreateObject<UI_Text>();
	planningTeam->rectTransform->SetPosition(0, startY - 100);
	planningTeam->rectTransform->SetSize(1920, 80);
	planningTeam->screenTextRenderer->SetFontSize(60);
	planningTeam->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	planningTeam->screenTextRenderer->SetText(L"윤동준  조현정");
	planningTeam->screenTextRenderer->SetFontName(L"덕온공주체");
	credits.push_back(planningTeam);

	// create credits - 아트 (더 큰 간격)
	auto artTitle = CreateObject<UI_Text>();
	artTitle->rectTransform->SetPosition(0, startY - 300);
	artTitle->rectTransform->SetSize(1920, 80);
	artTitle->screenTextRenderer->SetFontSize(60);  // 이름과 같은 크기
	artTitle->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	artTitle->screenTextRenderer->SetText(L"아트");
	artTitle->screenTextRenderer->SetFontName(L"덕온공주체");
	credits.push_back(artTitle);

	auto artTeam = CreateObject<UI_Text>();
	artTeam->rectTransform->SetPosition(0, startY - 400);
	artTeam->rectTransform->SetSize(1920, 80);
	artTeam->screenTextRenderer->SetFontSize(60);
	artTeam->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	artTeam->screenTextRenderer->SetText(L"손민경  강혜빈");
	artTeam->screenTextRenderer->SetFontName(L"덕온공주체");
	credits.push_back(artTeam);

	// create credits - 개발 (더 큰 간격)
	auto devTitle = CreateObject<UI_Text>();
	devTitle->rectTransform->SetPosition(0, startY - 600);
	devTitle->rectTransform->SetSize(1920, 80);
	devTitle->screenTextRenderer->SetFontSize(60);  // 이름과 같은 크기
	devTitle->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	devTitle->screenTextRenderer->SetText(L"개발");
	devTitle->screenTextRenderer->SetFontName(L"덕온공주체");
	credits.push_back(devTitle);

	// 개발팀 첫 번째 줄
	auto devTeam1 = CreateObject<UI_Text>();
	devTeam1->rectTransform->SetPosition(0, startY - 700);
	devTeam1->rectTransform->SetSize(1920, 80);
	devTeam1->screenTextRenderer->SetFontSize(60);
	devTeam1->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	devTeam1->screenTextRenderer->SetText(L"장아론  문선민");
	devTeam1->screenTextRenderer->SetFontName(L"덕온공주체");
	credits.push_back(devTeam1);

	// 개발팀 두 번째 줄
	auto devTeam2 = CreateObject<UI_Text>();
	devTeam2->rectTransform->SetPosition(0, startY - 800);
	devTeam2->rectTransform->SetSize(1920, 80);
	devTeam2->screenTextRenderer->SetFontSize(60);
	devTeam2->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	devTeam2->screenTextRenderer->SetText(L"양우정  한윤모");
	devTeam2->screenTextRenderer->SetFontName(L"덕온공주체");
	credits.push_back(devTeam2);
}

void EndScene::Start()
{
	// game object -> SceneStart()
	__super::Start();
}

void EndScene::Update()
{
	// game object -> Update()
	__super::Update();

	// 스페이스바로 빠른 스크롤
	float currentSpeed = 100.0f;  // 기본 속도를 느리게
	if (Input::GetKey(' '))
	{
		currentSpeed = 300.0f;  // 스피드 조정
	}

	// 크레딧 스크롤 (아래에서 위로 올라가도록)
	scrollY += currentSpeed * Time::GetDeltaTime();

	// 모든 크레딧 위치 업데이트 - 위로 이동
	for (size_t i = 0; i < credits.size(); ++i)
	{
		auto currentPos = credits[i]->rectTransform->GetPosition();
		credits[i]->rectTransform->SetPosition(currentPos.x, currentPos.y + currentSpeed * Time::GetDeltaTime());
	}

	//한윤모가 화면 위로 올라간 후 타이머 시작 (마지막 크레딧이 화면 위로 올라갔는지 확인)
	if (!isEnded && credits.size() > 0)
	{
		auto lastCreditPos = credits[credits.size() - 1]->rectTransform->GetPosition();
		if(lastCreditPos.y > 600.0f)  // 화면 위로 올라갔는지 확인
		{
			isEnded = true;  // 크레딧이 끝났음을 표시
			
		}
	}

//타이머 실행 및 씬 전환
	if (isEnded)
	{
		endTimer+= Time::GetDeltaTime();
		if (endTimer >= 0.5f)  // 5초 후에 씬 전환
		{
			SceneManager::Get().ChangeScene(EclipseApp::MENU);  // 메뉴 씬으로 전환
		}
	}

	// scene change
	if (Input::GetKeyDown('1'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::MOON);
	}
	if (Input::GetKeyDown('2'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::YUNMO);
	}
	if (Input::GetKeyDown('3'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::ARON);
	}
	if (Input::GetKeyDown('4'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::WOO);
	}
	if (Input::GetKeyDown('5'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::MENU);
	}
	if (Input::GetKeyDown('6'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::PLAY);
	}
	if (Input::GetKeyDown('7'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::END);
	}
}

void EndScene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}