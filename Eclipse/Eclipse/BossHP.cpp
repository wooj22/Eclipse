#include "BossHP.h"
#include "../Direct2D_EngineLib/Input.h"

void BossHP::Awake()
{
	rectTransform = AddComponent<RectTransform>();
	hpBarEmpty = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	hpBarFill = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();

	hpBarEmpty->rectTransform->SetParent(rectTransform);
	hpBarFill->rectTransform->SetParent(rectTransform);
}

void BossHP::SceneStart()
{
	hpBarFill->rectTransform->SetParent(hpBarEmpty->rectTransform);
	hpBarEmpty->rectTransform->SetPosition(0, 400);
	hpBarEmpty->rectTransform->SetSize(1400, 50);
	hpBarFill->rectTransform->SetSize(1400, 50);
	auto bossHPEmptyTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BossHPEmpty.png");
	hpBarEmpty->imageRenderer->sprite = ResourceManager::Get().CreateSprite(bossHPEmptyTexture, "BossHPEmptyTexture");
	auto bossHPFillTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BossHPFill.png");
	hpBarFill->imageRenderer->sprite = ResourceManager::Get().CreateSprite(bossHPFillTexture, "BossHPFillTexture");
	hpBarFill->imageRenderer->fillType = FillType::Horizontal;


	SetActive(false);
}

void BossHP::Update()
{
	if (Input::GetKeyDown('K'))
	{
		hpBarFill->imageRenderer->fillAmount = 0.5;
	}
}

void BossHP::Destroyed()
{

}

void BossHP::RefreshBossHp(float hp)
{
	hpBarFill->imageRenderer->fillAmount = hp;
}