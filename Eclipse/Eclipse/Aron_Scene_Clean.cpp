// 중복 코드 제거 및 설정 시스템 개선

// 설정 관리 시스템
class AronSceneConfig {
public:
    // 하드코딩된 값들을 상수로 정리
    static constexpr float SPAWN_Y = 1200.0f;
    static constexpr float GROUND_Y = -350.0f;
    static constexpr float FLOOR2_Y = -180.0f;
    static constexpr float MAP_BOUNDARY_X = 5000.0f;
    static constexpr float MAP_BOUNDARY_Y = 5000.0f;
    static constexpr float HONMUN_MOVE_SPEED = 3.0f;
    static constexpr float CAMERA_MOVE_SPEED = 300.0f;
    static constexpr int DEFAULT_TARGET_COUNT = 20;
    static constexpr float RESPAWN_INTERVAL = 0.5f;
    
    // 웨이브 설정
    struct WaveConfig {
        int totalSpawn;
        float spawnInterval;
        std::vector<HonmunType> allowedTypes;
    };
    
    static const std::unordered_map<int, WaveConfig> WAVE_CONFIGS;
};

// 웨이브 설정 초기화
const std::unordered_map<int, AronSceneConfig::WaveConfig> AronSceneConfig::WAVE_CONFIGS = {
    {1, {20, 1.0f, {HonmunType::A, HonmunType::B}}},
    {2, {25, 0.8f, {HonmunType::A, HonmunType::B, HonmunType::C}}},
    {3, {30, 0.6f, {HonmunType::A, HonmunType::B, HonmunType::C, HonmunType::D}}}
};

// 기존의 중복된 웨이브 시작 함수들 통합
/*
void StartWave1() { ... 100줄의 중복 코드 ... }
void StartWave2() { ... 100줄의 중복 코드 ... }
void StartWave3() { ... 100줄의 중복 코드 ... }
*/

// 통합된 웨이브 시작 시스템
void Aron_Scene::StartWave(int waveNumber)
{
    auto configIt = AronSceneConfig::WAVE_CONFIGS.find(waveNumber);
    if (configIt == AronSceneConfig::WAVE_CONFIGS.end()) {
        char debugMsg[100];
        sprintf_s(debugMsg, "Invalid wave number: %d\n", waveNumber);
        OutputDebugStringA(debugMsg);
        return;
    }
    
    const auto& config = configIt->second;
    
    // 이전 웨이브 안전하게 정리
    SafeCleanupWave();
    
    // GameManager 업데이트
    GameManager::Get().waveCount = waveNumber;
    GameManager::Get().isWave = true;
    
    // 웨이브 데이터 설정
    waveData.waveActive = true;
    waveData.currentWave = waveNumber;
    waveData.currentSpawnIndex = 0;
    waveData.lastSpawnTime = 0.0f;
    waveData.totalSpawnCount = config.totalSpawn;
    waveData.spawnInterval = config.spawnInterval;
    waveData.spawnedHonmunIds.clear();
    
    currentScore = 0;
    UpdateScoreUI();
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Wave %d started: %d enemies, %.1fs interval\n", 
        waveNumber, config.totalSpawn, config.spawnInterval);
    OutputDebugStringA(debugMsg);
}

// 기존의 중복된 스폰 함수들 통합
/*
void SpawnHonmun() { ... 중복 코드 ... }
void SpawnHonmunWave2() { ... 중복 코드 ... }
void SpawnHonmunWave3() { ... 중복 코드 ... }
*/

// 통합된 스폰 시스템
void Aron_Scene::SpawnHonmunForWave()
{
    auto configIt = AronSceneConfig::WAVE_CONFIGS.find(waveData.currentWave);
    if (configIt == AronSceneConfig::WAVE_CONFIGS.end()) return;
    
    const auto& config = configIt->second;
    const auto& allowedTypes = config.allowedTypes;
    
    if (allowedTypes.empty()) return;
    
    // 랜덤 위치 및 타입 선택
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);
    std::uniform_int_distribution<int> typeDis(0, static_cast<int>(allowedTypes.size() - 1));
    
    float spawnX = xDis(gen);
    HonmunType randomType = allowedTypes[typeDis(gen)];
    
    int id = CreateHonmun(randomType, spawnX, AronSceneConfig::SPAWN_Y);
    if (id != -1) {
        waveData.spawnedHonmunIds.push_back(id);
        
        char debugMsg[100];
        sprintf_s(debugMsg, "Wave %d spawned Honmun ID %d (Type %d) at (%.1f, %.1f)\n", 
            waveData.currentWave, id, static_cast<int>(randomType), spawnX, AronSceneConfig::SPAWN_Y);
        OutputDebugStringA(debugMsg);
    }
}

