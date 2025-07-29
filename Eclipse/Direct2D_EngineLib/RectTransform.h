#pragma once
#include <d2d1_1.h>
#include "Vector2.h"
#include "ITransform.h"
#include "TransformSystem.h"

/*[RectTransform Componenet]
* UI 게임오브젝트 전용 Transform 컴포넌트로,  
* 카메라의 영향을 받지 않고 항상 고정된 화면 좌표로 계산한다.
* 컴포넌트 생성시 Transform System에 등록되어 dirty패턴에 따라 matrix를 계속 계산한다.
* 
* RectTranform도 부모-자식 관계가 존재하며
* Position만 부모의 영향을 받고 size, povot은 독립적으로 운영된다.
*/

class RectTransform : public ITransform
{
private:
    // rect transform
    Vector2 position = { 0, 0 };
    D2D1_SIZE_F size = { 100, 100 };
    D2D1_POINT_2F pivot = { 0.5f, 0.5f };  // rect의 중심점 (unity 좌표계 사용시 left down = 0,0) -> offset 처리

    // parent, children
    RectTransform* parent = nullptr;
    std::vector<RectTransform*> children;

    // this matrix
    D2D1::Matrix3x2F screenLocalMatrix;
    D2D1::Matrix3x2F screenWolrdMatrix;
    D2D1::Matrix3x2F screenMatrix;

    // dirty
    bool isLocalDirty = true;
    bool isWorldDirty = true;

public:
    // matrix
    static D2D1::Matrix3x2F unityMatrix;          // 고정, screen size 필요
    static D2D1::Matrix3x2F renderMatrix;         // 고정

public:
    // component cycle
    RectTransform() = default;
    ~RectTransform() override = default;

    void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
    void Update() override final;
    void OnDestroy_Inner() override final;

public:
    // parent, children
    void SetParent(RectTransform* newParent);
    RectTransform* GetParent(RectTransform* newParent) { return parent; }
    const vector<RectTransform*>& GetChildrens() { return children; }
    void RemoveChild(RectTransform* child);
    void MarkWorldDirty();

    // set
    void SetPosition(const Vector2& position);
    void SetPosition(const float& x, const float& y);
    void SetSize(const float& w, const float& h);
    void SetPivot(const float& x, const float& y);

    // get
    D2D1_SIZE_F GetSize() const { return size; }
    Vector2 GetPosition() const { return position; }
    D2D1_POINT_2F GetPivot() const { return pivot; }
    Vector2 GetWorldPosition() const 
    { 
        if (parent)
            return parent->GetWorldPosition() + position;
        return position;
    }

    
private:
    // matrix
    void MakeScreenLocalMatrix();
	void MakeScreenWorldMatrix();

public:
    //  matrix get
	const D2D1::Matrix3x2F& GetScreenLocalMatrix() { return screenLocalMatrix; }
	const D2D1::Matrix3x2F& GetScreenWorldMatrix() { return screenWolrdMatrix; }
	const D2D1::Matrix3x2F& GetScreenMatrix() { return screenMatrix; }
};