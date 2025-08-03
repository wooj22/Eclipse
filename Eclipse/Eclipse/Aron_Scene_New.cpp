// 새로운 안전한 혼문 관리 시스템 구현

// 스마트 포인터로 안전한 혼문 생성
int Aron_Scene::CreateHonmun(HonmunType type, float x, float y)
{
    std::lock_guard<std::mutex> lock(honmunMutex);
    
    auto honmun = std::shared_ptr<Honmun>(CreateObject<Honmun>());
    if (!honmun) {
        OutputDebugStringA("ERROR: Failed to create Honmun object\n");
        return -1;
    }
    
    int id = nextHonmunId++;
    
    // 안전한 설정
    honmun->SetHonmunType(type);
    honmun->SetPosition(x, y);
    honmun->name = "Honmun_" + std::to_string(id);
    
    // 물리 설정
    auto* rb = honmun->GetComponent<Rigidbody>();
    if (rb) {
        rb->isKinematic = false;
        rb->useGravity = false;
    }
    
    auto* collider = honmun->GetComponent<CircleCollider>();
    if (collider) {
        collider->isTrigger = true;
    }
    
    honmunRegistry[id] = honmun;
    honmunManager.currentCount = static_cast<int>(honmunRegistry.size());
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Created Honmun ID %d at (%.1f, %.1f)\n", id, x, y);
    OutputDebugStringA(debugMsg);
    
    return id;
}

// 안전한 혼문 제거
bool Aron_Scene::RemoveHonmun(int id)
{
    std::lock_guard<std::mutex> lock(honmunMutex);
    
    auto it = honmunRegistry.find(id);
    if (it == honmunRegistry.end()) {
        return false;
    }
    
    auto honmun = it->second;
    if (honmun && honmun->IsActive()) {
        honmun->SetActive(false);
    }
    
    honmunRegistry.erase(it);
    honmunManager.currentCount = static_cast<int>(honmunRegistry.size());
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Removed Honmun ID %d\n", id);
    OutputDebugStringA(debugMsg);
    
    return true;
}

// 안전한 혼문 접근
std::shared_ptr<Honmun> Aron_Scene::GetHonmun(int id) const
{
    std::lock_guard<std::mutex> lock(honmunMutex);
    
    auto it = honmunRegistry.find(id);
    if (it != honmunRegistry.end() && it->second && it->second->IsActive()) {
        return it->second;
    }
    return nullptr;
}

// 개선된 혼문 이동 처리
void Aron_Scene::HandleHonmunMovement()
{
    float moveSpeed = 3.0f;
    
    // 안전한 혼문 선택
    if (Input::GetKeyDown('Z')) {
        std::lock_guard<std::mutex> lock(honmunMutex);
        if (!honmunRegistry.empty()) {
            auto it = honmunRegistry.begin();
            std::advance(it, rand() % honmunRegistry.size());
            selectedHonmunId = it->first;
            
            char debugMsg[100];
            sprintf_s(debugMsg, "Selected Honmun ID: %d\n", selectedHonmunId);
            OutputDebugStringA(debugMsg);
        }
    }
    
    // 선택된 혼문 이동
    auto selectedHonmun = GetHonmun(selectedHonmunId);
    if (selectedHonmun) {
        auto* transform = selectedHonmun->GetComponent<Transform>();
        if (transform) {
            Vector2 currentPos = transform->GetPosition();
            if (Input::GetKey(VK_LEFT)) transform->SetPosition(currentPos.x - moveSpeed, currentPos.y);
            if (Input::GetKey(VK_RIGHT)) transform->SetPosition(currentPos.x + moveSpeed, currentPos.y);
            if (Input::GetKey(VK_UP)) transform->SetPosition(currentPos.x, currentPos.y + moveSpeed);
            if (Input::GetKey(VK_DOWN)) transform->SetPosition(currentPos.x, currentPos.y - moveSpeed);
        }
    }
}

// 개선된 웨이브 스폰 시스템
void Aron_Scene::SpawnHonmun()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);
    
    float spawnX = xDis(gen);
    
    int id = CreateHonmun(GetRandomHonmunTypeWave1(), spawnX, waveData.spawnY);
    if (id != -1) {
        waveData.spawnedHonmunIds.push_back(id);
        
        char debugMsg[100];
        sprintf_s(debugMsg, "Wave spawned Honmun ID %d at (%.2f, %.2f)\n", 
            id, spawnX, waveData.spawnY);
        OutputDebugStringA(debugMsg);
    }
}

// 개선된 경계 체크
bool Aron_Scene::IsOutOfBounds(std::shared_ptr<Honmun> honmun)
{
    if (!honmun) return true;
    
    auto* transform = honmun->GetComponent<Transform>();
    if (!transform) return true;
    
    Vector2 pos = transform->GetPosition();
    float boundaryX = 5000.0f;
    float boundaryY = 5000.0f;
    
    return (abs(pos.x) > boundaryX || abs(pos.y) > boundaryY);
}

// 개선된 정리 시스템
void Aron_Scene::CheckAndRemoveOutOfBounds()
{
    std::lock_guard<std::mutex> lock(honmunMutex);
    
    std::vector<int> toRemove;
    
    for (const auto& pair : honmunRegistry) {
        if (IsOutOfBounds(pair.second)) {
            toRemove.push_back(pair.first);
        }
    }
    
    for (int id : toRemove) {
        honmunRegistry.erase(id);
    }
    
    if (!toRemove.empty()) {
        honmunManager.currentCount = static_cast<int>(honmunRegistry.size());
        char debugMsg[100];
        sprintf_s(debugMsg, "Removed %zu out-of-bounds Honmuns\n", toRemove.size());
        OutputDebugStringA(debugMsg);
    }
}