#pragma once
#include "ICollider.h"
#include "ColliderSystem.h"

/* [CircleCollider Component]
* 원 충돌 영역을 지정하여 충돌 체크를 담당하는 컴포넌트
* transform의 position을 중점으로 offset만큼 떨어뜨린 radius의 원 영역을 가진다
* bound 정보를 Update하며 ColliderSystem에서 sap 알고리즘을 사용하도록 지원한다.
* ICollider를 상속받은 모든 종류의 콜라이더와의 상호작용을 지원한다.
* isTrigger 플래그를 설정하여 Block, Overlap 처리를 지정할 수 있다.
* ColliderSystem에 등록되어 충돌 발생시 충돌 이벤트를 호출하게 되고,
* 각 이벤트 종류에 맞게 Script 컴포넌트를 찾아 이벤트 함수를 호출해준다. -> 유니티처럼 사용 가능하도록
* 
* * Block(collistion mode) 구현 방식 - ContectInfo
* 각 콜라이더에서 충돌 체크 연산시 두 콜라이더가 충돌한 경우에는
* 충돌 지점(point)과 법선벡터(normal)를 구해 map에 콜라이더와 함께 저장한다.
* FinalizeCollision시점에서 OnCollision 충돌이 경우, 해당 ContectInfo를 함께 전달하여 normal에 따라 이동 시도를 제한한다.
*/

class Transform;
class BoxCollider;
class CircleCollider : public ICollider
{
public:
    Transform* transform = nullptr;

    // collider
    Vector2 offset = Vector2::zero;
    float radius = 1.0f;

    // Flatformer일 경우 위로 충돌 보정 무시
    bool isFlatformerCharacter = false;

public:
    // component cycle
    CircleCollider() { colliderType = ColliderType::Circle; }
    ~CircleCollider() override {  }

    void OnEnable_Inner() override final;
    void OnDisable_Inner() override final;
    void OnDestroy_Inner() override final;

private:
    void UpdateBounds() override;
    bool isCollision(ICollider* other, ContactInfo& contact) override;
    void FinalizeCollision() override;

private:
    // collistion cheak
    bool CheckCircleCollision(CircleCollider* other, ContactInfo& contact);
    bool CheckBoxCollision(BoxCollider* other, ContactInfo& contact);

    // raycast hit
    bool Raycast(const Ray& ray, float maxDistance, RaycastHit& hitInfo);

private:
    // collision event
    void OnCollisionEnter(ICollider* other, ContactInfo& contact) override;
    void OnCollisionStay(ICollider* other, ContactInfo& contact) override;
    void OnCollisionExit(ICollider* other, ContactInfo& contact) override;
    void OnTriggerEnter(ICollider* other) override;
    void OnTriggerStay(ICollider* other) override;
    void OnTriggerExit(ICollider* other) override;

public:
    // func
    Vector2 GetCenter() const;
    void DebugColliderDraw() override;
};

