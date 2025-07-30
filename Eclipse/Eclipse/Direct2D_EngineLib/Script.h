#pragma once
#include "Component.h"
#include "ScriptSystem.h"
#include "GameObject.h"
#include "ICollider.h"
#include "SceneManager.h"
#include "Scene.h"

/* [Script Conponent]
* GameObject의 여러 Component들을 controll하는 script 컴포넌트이다.
* 게임 콘텐츠에서는 이 class를 상속받은 script를 작성하여
* 기능별로 스크립트를 분리하고, 재활용 할 수 있다. ex) Movement.cs, Inventory.cs
* -> 유니티의 모노비헤이어라고 생각하면 된다.
*/

class ICollider;
class Script : public Component
{
public:
    bool started = false;

    /* script component cycle */
    Script() = default;
	~Script() override = default;

    virtual void OnEnable() {}         // 컴포넌트 활성화시
    virtual void OnDisable() {}        // 컴포넌트 비활성화시
    virtual void Awake() {}            // Update()전에 1회 호출
    virtual void Start() {}            // Awake() 이후 Updtae() 직전 시점 1회 호출
    virtual void Update() {}           // 프레임 단위 반복 호출
    virtual void FixedUpdate() {}      // 물리 업데이트 0.02f 보장 반복 호출
    virtual void OnDestroy() {}        // 컴포넌트 or 오브젝트 소멸 시점
	

    /* activate inner */
    void OnEnable_Inner() override final
    {
        ScriptSystem::Get().Regist(this);
        OnEnable();
    }

    void OnDisable_Inner() override final
    {
        ScriptSystem::Get().Unregist(this);
        OnDisable();
    }

    void OnDestroy_Inner() override final
    {
        ScriptSystem::Get().Unregist(this);
        OnDestroy();
    }


    /* Collition Event */
    // trigger
    virtual void OnTriggerEnter(ICollider* other) {}
    virtual void OnTriggerStay(ICollider* other) {}
    virtual void OnTriggerExit(ICollider* other) {}

    // collision
    virtual void OnCollisionEnter(ICollider* other, const ContactInfo& contact) {}
    virtual void OnCollisionStay(ICollider* other, const ContactInfo& contact) {}
    virtual void OnCollisionExit(ICollider* other, const ContactInfo& contact) {}

    /* Instantiate */
	template<typename T, typename... Args>
	T* Instantiate(Vector2 position = { 0,0 }, GameObject* parent = nullptr, Args&&... args)
	{
		return SceneManager::Get().GetCurrentScene()->CreateObject<T>(position, parent, std::forward<Args>(args)...);
	}
};



