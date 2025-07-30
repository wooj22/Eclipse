#pragma once
#include "ICollider.h"
#include "ColliderSystem.h"

/* [BoxCollider Component]
* �ڽ� ������ �浹 ������ �����Ͽ� �浹 üũ�� ����ϴ� ������Ʈ
* transform�� position�� �������� offset��ŭ ����߸� size�� �ݶ��̴� ������ ������.
* bound ������ Update�ϸ� ICollider�� ��ӹ��� ��� ������ �ݶ��̴����� ��ȣ�ۿ��� �����Ѵ�.
* isTrigger �÷��׸� �����Ͽ� Block, Overlap ó���� ������ �� �ִ�.
* ColliderSystem�� ��ϵǾ� �浹 �߻��� �浹 �̺�Ʈ�� ȣ���ϰ� �ǰ�,
* �� �̺�Ʈ ������ �°� Script ������Ʈ�� ã�� �̺�Ʈ �Լ��� ȣ�����ش�. -> ����Ƽó�� ��� �����ϵ���
* 
* Block(collistion mode) ���� ��� - ContectInfo
* �� �ݶ��̴����� �浹 üũ ����� �� �ݶ��̴��� �浹�� ��쿡��
* �浹 ����(point)�� ��������(normal)�� ���� map�� �ݶ��̴��� �Բ� �����Ѵ�.
* FinalizeCollision�������� OnCollision �浹�� ���, �ش� ContectInfo�� �Բ� �����Ͽ� normal�� ���� �̵� �õ��� �����Ѵ�.
*/

class Transform;
class CircleCollider;
class BoxCollider : public ICollider
{
public:
    Transform* transform = nullptr;

    // collider
    Vector2 offset = Vector2::zero;
    Vector2 size = { 10,10 };
    
public:
    // component cycle
    BoxCollider() { colliderType = ColliderType::Box; }
    ~BoxCollider() override = default;

    void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
    void OnDestroy_Inner() override final;

private:
    void UpdateBounds() override;
    bool isCollision(ICollider* other, ContactInfo& contact) override;
    void FinalizeCollision() override;

private:
    // collision cheak
    bool CheckAABBCollision(BoxCollider* other, ContactInfo& contact);
    bool CheckCircleCollision(CircleCollider* other, ContactInfo& contact);

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