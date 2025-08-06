#include "Chat.h"
#include "../Direct2D_EngineLib/Input.h"

void Chat::Awake()
{
	chatText = GameManager::Get().g_playUI->chat_Text;
	chatCondition = ChatCondition::None;
	// ���̺� 0
	AddChat(0, ChatCondition::None, L"��");
	AddChat(0, ChatCondition::None, L"���̡� �˰� ���������.");
	AddChat(0, ChatCondition::None, L"��� �ӿ����� Ÿ���� ��ȥ���� ���ƿԴ�.");
	AddChat(0, ChatCondition::None, L"����� ������ �ʱ⸦��");
	AddChat(0, ChatCondition::None, L"�׵��� ���� ������ ���� �ǵ��� �� ����.\n��ȥ�� nȸ óġ�ϼ���.");

	// ���̺� 1
	AddChat(1, ChatCondition::Success, L"�״� ���� ��ó��� ���� �� �� �־���.");
	AddChat(1, ChatCondition::Fail, L"ȥ���� ������ ������ ����.");
	AddChat(1, ChatCondition::None, L"������ �� �ż� �ž�.");

	AddChat(1, ChatCondition::None, L"��ȥ�� �� �ȿ� �����־�.");
	AddChat(1, ChatCondition::None, L"�� ȥ��, �װ� ���ϴ� ���·� �ٵ�� ��.");
	AddChat(1, ChatCondition::None, L"���ϴ� ���·� �� ���� ��ĥ �� ���� �ž�.");

	AddChat(1, ChatCondition::None, L"�� ���� ��ҿ��� ���� ���� ȥ�顦");
	AddChat(1, ChatCondition::None, L"�׵��� ������� ������.");
	AddChat(1, ChatCondition::None, L"�̹����� �׵��� �ε��� �� �ְھ�?\n���⸦ nȸ �����ϼ���.");

	// ���̺� 2
	AddChat(2, ChatCondition::Success, L"����, ���ݱ����� �� �����.");
	AddChat(2, ChatCondition::None, L"���ݸ� �� ��Ƽ��, ���� ����.");
	AddChat(2, ChatCondition::Fail, L"������ ���ſ����� ����.");
	AddChat(2, ChatCondition::None, L"������ ���� ���� �� �ƴϾ�.");

	AddChat(2, ChatCondition::None, L"ó�����١� ȥ�� ���� �ξ� �ڿ������� �ٷ� �� �ְ� �Ǿ���.");
	AddChat(2, ChatCondition::None, L"�̹����� ���� �޶�.");
	AddChat(2, ChatCondition::None, L"�� �ȿ����� ������ Ÿ������ ���� ȥ�� �����Ծ�.\n���ε����ϴ� ��ȥ�� nȸ �̸����� óġ�ϼ���.");

	// ���̺� 3
	AddChat(3, ChatCondition::Success, L"��ȭ�� ȥ�鵵�� ����������.");
	AddChat(3, ChatCondition::Fail, L"���� ȥ�̡� �ʹ� ���� ��ĵƳ� ��.");
	AddChat(3, ChatCondition::None, L"����, ƴ�� ����, �ڷ� ������ ���� ����.");
	AddChat(3, ChatCondition::None, L"�׸� ���� ��.\n������ óġ�ϼ���.");

	// ���̺� 4
	AddChat(4, ChatCondition::Success, L"�޵�, �ٽ� ���� ��ã�� �־�.");
	AddChat(4, ChatCondition::Success, L"�� �����. ������.");

	SetCondition(chatCondition);
	chatText->screenTextRenderer->SetText(currentLines[chatCount]);
}
void Chat::Update()
{

}

//�÷��̾� ���ͷ��� �� ȣ�� �Լ�
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
	// ���� �׷��� �ִ��� �˻�
	for (auto& group : chatGroups)
	{
		if (group.waveIndex == waveIndex && group.condition == condition)
		{
			group.lines.push_back(line);
			return;
		}
	}

	// ������ �� �׷� ����
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

	// 1. ����(Success/Fail) ��� ���� �߰�
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

	// 2. None ��� �׻� �߰�
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