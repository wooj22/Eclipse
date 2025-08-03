#pragma once
#include <vector>
#include <typeinfo>
#include <iostream>
#include <windows.h> 
#include "Component.h"
#include "Object.h"
#include "Transform.h";
#include "RectTransform.h"

using namespace std;

/* [GameObject]
* Component를 등록시킬 수 있는 오브젝트로,
* 게임 콘텐츠에 활용할 오브젝트는 이 클래스를 상속받아 컴포넌트를 조합하여 구현한다.
* 즉 GameObject는 Component들의 컨테이너 역할을 한다.
* Tranform은 GameObject의 기본 컴포넌트로, AddComponenet시 public으로 접근할 수 있는 멤버로 연결한다.
* 
* < GameObject Cycle >
*  : GameObject의 사이클은 기본적으로 사용하지 않아도 된다. (awake부터~)
*    보통 Script 컴포넌트를 활용하여 로직을 작성하며, 
     간단한 오브젝트 같은 경우에 Script 컴포넌트를 만들지 않고 빨리 작성할 수 있도록 냅둔 것이다.
*  1．　게임오브젝트 생성자() : 컴포넌트 생성. 초기화 로직
*  2．　게임오브젝트 Awake()           // 해당 게임오브젝트의 모든 컴포넌트가 생성되고 난 뒤
*  3．　게임오브젝트 SceneStart()　    // Scene의 Start시점 (update중 생성된 오브젝트라면 호출되지 않음)
*  4．　게임오브젝트 Update()
*  5．　게임오브젝트 Destroyed()
* 
* < 콘텐츠 제작시 유의사항 >
* 1) '생성자()'에서 컴포넌트 생성하고 리소스 로드등 초기화를 한다.
* 2) 생성자에서 컴포넌트를 생성할 때 각 컴포넌트의 OnEnable()이 호출된다.
* 3) 만약 Sciprt 컴포넌트가 있다면 그냥 유니티랑 똑같이 생각하고 사용하면 된다. (Awake, Start 보장)
* 4) 오브젝트가 생성될 때 ComponentInit()에 이어 'Awake()'가 호출된다. 즉, this오브젝트의 컴포넌트 등록이 모두 마친 뒤 호출 되는것
* 5) Scene이 Start()될 때 'SceneStart()'가 호출된다. 
     이때는 씬의 모든 게임오브젝트들이 생성되어있는 시점이므로 Find, 부모관계 지정 등의 작업이 가능하다
     ⭐ 만약 Scene의 Update중간에 생성되는 게임오브젝트라면 SceneStart()가 호출되지 않으므로
        SceneStart()안에 코드를 작성하지 않아야 한다.
        중간에 생성되는 오브젝트는 다른 게임오브젝트들이 이미 생성된 시점이기 때문에
        생성자나 Awake()에 초기화 코드를 모두 작성하면 된다.
        그러니까 웬만큼 간단한 오브젝트가 아닌 이상 따로 Script 컴포넌트를 사용하라.
  6) Scene이 Update될 때 'Update()'가 매 프레임 호출된다.
  7) 게임오브젝트가 파괴되거나 씬이 종료될 때 Destroyed()가 호출된다.
*/

class Component;
class GameObject : public Object
{
    /* GameObject Data */
private: 
    vector<Component*> components; 
	bool isActive = true;

public: 
    Transform* transform = nullptr;
	RectTransform* rectTransform = nullptr;
    string name = "GameObject";
    string tag = "Untagged";

    /* GameObject Cycle */
public:
    GameObject(const string& objName = "GameObject", const string& objTag = "Untagged")
        : name(objName), tag(objTag)
    {
        allGameObjects.push_back(this);
    }

    virtual ~GameObject()
    {
        // component delete
        for (Component* comp : components) {
            comp->OnDestroy_Inner();
            delete comp;
        }
        components.clear();

		// gameobject delete
        auto it = std::find(allGameObjects.begin(), allGameObjects.end(), this);
        if (it != allGameObjects.end()) allGameObjects.erase(it);  
    }
 
    virtual void Awake() {}            // 오브젝트가 생성될 때, 생성자 이후
    virtual void SceneStart() {}       // Scene의 Start -> Update중 SceneStart() 호출 보장 x
    virtual void Update() {}           // Scene의 Update
    virtual void Destroyed() {}        // Scene의 Exit, GameObject Delete



