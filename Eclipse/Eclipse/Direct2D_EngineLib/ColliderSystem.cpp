#include "ColliderSystem.h"
#include <algorithm>
#include "ICollider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

// component 등록
void ColliderSystem::Regist(ICollider* component)
{
    pending_components.push_back(component);
}

// component 등록 해제
void ColliderSystem::Unregist(ICollider* component)
{
    // delete
    for (auto it = components.begin(); it != components.end(); ++it) {
        if (*it == component) {
            components.erase(it);
            return;
        }
    }

    // pending delete
    for (auto it = pending_components.begin(); it != pending_components.end(); ++it) {
        if (*it == component) {
            pending_components.erase(it);
            return;
        }
    }
}

// component system
void ColliderSystem::FixedUpdate()
{
    // pending_components push
    for (ICollider* collider : pending_components)
    {
        components.push_back(collider);
    }
    pending_components.clear();

	// update bounds
    for (auto collider : components)
    {
        collider->currentFrameCollisions.clear();   // curent frame collision data reset
        collider->UpdateBounds();                   // update bounds
    }

    // Sweep and Prune - SweepandPruneOverlapCheck
    // minX sort
    std::sort(components.begin(), components.end(), [](ICollider* a, ICollider* b)
        { return a->minX < b->minX; });

    // collision cheak
    for (size_t i = 0; i < components.size(); ++i)
    {
        ICollider* a = components[i];
        for (size_t j = i + 1; j < components.size(); ++j)
        {
            ICollider* b = components[j];
            if (b->minX > a->maxX)
                break;

            if (a->maxY < b->minY || a->minY > b->maxY)
                continue;

            // collision contact info
            ContactInfo contact;

            if (a->isCollision(b, contact))
            {
                a->currentFrameCollisions[b] = contact;
                // B 쪽 contact는 법선을 반대로 해서 저장
                ContactInfo inverseContact = contact;
                inverseContact.normal = { -contact.normal.x, -contact.normal.y };
                b->currentFrameCollisions[a] = inverseContact;
            }
        }
    }

    // Enter / Stay / Exit
    for (auto collider : components)
    {
        collider->FinalizeCollision();
    }
}

// Raycast 광선과 콜라이더의 충돌 정보 반환
RaycastHit ColliderSystem::Raycast(const Ray& ray, float maxDistance)
{
    RaycastHit closestHit;
    closestHit.collider = nullptr;
    closestHit.distance = maxDistance;

    for (ICollider* col : components)
    {
        // trigger collider는 제외
        if (col->isTrigger) continue;

        RaycastHit hitInfo;
        if (col->Raycast(ray, maxDistance, hitInfo))
        {
            // 충돌 콜라이더중 가장 가까운 것으로 저장
            if (hitInfo.distance < closestHit.distance)
            {
                closestHit = hitInfo;
            }
        }
    }

    // 충돌한 콜라이더 값을 경우 closesHit.collider == nullptr;
    return closestHit;
}