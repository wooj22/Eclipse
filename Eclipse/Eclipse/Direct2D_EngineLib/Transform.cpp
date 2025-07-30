#include "Transform.h"
#include "Camera.h"

// static mamber init
D2D1::Matrix3x2F Transform::unityMatrix = D2D1::Matrix3x2F::Identity();
D2D1::Matrix3x2F Transform::renderMatrix = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);

// component cycle
void Transform::OnEnable_Inner()
{
    TransformSystem::Get().Regist(this);
}

void Transform::OnDisable_Inner()
{
	TransformSystem::Get().Unregist(this);
}

void Transform::OnDestroy_Inner()
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

void Transform::Update()
{
    // previous position
    prePosition = position;

    // transform update
    MakeLocalMatrix();
    MakeWorldMatrix();
    MakeScreenMatrix();
}

// Set Parent
void Transform::SetParent(Transform* newParent)
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

// Remove Child
void Transform::RemoveChild(Transform* removeChild) 
{
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (*it != removeChild)
            continue;

        it = children.erase(it);
        break;
    }
}

// Mark World Dirty
void Transform::MarkWorldDirty() 
{
    isWorldDirty = true;
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->MarkWorldDirty();
    }
}

// set
void Transform::SetPosition(const Vector2& position)
{
    isLocalDirty = true;
    this->position = position;
    MarkWorldDirty();
}

void Transform::SetPosition(const float& x, const float& y)
{ 
    isLocalDirty = true;
    position = { x, y }; 
    MarkWorldDirty();
}

void Transform::SetRotation(const float& angle)
{ 
    isLocalDirty = true; 
    rotation = angle; 
    MarkWorldDirty();
}

void Transform::SetScale(const Vector2& scale)
{
    isLocalDirty = true;
    this->scale = scale;
    MarkWorldDirty();
}

void Transform::SetScale(const float& scaleX, const float& scaleY)
{
    isLocalDirty = true; 
    scale = { scaleX, scaleY }; 
    MarkWorldDirty();
}

void Transform::ResetTransform() 
{ 
    position = { 0.0f, 0.0f };
    rotation = { 0.0f }; scale = { 1.0f, 1.0f }; 
    isLocalDirty = true; 
    MarkWorldDirty();
}

// translate
void Transform::Translate(const Vector2& translation)
{
    isLocalDirty = true;
    position += translation;
    MarkWorldDirty();
}

void Transform::Translate(const float& x, const float& y)
{
    isLocalDirty = true;
    position.x += x;
    position.y += y;
    MarkWorldDirty();
}

// Rotate
void Transform::Rotate(const float& angle)
{
    isLocalDirty = true;
    this->rotation += angle;
    MarkWorldDirty();
}

// Scaleing
void Transform::Scaleing(const Vector2& scale)
{
    isLocalDirty = true;
    this->scale += scale;
    MarkWorldDirty();
}

void Transform::Scaleing(const float& scaleX, const float& scaleY)
{
    isLocalDirty = true;
    this->scale.x += scaleX;
    this->scale.y += scaleY;
    MarkWorldDirty();
}

// Local Matrix
void Transform::MakeLocalMatrix() 
{
    if (isLocalDirty) {
        localMatrix = D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
            D2D1::Matrix3x2F::Rotation(rotation) *
            D2D1::Matrix3x2F::Translation(position.x, position.y);

        isLocalDirty = false;
    }
}

// World Matrix
void Transform::MakeWorldMatrix() {
    if (isWorldDirty) {
        auto worldMatrix = GetLocalMatrix();
        this->worldMatrix = parent ? worldMatrix * parent->GetWorldMatrix() : worldMatrix;
        isWorldDirty = false;
    }
}

// Screen Matrix
void Transform::MakeScreenMatrix() {
    // d2d 
    //screenMatrix = GetWorldMatrix() * Camera::GetMainInverseMatrix();

    // unity
    screenMatrix = renderMatrix * worldMatrix * Camera::GetMainInverseMatrix() * unityMatrix;
}