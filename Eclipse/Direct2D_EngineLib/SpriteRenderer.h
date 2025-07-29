#pragma once
#include <wrl/client.h> 
#include <d2d1_1.h>
#include "IRenderer.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "iostream"

/* [Sprite Renderer Conponent]
* <World>�� �̹��� �� ��(sprite)�� render�� ����ϴ� component��
* ������Ʈ ������ RenderSystem�� ��ϵǾ� sprite�� �Լ� render�Ѵ�.
* render�� �ʿ��� transform�� �� ������Ʈ�� ��ϵ� ������Ʈ�� transform�� �����Ϳ� ��� Ȱ���Ѵ�
*/

class Transform;
class SpriteRenderer : public IRenderer
{
private:
	Transform* transform;
	D2D1_RECT_F destRect;
public:
	shared_ptr<Sprite> sprite;		// ���� �ڿ�	
	float alpha = 1.0f;				// ����
	bool flipX = false;				// x�� ����
	bool flipY = false;				// y�� ����

public:
	// component cycle
	SpriteRenderer() = default;
	~SpriteRenderer() override = default;

	void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
	void Update() override final;
	void Render() override final;
	void OnDestroy_Inner() override final;
};

