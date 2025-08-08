#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "Time.h"

void Rigidbody::OnEnable_Inner()
{
    PhysicsSystem::Get().Regist(this);
	transform = this->gameObject->transform;
}

void Rigidbody::OnDisable_Inner()
{
	PhysicsSystem::Get().Unregist(this);
	transform = nullptr;
}

void Rigidbody::OnDestroy_Inner()
{
    PhysicsSystem::Get().Unregist(this);
}

void Rigidbody::FixedUpdate()
{
    if (!transform) return;

    if (!isKinematic)
    {
        // impulse update
        velocity += impulse / mass;

        // gravity update
        if (useGravity && !isGrounded) acceleration += Vector2(0, -9.8f) * gravityScale;

        // acceleration update
        velocity += acceleration * Time::GetFixedDeltaTime();
        velocity *= (1.0f - drag);

        // block update
        if (isBlockedLeft && velocity.x < 0) velocity.x = 0;
        if (isBlockedRight && velocity.x > 0) velocity.x = 0;
        if (isBlockedDown && velocity.y < 0) velocity.y = 0;
        if (isBlockedUp && velocity.y > 0) velocity.y = 0;

        // grounded -> gravity reset
        //if (isGrounded && useGravity && velocity.y < 0) velocity.y = 0;

        // position update
        transform->SetPosition(transform->GetPosition() + velocity * Time::GetFixedDeltaTime());

        // reset
        impulse = Vector2::zero;
        acceleration = Vector2::zero;
    }
}

// colliison position correct
void Rigidbody::CorrectPosition(const ContactInfo& contact)
{
    if (!isKinematic)
    {
        // position
        transform->Translate(contact.normal * contact.depth);

        // block
        if (contact.normal.x > 0)      isBlockedLeft = true;
        else if (contact.normal.x < 0) isBlockedRight = true;
        if (contact.normal.y > 0)      isBlockedDown = true;
        else if (contact.normal.y < 0) isBlockedUp = true;
    }
}

// turnneling collision continuous detection
void Rigidbody::CollisionContinuousDetection()
{

}

void Rigidbody::AddForce(const Vector2& force)
{
    acceleration += force / mass;
}

void Rigidbody::AddImpulse(const Vector2& impulse) 
{
    this->impulse = impulse / mass;
}
