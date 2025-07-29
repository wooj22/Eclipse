#pragma once
#include <d2d1.h>
#include "Component.h"
#include "CameraSystem.h"
#include "Vector2.h"

/* [Camera Conponent]
* 
* TODO :: zoom 등 기능 함수 추가
*/

class Transform;
class Camera : public Component
{
private:
	Transform* transform = nullptr;

	// view
	float viewWidth = 1920.0f;
	float viewHeight = 1080.0f;

	// matrix
	D2D1::Matrix3x2F worldMatrix = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F inverseMatrix = D2D1::Matrix3x2F::Identity();

public:
	// main camera
	static Camera* mainCamera;
	static void SetMainCamera(Camera* cam);
	static Camera* GetMainCamera();
	static const D2D1::Matrix3x2F& GetMainInverseMatrix();		// -> transform screen matrix

public:
	// component cycle
	Camera(float width, float height) : viewWidth(width), viewHeight(height) {  }
	~Camera() override = default;
	void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
	void Update();
	void OnDestroy_Inner() override final;

public:
	// culling
	bool IsInView(const Vector2& worldPos, const Vector2& boundSize = Vector2(0, 0)) const;

	// get / set
	void SetViewSize(float width, float height) { viewWidth = width; viewHeight = height; }
	float GetViewWidth() const { return viewWidth; }
	float GetViewHeight() const { return viewHeight; }
};

