#include "Typer.h"
#include "../Direct2D_EngineLib/ScreenTextRenderer.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include <cstdlib>
#include <ctime>

void Typer::Awake()
{
    // Sound Clip
    typingSound[0] = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/Typing/Typing1.wav");
    typingSound[1] = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/Typing/Typing2.wav");
    typingSound[2] = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/Typing/Typing3.wav");

    // seed
    srand((unsigned int)time(nullptr));
}

void Typer::Update()
{
	TypingDirecting();
}

// 타이핑 연출
// cur_text를 target_text가 될 때까지 typingSpeed대로 한자씩 치는 연출
// 띄어쓰기가 있으면 spacingDelay만큼 쉬었다가 마저 이어친다.
// 타이핑을 모두 완료하면 isTyping이 true가 된다.
void Typer::TypingDirecting()
{
	if (!isTyping || target_text.empty() || !textRenderer || !audioSource)
		return;

	timer += Time::GetDeltaTime();

    if (cur_text.size() < target_text.size())
    {
        wchar_t nextChar = target_text[cur_text.size()];

        // 띄어씌기 딜레이
        float delay = (nextChar == L' ') ? spacingDelay : 1.0f / typingSpeed;

        if (timer >= delay)
        {
            cur_text += nextChar;
            textRenderer->SetText(cur_text);
            timer = 0.0f;

            // sound
		    soundIndex = rand() % SOUND_COUNT;
			audioSource->SetClip(typingSound[soundIndex]);
			audioSource->PlayOneShot();
        }
    }
    else
    {
        // 타이핑 완료
        isTyping = false;
        timer = 0.0f;
    }
}

// 타이핑 시작
void Typer::StartTyping(wstring text)
{
	target_text = text;
	cur_text = L"";
	isTyping = true;
}

// 타이핑 종료
void Typer::EndTyping()
{
	cur_text = target_text;
	textRenderer->SetText(cur_text);
	isTyping = false;
}