    /*  GameObject Active  */
    void SetActive(bool active)
    {
        if (isActive == active) return;
        isActive = active;

        // component enabled
        for (Component* comp : components) 
        {
			comp->SetEnabled(active);
        }

		// children active
        if (transform) 
        {
			vector<Transform*> childrens = transform->GetChildrens();
            for (Transform* childTransform : childrens)
            {
                if (childTransform && childTransform->gameObject)
                {
                    childTransform->gameObject->SetActive(active);
                }
            }
            return;
        }

		if (rectTransform)
		{
            vector<RectTransform*> childrens = rectTransform->GetChildrens();
			for (RectTransform* childRectTransform : childrens)
			{
				if (childRectTransform && childRectTransform->gameObject)
				{
					childRectTransform->gameObject->SetActive(active);
				}
			}
		}
	}

    bool IsActive() const { return isActive; }



    /*  GameObject Destroy  */
    // isDestroyed true가 되면 Scene쪽에서 delete
private:
    bool isDestroyed = false;           
public:
    void Destroy() { isDestroyed = true; }
    bool IsDestroyed() const { return isDestroyed; }
    static void Destroy(GameObject* obj) 
    {
        if(obj) obj->Destroy();
    }




    /*  GameObject Find  */
    // allGameObjects에는 현재 씬의 살아있는 오브젝트만 포함된다.
private:
    static vector<GameObject*> allGameObjects;
public:
    // game object find - name
    // targetName과 같은 게임오브젝트 하나를 반환한다.
    static GameObject* Find(const string& targetName)
    {
        for (GameObject* obj : allGameObjects)
        {
            if (obj && !obj->IsDestroyed() && obj->name == targetName)
                return obj;
        }
        return nullptr;
    }

    // game object all find - name
    // targetName과 같은 게임오브젝트 전체를 반환한다.
    static vector<GameObject*> FindAll(const string& targetName)
    {
        vector<GameObject*> result;

        for (GameObject* obj : allGameObjects)
        {
            if (obj && !obj->IsDestroyed() && obj->name == targetName)
            {
                result.push_back(obj);
            }
        }

        return result;
    }

    // game object find - tag
    // targetTag과 같은 태그의 게임오브젝트 전체를 반환한다.
    static GameObject* FindWithTag(const string& targetTag)
    {
        for (GameObject* obj : allGameObjects)
        {
            if (obj && !obj->IsDestroyed() && obj->tag == targetTag)
                return obj;
        }
        return nullptr;
    }

    // game object all find - tag
    // targetTag과 같은 태그의 게임오브젝트 전체를 반환한다.
    static vector<GameObject*> FindAllWithTag(const string& targetTag)
    {
        vector<GameObject*> result;

        for (GameObject* obj : allGameObjects)
        {
            if (obj && !obj->IsDestroyed() && obj->tag == targetTag)
                result.push_back(obj);
        }
        return result;
    }




    /*  Component  */
public:
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(is_base_of<Component, T>::value, "T must derive from Component");

        // Transform 중복 검사
        if (std::is_same<T, Transform>::value && transform != nullptr)
        {
            throw std::runtime_error("Transform을 두개 생성할 수 없습니다.");
        }

        // RectTransform 중복 검사
        if (std::is_same<T, RectTransform>::value && rectTransform != nullptr)
        {
            throw std::runtime_error("RectTransform을 두개 생성할 수 없습니다.");
        }

        // Transform과 RectTransform은 서로 동시에 존재할 수 없음
        if ((std::is_same<T, Transform>::value && rectTransform != nullptr) ||
            (std::is_same<T, RectTransform>::value && transform != nullptr))
        {
            throw std::runtime_error("Tranform과 RectTransform중 하나만 소유할 수 있습니다.");
        }

		// add component
        T* comp = new T(forward<Args>(args)...);
        comp->gameObject = this;
        components.push_back(comp);
        comp->OnEnable_Inner();

		// gameobject transform public member
        if (auto trans = dynamic_cast<Transform*>(comp)) {
            transform = trans;
        }

		// gameobject rectTransform public member
		if (auto rectTrans = dynamic_cast<RectTransform*>(comp)) {
			rectTransform = rectTrans;
		}

        return comp;
    }

    template<typename T>
    vector<T*> GetComponents() {
        vector<T*> result;
        for (Component* comp : components) {
            if (auto casted = dynamic_cast<T*>(comp))
                result.push_back(casted);
        }
        return result;
    }

    template<typename T>
    T* GetComponent() {
         for (Component* comp : components) {
            if (typeid(*comp) == typeid(T))
                return static_cast<T*>(comp);
        }
        return nullptr;
    }

    template<typename T>
    bool RemoveComponent(T* target) {

        for (auto it = components.begin(); it != components.end(); ++it) {
            if (*it == target)
            {
                (*it)->OnDestroy_Inner();
                delete* it;
                components.erase(it);
                return true;
            }
        }
        return false;
    }
};