#include "BossController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"

void BossController::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	rb = gameObject->GetComponent<Rigidbody>();
	collider = gameObject->GetComponent<CircleCollider>();
}

void BossController::Start()
{

}

void BossController::Update()
{
	
}

void BossController::FixedUpdate()
{
	
}

void BossController::OnDestroy()
{

}

/*-------------  trigger event  -------------*/ 
void BossController::OnTriggerEnter(ICollider* other)
{

}

void BossController::OnTriggerStay(ICollider* other)
{

}

void BossController::OnTriggerExit(ICollider* other)
{

}