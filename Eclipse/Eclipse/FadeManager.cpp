#include "FadeManager.h"
#include "../Direct2D_EngineLib/ImageRenderer.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"

void FadeManager::Awake()
{
	ir = gameObject->GetComponent<ImageRenderer>();
    currentAlpha = ir->GetAlpha();
}

void FadeManager::Update() 
{
    // test
	if (Input::GetKeyDown('I'))FadeIn();
	if (Input::GetKeyDown('O'))FadeOut();

    // fade
    if (isFading)
    {
        float delta = fadeSpeed * Time::GetDeltaTime();

        if (currentAlpha < targetAlpha)
            currentAlpha = min(currentAlpha + delta, targetAlpha);
        else
            currentAlpha = max(currentAlpha - delta, targetAlpha);

        ir->SetAlpha(currentAlpha);

        // end
        if (fabs(currentAlpha - targetAlpha) <= 0.001f)
        {
            currentAlpha = targetAlpha;
            isFading = false;
        }
    }
}

void FadeManager::FadeIn() 
{
	targetAlpha = 1.0f;
	isFading = true;
}

void FadeManager::FadeOut() 
{
	targetAlpha = 0.0f;
	isFading = true;
}