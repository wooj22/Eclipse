#include "RectTransform.h"

// static mamber init
D2D1::Matrix3x2F RectTransform::unityMatrix = D2D1::Matrix3x2F::Identity();
D2D1::Matrix3x2F RectTransform::renderMatrix = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);

// component cycle
void RectTransform::OnEnable_Inner()
{
    TransformSystem::Get().Regist(this);
}

void RectTransform::OnDisable_Inner()
{
	TransformSystem::Get().Unregist(this);
}

void RectTransform::OnDestroy_Inner()
{
    TransformSystem::Get().Unregist(this);

    // parent clear
    if (parent != nullptr) {
        parent->RemoveChild(this);
        parent = nullptr;
    }

    // child clear
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->parent = nullptr;
    }
    children.clear();
}

void RectTransform::Update()
{
    MakeScreenLocalMatrix();
    MakeScreenWorldMatrix();
}

// parent, children
void RectTransform::SetParent(RectTransform* newParent)
{
	// 부모가 같을 경우
	if (newParent == parent) return;

	// 부모 set
	if (parent != nullptr) {
		parent->RemoveChild(this);
	}

	parent = newParent;
	if (parent)
		parent->children.push_back(this);

	MarkWorldDirty();
}

void RectTransform::RemoveChild(RectTransform* child)
{
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (*it != child)
            continue;

        it = children.erase(it);
        break;
    }
}

void RectTransform::MarkWorldDirty()
{
    isWorldDirty = true;
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        (*it)->MarkWorldDirty();
    }
}

// set
void RectTransform::SetPosition(const Vector2& position)
{
    this->position = position;
    isLocalDirty = true;
    MarkWorldDirty();
}

void RectTransform::SetPosition(const float& x, const float& y)
{
    position = { x, y };
    isLocalDirty = true;
    MarkWorldDirty();
}

void RectTransform::SetSize(const float& w, const float& h)
{
    size = { w, h };
    isLocalDirty = true;
    MarkWorldDirty();
}

void RectTransform::SetPivot(const float& x, const float& y)
{
    pivot = { x, y };
    isLocalDirty = true;
    MarkWorldDirty();
}

// Screen Local Matrix
inline void RectTransform::MakeScreenLocalMatrix()
{
    if (isLocalDirty)
    {
        // unity
        float offsetX = -size.width * pivot.x;
        float offsetY = size.height * (1.0f - pivot.y);           // Y 축 반전 고려

        screenLocalMatrix =
            D2D1::Matrix3x2F::Translation(offsetX, offsetY) *     // pivot 보정
            D2D1::Matrix3x2F::Translation(position.x, position.y);

        isLocalDirty = false;
    }
}

// Screen World, Screen Matrix - 카메라 역행렬 적용이 필요 없으므로 같이 계산
inline void RectTransform::MakeScreenWorldMatrix()
{
	if (isWorldDirty) {
        screenWolrdMatrix = parent ? screenLocalMatrix * parent->GetScreenWorldMatrix() : screenLocalMatrix;
		screenMatrix = renderMatrix * screenWolrdMatrix * unityMatrix;
        isWorldDirty = false;
	}
}