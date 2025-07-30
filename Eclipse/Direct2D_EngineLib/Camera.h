#pragma once
#include <d2d1.h>
#include "Component.h"
#include "CameraSystem.h"
#include "Vector2.h"
#include "Rect.h"

/* [Camera Conponent]
* 
* TODO :: zoom 등 기능 함수 추가
*/

class Transform;
class Camera : public Component
{
private:
	Transform* transform = nullptr;
	Vector2 viewSize = { 1920.0f, 1080.0f };

	// matrix
	D2D1::Matrix3x2F worldMatrix = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F inverseMatrix = D2D1::Matrix3x2F::Identity();

private:
	// target 
	Transform* target = nullptr;
	float targetTraceSpeed = 100.0f;
	float targetTraceLimitX = 10.0f;
	float targetTraceLimitY = 20.0f;

	// map condition
	bool useMapCondition = false;
	Rect mapRect;

public:
	// main camera
	static Camera* mainCamera;
	static void SetMainCamera(Camera* cam);
	static Camera* GetMainCamera();
	static const D2D1::Matrix3x2F& GetMainInverseMatrix();		// -> transform screen matrix

public:
	// component cycle
	Camera(float width, float height) { viewSize = { width, height }; }
	~Camera() override = default;
	void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
	void Update();
	void OnDestroy_Inner() override final;

public:
	// culling
	bool IsInView(const Vector2& worldPos, const Vector2& boundSize = Vector2(0, 0)) const;

	// get / set
	void SetViewSize(float width, float height) { viewSize.x = width; viewSize.y = height; }
	Vector2 GetViewSize() const { return viewSize; }

public:
	// target trace
	void SetTarget(Transform* target) { this->target = target; }
	void SetTargetTraceSpeed(float speed) { targetTraceSpeed = speed; }
	void SetTargetTraceLimitX(float limit) { targetTraceLimitX = limit; }
	void SetTargetTraceLimitY(float limit) { targetTraceLimitY = limit; }
private:
	void TargetTrace();

public:
	// map condition
	void SetUseMapCondition(bool use) { useMapCondition = use; }
	void SetMapCondition(const Rect rect) { mapRect = rect; useMapCondition = true; }
private:
	void MapBoundaryCondition();

public:
	// screen -> world
	static Vector2 GetScreenToWorldPosition(Vector2 screenPos_unity);
	static Vector2 GetScreenToWorldPosition_D2D(Vector2 screenPos_d2d, Vector2 viewSize);
};

