#include "Chat.h"
#include "../Direct2D_EngineLib/Input.h"

Chat::Chat()
{
	chatText = GameManager::Get().g_playUI->chat_Text;
	curchatCondition = ChatCondition::Quest;
	// 퀘스트 0
	AddChat(0, ChatCondition::Quest, L"달이… 검게 물들었구나.");
	AddChat(0, ChatCondition::Quest, L"어둠 속에서… 타락한 영혼들이 꺠어나고 있다...");
	AddChat(0, ChatCondition::Quest, L"등불이 꺼지지 않기를…");
	AddChat(0, ChatCondition::Quest, L"… 드디어 깨어났군.");
	AddChat(0, ChatCondition::Quest, L"지금이 어떤 밤인지 아나?");
	AddChat(0, ChatCondition::Quest, L"하늘이 뒤집히는 월식의 밤… 그리고 '혼문'이 열리는 순간이다.");
	AddChat(0, ChatCondition::Quest, L"타락한 영혼들이 하늘에서 쏟아지고 있다.");
	AddChat(0, ChatCondition::Quest, L"이번엔… 막아낼 존재가 없어.");
	AddChat(0, ChatCondition::Quest, L"하지만 네 안에, 옛 영웅의 혼이 깃들어 있다.");
	AddChat(0, ChatCondition::Quest, L"그 힘을 깨우려면 먼저 움직일 줄 알아야지.");
	AddChat(0, ChatCondition::Quest, L"일단 몸부터 풀어라. SPACE 바로 점프하고,\nA와 D를 눌러 좌우로 움직일 수 있어.");

	// 퀘스트 1
	AddChat(1, ChatCondition::Quest, L"좌클릭으로 공격이 가능하고,\n공격 중에 집중하면… 시간이 느려진다.");
	AddChat(1, ChatCondition::Quest, L"그리고 빠르게 피하고 싶을 때는 SHIFT로 몸을 날려라.");
	AddChat(1, ChatCondition::Quest, L"이 힘을 잘 활용하도록.");

	// 웨이브 시작 0
	AddChat(0, ChatCondition::Wave, L"Q키로 적의 기운을 흡수할 수 있고,\nE키로 방출하여 강력한 일격을 가할수도 있지.");
	AddChat(0, ChatCondition::Wave, L"이제 앞으로는 네 손에 달렸다.");
	AddChat(0, ChatCondition::Wave, L"그들이 다가오고 있다. 막지 못하면, 더는 되돌릴 수 없다.\n\n(영혼을 20회 처치하세요.)");//퀘스트는 회색으로 가능한지

	// 웨이브 종료 1
	AddChat(1, ChatCondition::Success, L"잠시나마 숨을 돌릴 수 있겠어. 하지만 방심은 금물이야.");
	AddChat(1, ChatCondition::Fail, L"혼들은 여전히 떠돌고 있어. 더 거세게 덮쳐올 거야.");

	// 퀘스트 2
	AddChat(2, ChatCondition::Quest, L"네 안의 힘은 아직 완전하지 않아.\n싸움에서 모은 혼으로 네 능력을 키워라.");
	AddChat(2, ChatCondition::Quest, L"Tab을 눌러 봐. 새로운 힘을 배울 수 있을거야.");

	//  웨이브 시작 1
	AddChat(1, ChatCondition::Wave, L"힘이 조금은 자리 잡았군…\n혼문이 완전히 닫히지 않는 한, 놈들은 끝없이 몰려올 것이다.");
	AddChat(1, ChatCondition::Wave, L"마음을 다잡아라… 숨을 고를 틈은 끝났다.\n\n(연쇄를 10회 진행하세요.)");//퀘스트는 회색으로 가능한지

	// 웨이브 종료 2 
	AddChat(2, ChatCondition::Success, L"좋아, 지금까지는 잘 버텼어. 다음에 올 폭풍을 대비하자.");
	AddChat(2, ChatCondition::Fail, L"이 속도로는 버티기 힘들다. 다음 번에는 반드시 막아내야 한다.");

	// 퀘스트 3
	AddChat(3, ChatCondition::Quest, L"혼을 모아둔 건 잊지 않았겠지?\n새로운 능력을 익혀라.");

	// 웨이브 시작 2
	AddChat(2, ChatCondition::Wave, L"이번엔 타락하지 않은 혼이 내려오고 있어.\n그 영혼을 지켜내라.\n\n(루나를 보호하세요.)");//퀘스트는 회색으로 가능한지

	// 웨이브 종료 3
	AddChat(3, ChatCondition::Success, L"네 덕분에 그 빛은 꺼지지 않았다.");
	AddChat(3, ChatCondition::Fail, L"그 빛이 꺼진 만큼, 이 밤은 더욱 길어질 것이다.");

	// 퀘스트 4
	AddChat(4, ChatCondition::Quest, L"이번 전투로 모은 혼을 잊지 마라.");
	AddChat(4, ChatCondition::Quest, L"더 강해져야 다음에도 지켜낼 수 있다.\n\n(녹스를 처치하세요.)");//퀘스트는 회색으로 가능한지

	// 웨이브 시작 3
	AddChat(3, ChatCondition::Wave, L"이젠, 틈도 없고, 뒤로 물러날 곳도 없어. 그를 막아야해.\n\n(녹스를 처치하세요.)");//퀘스트는 회색으로 가능한지

	// 웨이브 4
	AddChat(4, ChatCondition::Success, L"…끝났다. 달빛이 돌아오고, 혼문이 닫혔다.");
	AddChat(4, ChatCondition::Success, L"너는 이 밤을 끝낸 자, 그리고 수많은 혼을 구한 자로 기억될 것이다.");
	AddChat(4, ChatCondition::Success, L"언젠가 또 다른 밤이 찾아온다면… 너의 이름이 다시 불릴 것이다.");
	AddChat(4, ChatCondition::Success, L"이제 가라. 네가 지켜내 세상 속으로.");

	sequence.push_back({ ChatCondition::Quest, 0 });
	sequence.push_back({ ChatCondition::Quest, 1 });
	sequence.push_back({ ChatCondition::Wave, 0 });
	sequence.push_back({ ChatCondition::Success, 1 });
	sequence.push_back({ ChatCondition::Quest, 2 });
	sequence.push_back({ ChatCondition::Wave, 1 });
	sequence.push_back({ ChatCondition::Success, 2 });
	sequence.push_back({ ChatCondition::Quest, 3 });
	sequence.push_back({ ChatCondition::Wave, 2 });
	sequence.push_back({ ChatCondition::Success, 3 });
	sequence.push_back({ ChatCondition::Quest, 4 });
	sequence.push_back({ ChatCondition::Wave, 3 });
	sequence.push_back({ ChatCondition::Success, 4 });

	SetCondition();
	chatText->screenTextRenderer->SetText(currentLines[chatCount]);
}