// 기존의 하드코딩된 입력 처리 개선
/*
if (Input::GetKeyDown('1')) { SceneManager::Get().ChangeScene(EclipseApp::MOON); }
if (Input::GetKeyDown('2')) { SceneManager::Get().ChangeScene(EclipseApp::YUNMO); }
... 반복적인 하드코딩 ...
*/

// 설정 기반 입력 처리 시스템
class InputHandler {
private:
    std::unordered_map<char, std::function<void()>> keyBindings;
    
public:
    InputHandler(Aron_Scene* scene) {
        // 씬 전환 키 바인딩
        keyBindings['1'] = []() { SceneManager::Get().ChangeScene(EclipseApp::MOON); };
        keyBindings['2'] = []() { SceneManager::Get().ChangeScene(EclipseApp::YUNMO); };
        keyBindings['3'] = []() { SceneManager::Get().ChangeScene(EclipseApp::ARON); };
        keyBindings['4'] = []() { SceneManager::Get().ChangeScene(EclipseApp::WOO); };
        keyBindings['5'] = []() { SceneManager::Get().ChangeScene(EclipseApp::MENU); };
        keyBindings['6'] = []() { SceneManager::Get().ChangeScene(EclipseApp::PLAY); };
        keyBindings['7'] = []() { SceneManager::Get().ChangeScene(EclipseApp::END); };
        
        // 웨이브 키 바인딩
        keyBindings['Q'] = [scene]() { 
            if (!scene->waveData.waveActive) scene->StartWave(1); 
        };
        keyBindings['E'] = [scene]() { 
            if (!scene->waveData.waveActive) scene->StartWave(2); 
        };
        keyBindings['T'] = [scene]() { 
            if (!scene->waveData.waveActive) scene->StartWave(3); 
        };
        
        // 기능 키 바인딩
        keyBindings['R'] = [scene]() { scene->ResetScene(); };
        keyBindings['C'] = [scene]() { scene->ResetCamera(); };
        keyBindings['Z'] = [scene]() { scene->SelectPreviousHonmun(); };
        keyBindings['X'] = [scene]() { scene->SelectNextHonmun(); };
    }
    
    void ProcessInput() {
        for (const auto& binding : keyBindings) {
            if (Input::GetKeyDown(binding.first)) {
                binding.second();
            }
        }
    }
};

// 통합된 업데이트 시스템
void Aron_Scene::Update()
{
    __super::Update();
    
    // 모듈화된 업데이트 함수들
    ProcessInput();
    UpdateWaveSystem();
    UpdateHonmunManager();
    UpdateUI();
    DrawDebugInfo();
}

// UI 업데이트 통합
void Aron_Scene::UpdateUI()
{
    UpdateScoreUI();
    
    if (debug_text && debug_text->screenTextRenderer) {
        wchar_t debugInfo[300];
        swprintf_s(debugInfo, L"Honmuns: %d/%d | Wave: %d | Score: %d | Controls: QETZ/Arrow/UHJK/C", 
            honmunManager.currentCount, honmunManager.targetCount, 
            waveData.currentWave, currentScore);
        debug_text->screenTextRenderer->SetText(debugInfo);
    }
}

// 중복된 카메라 이동 코드 제거
void Aron_Scene::ResetCamera()
{
    if (cam) {
        auto* cameraTransform = cam->GetComponent<Transform>();
        if (cameraTransform) {
            cameraTransform->SetPosition(0.0f, 0.0f);
            OutputDebugStringA("Camera reset to origin\n");
        }
    }
}

// 혼문 선택 로직 간소화
void Aron_Scene::SelectPreviousHonmun()
{
    std::lock_guard<std::mutex> lock(honmunMutex);
    if (honmunRegistry.empty()) return;
    
    auto it = honmunRegistry.find(selectedHonmunId);
    if (it != honmunRegistry.begin()) {
        --it;
    } else {
        it = std::prev(honmunRegistry.end());
    }
    
    selectedHonmunId = it->first;
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Selected previous Honmun ID: %d\n", selectedHonmunId);
    OutputDebugStringA(debugMsg);
}

void Aron_Scene::SelectNextHonmun()
{
    std::lock_guard<std::mutex> lock(honmunMutex);
    if (honmunRegistry.empty()) return;
    
    auto it = honmunRegistry.find(selectedHonmunId);
    ++it;
    if (it == honmunRegistry.end()) {
        it = honmunRegistry.begin();
    }
    
    selectedHonmunId = it->first;
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Selected next Honmun ID: %d\n", selectedHonmunId);
    OutputDebugStringA(debugMsg);
}