#include "Chat.h"
#include "../Direct2D_EngineLib/Input.h"

void Chat::Awake()
{
	chatText = GameManager::Get().g_playUI->chat_Text;
	chatCondition = ChatCondition::None;
	// 웨이브 0
	AddChat(0, ChatCondition::None, L"…");
	AddChat(0, ChatCondition::None, L"달이… 검게 물들었구나.");
	AddChat(0, ChatCondition::None, L"어둠 속에서… 타락한 영혼들이 돌아왔다.");
	AddChat(0, ChatCondition::None, L"등불이 꺼지지 않기를…");
	AddChat(0, ChatCondition::None, L"그들을 막지 않으면 더는 되돌릴 수 없어.\n영혼을 n회 처치하세요.");

	// 웨이브 1
	AddChat(1, ChatCondition::Success, L"그대 덕에 잠시나마 숨을 쉴 수 있었어.");
	AddChat(1, ChatCondition::Fail, L"혼들은 여전히 떠돌고 있지.");
	AddChat(1, ChatCondition::None, L"다음은 더 거셀 거야.");

	AddChat(1, ChatCondition::None, L"…혼이 네 안에 남아있어.");
	AddChat(1, ChatCondition::None, L"그 혼을, 네가 원하는 형태로 다듬어 봐.");
	AddChat(1, ChatCondition::None, L"원하는 형태로 그 힘을 펼칠 수 있을 거야.");

	AddChat(1, ChatCondition::None, L"더 깊은 어둠에서 길을 잃은 혼들…");
	AddChat(1, ChatCondition::None, L"그들은 기억조차 없겠지.");
	AddChat(1, ChatCondition::None, L"이번에도 그들을 인도할 수 있겠어?\n연쇄를 n회 진행하세요.");

	// 웨이브 2
	AddChat(2, ChatCondition::Success, L"좋아, 지금까지는 잘 버텼어.");
	AddChat(2, ChatCondition::None, L"조금만 더 버티면, 끝이 보여.");
	AddChat(2, ChatCondition::Fail, L"…점점 버거워지는 거지.");
	AddChat(2, ChatCondition::None, L"하지만 아직 끝난 건 아니야.");

	AddChat(2, ChatCondition::None, L"처음보다… 혼의 힘을 훨씬 자연스럽게 다룰 수 있게 되었어.");
	AddChat(2, ChatCondition::None, L"이번은… 뭔가 달라.");
	AddChat(2, ChatCondition::None, L"그 안에서… 완전히 타락하지 않은 혼이 내려왔어.\n새로등장하는 영혼을 n회 미만으로 처치하세요.");

	// 웨이브 3
	AddChat(3, ChatCondition::Success, L"정화된 혼들도… 조용해졌어.");
	AddChat(3, ChatCondition::Fail, L"…그 혼이… 너무 깊이 잠식됐나 봐.");
	AddChat(3, ChatCondition::None, L"이젠, 틈도 없고, 뒤로 물러날 곳도 없어.");
	AddChat(3, ChatCondition::None, L"그를 막아 줘.\n보스를 처치하세요.");

	// 웨이브 4
	AddChat(4, ChatCondition::Success, L"달도, 다시 빛을 되찾고 있어.");
	AddChat(4, ChatCondition::Success, L"잘 해줬어. 정말로.");

	SetCondition(chatCondition);
	chatText->screenTextRenderer->SetText(currentLines[chatCount]);
}
void Chat::Update()
{

}

//플레이어 인터렉션 시 호출 함수
void Chat::NextChat()
{
	if (!finished)
	{
		chatCount++;
		chatText->screenTextRenderer->SetText(currentLines[chatCount]);
		if (chatCount == currentLines.size() - 1)
		{
			finished = true;
			GameManager::Get().g_playUI->chat_Button->SetActive(true);
		}
	}
}

void Chat::AddChat(int waveIndex, ChatCondition condition, const std::wstring& line)
{
	// 기존 그룹이 있는지 검사
	for (auto& group : chatGroups)
	{
		if (group.waveIndex == waveIndex && group.condition == condition)
		{
			group.lines.push_back(line);
			return;
		}
	}

	// 없으면 새 그룹 생성
	WaveChatGroup newGroup;
	newGroup.waveIndex = waveIndex;
	newGroup.condition = condition;
	newGroup.lines.push_back(line);

	chatGroups.push_back(newGroup);
}

void Chat::SetCondition(ChatCondition condition)
{
	chatCondition = condition;
	chatCount = 0;
	finished = false;
	currentLines.clear();

	// 1. 조건(Success/Fail) 대사 먼저 추가
	if (chatCondition != ChatCondition::None)
	{
		for (const auto& group : chatGroups)
		{
			bool gd = group.waveIndex == GameManager::Get().waveCount;
			bool gsd = group.condition == chatCondition;

			if (group.waveIndex == GameManager::Get().waveCount && group.condition == chatCondition)
			{
				currentLines.insert(currentLines.end(), group.lines.begin(), group.lines.end());
				break;
			}
		}
	}

	// 2. None 대사 항상 추가
	for (const auto& group : chatGroups)
	{
		if (group.waveIndex == GameManager::Get().waveCount && group.condition == ChatCondition::None)
		{
			currentLines.insert(currentLines.end(), group.lines.begin(), group.lines.end());
			break;
		}
	}

	chatText->screenTextRenderer->SetText(currentLines[chatCount]);
}