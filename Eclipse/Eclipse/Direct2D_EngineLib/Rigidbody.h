#pragma once
#include "Component.h"
#include "PhysicsSystem.h"
#include "Vector2.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

/* [Rigidbody Component]
* ���� �Ի��� ����ϴ� ������Ʈ��, Transform�� �����Ͽ� �̵���Ų��.
* �ӵ�, ���ӵ�, ����, �߷�, ���� ���� ���� ���� �Ӽ��� �����ϸ�
* PhysicsSystem�� ��ϵǾ� FixedUpdate()�ֱ�� ������ ó���Ѵ�.
*/

class Transform;
class Rigidbody : public Component
{
private:
    Transform* transform = nullptr;

private:
    // ground gravity controll
    bool isGrounded;
    int groundContactCount = 0;

    // collision block controll
    bool isBlockedLeft = false;
    bool isBlockedRight = false;
    bool isBlockedUp = false;
    bool isBlockedDown = false;

public:
    Vector2 velocity = Vector2::zero;           // �ӵ�
    Vector2 acceleration = Vector2::zero;       // ���ӵ�
    float mass = 1.0f;                          // ����
    bool useGravity = true;                     // �߷� ��� ����
    float gravityScale = 1.0f;                  // �߷� ������
    float drag = 0.0f;                          // ���� ����, ������ (Ŭ���� �ӵ� ����)
    bool isKinematic = false;                   // kinematic ������ ���� ����� �浹 ���� x

private:
    Vector2 impulse = Vector2::zero;            // ������ (�������� ��)

public:
    // component cycle
    Rigidbody() = default;
    ~Rigidbody() override = default;

    void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
    void FixedUpdate();
    void OnDestroy_Inner() override final;

private:
    void CorrectPosition(const ContactInfo& contact);

public:
    // func
    void AddForce(const Vector2& force);
    void AddImpulse(const Vector2& impulse);

    // friend
    friend class BoxCollider;
    friend class CircleCollider;
};


