#include "Chat.h"
#include "../Direct2D_EngineLib/Input.h"

Chat::Chat()
{
	chatText = GameManager::Get().g_playUI->chat_Text;
	curchatCondition = ChatCondition::Quest;
	// ����Ʈ 0
	AddChat(0, ChatCondition::Quest, L"���̡� �˰� ���������.");
	AddChat(0, ChatCondition::Quest, L"��� �ӿ����� Ÿ���� ��ȥ���� �ƾ�� �ִ�...");
	AddChat(0, ChatCondition::Quest, L"����� ������ �ʱ⸦��");
	AddChat(0, ChatCondition::Quest, L"�� ���� �����.");
	AddChat(0, ChatCondition::Quest, L"������ � ������ �Ƴ�?");
	AddChat(0, ChatCondition::Quest, L"�ϴ��� �������� ������ �㡦 �׸��� 'ȥ��'�� ������ �����̴�.");
	AddChat(0, ChatCondition::Quest, L"Ÿ���� ��ȥ���� �ϴÿ��� ������� �ִ�.");
	AddChat(0, ChatCondition::Quest, L"�̹����� ���Ƴ� ���簡 ����.");
	AddChat(0, ChatCondition::Quest, L"������ �� �ȿ�, �� ������ ȥ�� ���� �ִ�.");
	AddChat(0, ChatCondition::Quest, L"�� ���� ������� ���� ������ �� �˾ƾ���.");
	AddChat(0, ChatCondition::Quest, L"�ϴ� ������ Ǯ���. SPACE �ٷ� �����ϰ�,\nA�� D�� ���� �¿�� ������ �� �־�.");

	// ����Ʈ 1
	AddChat(1, ChatCondition::Quest, L"��Ŭ������ ������ �����ϰ�,\n���� �߿� �����ϸ顦 �ð��� ��������.");
	AddChat(1, ChatCondition::Quest, L"�׸��� ������ ���ϰ� ���� ���� SHIFT�� ���� ������.");
	AddChat(1, ChatCondition::Quest, L"�� ���� �� Ȱ���ϵ���.");

	// ���̺� ���� 0
	AddChat(0, ChatCondition::Wave, L"QŰ�� ���� ����� ����� �� �ְ�,\nEŰ�� �����Ͽ� ������ �ϰ��� ���Ҽ��� ����.");
	AddChat(0, ChatCondition::Wave, L"���� �����δ� �� �տ� �޷ȴ�.");
	AddChat(0, ChatCondition::Wave, L"�׵��� �ٰ����� �ִ�. ���� ���ϸ�, ���� �ǵ��� �� ����.\n\n(��ȥ�� 20ȸ óġ�ϼ���.)");//����Ʈ�� ȸ������ ��������

	// ���̺� ���� 1
	AddChat(1, ChatCondition::Success, L"��ó��� ���� ���� �� �ְھ�. ������ ����� �ݹ��̾�.");
	AddChat(1, ChatCondition::Fail, L"ȥ���� ������ ������ �־�. �� �ż��� ���Ŀ� �ž�.");

	// ����Ʈ 2
	AddChat(2, ChatCondition::Quest, L"�� ���� ���� ���� �������� �ʾ�.\n�ο򿡼� ���� ȥ���� �� �ɷ��� Ű����.");
	AddChat(2, ChatCondition::Quest, L"Tab�� ���� ��. ���ο� ���� ��� �� �����ž�.");

	//  ���̺� ���� 1
	AddChat(1, ChatCondition::Wave, L"���� ������ �ڸ� ��ұ���\nȥ���� ������ ������ �ʴ� ��, ����� ������ ������ ���̴�.");
	AddChat(1, ChatCondition::Wave, L"������ ����ƶ� ���� �� ƴ�� ������.\n\n(���⸦ 10ȸ �����ϼ���.)");//����Ʈ�� ȸ������ ��������

	// ���̺� ���� 2 
	AddChat(2, ChatCondition::Success, L"����, ���ݱ����� �� �����. ������ �� ��ǳ�� �������.");
	AddChat(2, ChatCondition::Fail, L"�� �ӵ��δ� ��Ƽ�� �����. ���� ������ �ݵ�� ���Ƴ��� �Ѵ�.");

	// ����Ʈ 3
	AddChat(3, ChatCondition::Quest, L"ȥ�� ��Ƶ� �� ���� �ʾҰ���?\n���ο� �ɷ��� ������.");

	// ���̺� ���� 2
	AddChat(2, ChatCondition::Wave, L"�̹��� Ÿ������ ���� ȥ�� �������� �־�.\n�� ��ȥ�� ���ѳ���.\n\n(�糪�� ��ȣ�ϼ���.)");//����Ʈ�� ȸ������ ��������

	// ���̺� ���� 3
	AddChat(3, ChatCondition::Success, L"�� ���п� �� ���� ������ �ʾҴ�.");
	AddChat(3, ChatCondition::Fail, L"�� ���� ���� ��ŭ, �� ���� ���� ����� ���̴�.");

	// ����Ʈ 4
	AddChat(4, ChatCondition::Quest, L"�̹� ������ ���� ȥ�� ���� ����.");
	AddChat(4, ChatCondition::Quest, L"�� �������� �������� ���ѳ� �� �ִ�.\n\n(�콺�� óġ�ϼ���.)");//����Ʈ�� ȸ������ ��������

	// ���̺� ���� 3
	AddChat(3, ChatCondition::Wave, L"����, ƴ�� ����, �ڷ� ������ ���� ����. �׸� ���ƾ���.\n\n(�콺�� óġ�ϼ���.)");//����Ʈ�� ȸ������ ��������

	// ���̺� 4
	AddChat(4, ChatCondition::Success, L"��������. �޺��� ���ƿ���, ȥ���� ������.");
	AddChat(4, ChatCondition::Success, L"�ʴ� �� ���� ���� ��, �׸��� ������ ȥ�� ���� �ڷ� ���� ���̴�.");
	AddChat(4, ChatCondition::Success, L"������ �� �ٸ� ���� ã�ƿ´ٸ顦 ���� �̸��� �ٽ� �Ҹ� ���̴�.");
	AddChat(4, ChatCondition::Success, L"���� ����. �װ� ���ѳ� ���� ������.");

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
			GameManager::Get().g_playUI->chatNext_Image->SetActive(false);
		}
	}
}

void Chat::AddChat(int index, ChatCondition condition, const std::wstring& line)
{
	// ���� �׷��� �ִ��� �˻�
	for (auto& group : chatGroups)
	{
		if (group.Index == index && group.condition == condition)
		{
			group.lines.push_back(line);
			return;
		}
	}

	// ������ �� �׷� ����
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

	// index�� ����Ʈ ��ȣ �Ǵ� ���̺� ��ȣ
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