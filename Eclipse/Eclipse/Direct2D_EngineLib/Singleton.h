#pragma once
#include <cassert>

/* [Singleton 클래스]
* 싱글톤 코드를 재활용하여 상속만으로 각 클래스를 싱글톤으로 만들 수 있다.
* - Class RenderManager : public Singleton<RenderManager> {};
* - RenderManager를 만드는 순간 Singleton생성자에 의해 static RenderManager Instance가 생기므로 정적으로 활용할 수 있다.
* - RenderManager::Get().DrawImage(bitmap, posX, posY);
*/

template <typename T>
class Singleton
{
private:
	static T* Instance;

public:
	// 생성자
	Singleton()
	{
		assert(Instance == nullptr && "Singleton instance already created!");
		Instance = static_cast<T*>(this);
	}

	// 소멸자
	virtual ~Singleton() = default;

	// static 객체 반환
	static T& Get()
	{
		assert(Instance != nullptr && "Singleton instance not created!");
		return *Instance;
	}

	// 복사 및 이동 금지
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};

template <typename T>
T* Singleton<T>::Instance = nullptr;