void Chat::Awake()
{
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
			GameManager::Get().g_playUI->chatNext_Image->SetActive(false);
		}
	}
}

void Chat::AddChat(int index, ChatCondition condition, const std::wstring& line)
{
	// 기존 그룹이 있는지 검사
	for (auto& group : chatGroups)
	{
		if (group.Index == index && group.condition == condition)
		{
			group.lines.push_back(line);
			return;
		}
	}

	// 없으면 새 그룹 생성
	WaveChatGroup newGroup;
	newGroup.Index = index;
	newGroup.condition = condition;
	newGroup.lines.push_back(line);

	chatGroups.push_back(newGroup);
}

void Chat::SetCondition(ChatCondition chatCondition)
{
	ChatStep seq = sequence[sequenceCount++];
	chatCount = 0;
	finished = false;
	currentLines.clear();
	if (chatCondition == ChatCondition::Quest) chatCondition = seq.condition;
	curchatCondition = seq.condition;

	// index는 퀘스트 번호 또는 웨이브 번호
	for (const auto& group : chatGroups)
	{
		if (group.Index == seq.index && group.condition == chatCondition)
		{
			currentLines.insert(currentLines.end(), group.lines.begin(), group.lines.end());
		}
	}

	chatText->screenTextRenderer->SetText(currentLines[chatCount]);
	if (currentLines.size() == 1) finished = true;
}