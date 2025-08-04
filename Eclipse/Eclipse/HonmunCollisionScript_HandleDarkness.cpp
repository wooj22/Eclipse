void HonmunCollisionScript::HandleDarknessReaction(HonmunCollisionScript* otherScript)
{
	// C + C = 체력 감소 시스템 (3 → 2 → 1 → 0 완전 파괴) + 점수 +3점
	
	// 디버그 출력
	OutputDebugStringA("C + C collision detected!\n");
	
	// 점수 시스템을 위한 현재 씬 가져오기
	auto* currentScene = SceneManager::Get().GetCurrentScene();
	auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
	
	// C + C 충돌 시 항상 +3점 부여
	if (aronScene)
	{
		aronScene->AddScore(3);
		OutputDebugStringA("C + C collision: +3 points!\n");
	}
	
	// 체력 감소
	health--;
	otherScript->health--;
	
	// 체력이 0이 되면 완전 파괴
	if (health <= 0 || otherScript->health <= 0)
	{
		OutputDebugStringA("C type health reached 0, destroying objects!\n");
		if (health <= 0) DestroyThis();
		if (otherScript->health <= 0) otherScript->DestroyThis();
	}
	else
	{
		// 체력이 남아있으면 강한 밀려남 효과 (알까기 컴셉)
		OutputDebugStringA("Both C objects still have health, strong bounce!\n");
		
		// 충돌 지점 계산 (두 C 혼문의 중간 지점)
		Vector2 collisionPoint = (transform->GetPosition() + otherScript->transform->GetPosition()) * 0.5f;
		
		// 주변 혼문들을 끌어당기는 효과 추가
		AbsorbNearbyEnemies(collisionPoint, otherScript);
		
		// 충돌 후 강한 밀려남 효과
		BounceAwayKinematic(otherScript);
	}
}