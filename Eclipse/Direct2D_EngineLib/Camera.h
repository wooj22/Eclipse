#pragma once
#include <d2d1.h>
#include "Component.h"
#include "CameraSystem.h"
#include "Vector2.h"
#include "Rect.h"

/* [Camera Conponent]
* 
* 
*/

class Transform;
class Camera : public Component
{
private:
	Transform* transform = nullptr;
	Vector2 viewSize = { 1920.0f, 1080.0f };
	float zoom = 1.f;

	// matrix
	D2D1::Matrix3x2F worldMatrix = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F inverseMatrix = D2D1::Matrix3x2F::Identity();

private:
	// target 
	Transform* target = nullptr;
	bool useTargetTrace = false;
	float targetTraceXSpeed = 100.0f;
	float targetTraceYSpeed = 100.0f;
	float targetTraceLimitX = 10.0f;
	float targetTraceLimitY = 20.0f;

	// map condition
	bool useMapCondition = false;
	Rect mapRect;

	// shake
	bool isShaking = false;
	float shakeTime = 0.0f;
	float shakeDuration = 0.0f;
	float shakeAmplitude = 0.0f;
	float shakeFrequency = 0.0f;
	float shakeRoughness = 0.0f;
	float shakeTimer = 0.0f;
	Vector2 shakeOffset = Vector2::zero;

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

	// zoom
	void SetZoom(float z) { zoom = max(z, 0.01f); }
	float GetZoom() const { return zoom; }
	void ZoomIn(float amount) { zoom += amount; if (zoom < 0.01f) zoom = 0.01f; }
	void ZoomOut(float amount) { zoom -= amount; if (zoom < 0.01f) zoom = 0.01f; }

public:
	// target trace
	void UseTargetTrace(bool b){ useTargetTrace = b; }
	void SetTarget(Transform* target) { this->target = target; useTargetTrace = true; }
	void SetTargetTraceXSpeed(float speed) { targetTraceXSpeed = speed; }
	void SetTargetTraceYSpeed(float speed) { targetTraceYSpeed = speed; }
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
	// shake
	void Shake(float amplitude, float frequency, float duration, float roughness = 0);

public:
	// screen -> world
	static Vector2 GetScreenToWorldPosition(Vector2 screenPos_unity);
	static Vector2 GetScreenToWorldPosition_D2D(Vector2 screenPos_d2d, Vector2 viewSize);
};

