#include "PlayerController_Woo.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "../Direct2D_EngineLib/Rigidbody.h"


void PlayerController_Woo::OnEnable()
{

}

void PlayerController_Woo::OnDisable()
{

}

void PlayerController_Woo::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	rb = gameObject->GetComponent<Rigidbody>();
}

void PlayerController_Woo::Start()
{

}

void PlayerController_Woo::Update()
{
	InputCheak();
}

void PlayerController_Woo::FixedUpdate()
{
	Movement();
}

void PlayerController_Woo::OnDestroy()
{

}

void PlayerController_Woo::OnTriggerEnter(ICollider* other)
{

}

void PlayerController_Woo::OnTriggerStay(ICollider* other)
{

}

void PlayerController_Woo::OnTriggerExit(ICollider* other)
{

}

void PlayerController_Woo::OnCollisionEnter(ICollider* other, const ContactInfo& contact)
{

}

void PlayerController_Woo::OnCollisionStay(ICollider* other, const ContactInfo& contact)
{

}

void PlayerController_Woo::OnCollisionExit(ICollider* other, const ContactInfo& contact)
{

}

void PlayerController_Woo::InputCheak()
{
	inputX = Input::GetAxisHorizontal();
	inputY = Input::GetAxisVertical();

	isW = Input::GetKey('W');
	isA = Input::GetKey('A');
	isD = Input::GetKey('D');
}

void PlayerController_Woo::Movement()
{
	rb->velocity.x = inputX * speed;
}

void PlayerController_Woo::Jump()
{
	if (isGround && Input::GetKeyDown(' ')) {
		rb->AddForce(Vector2(0, jumpForce));
		isGround = false;
	}
}