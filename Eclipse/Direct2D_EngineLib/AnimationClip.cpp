#include "AnimationClip.h"

/// LoadSpritesFromJson
/// 넘겨받은 Texture(sprite sheet)를 바탕으로 json data에 따라 sprite들을 생성하고, spriteList에 저장한다.
void AnimationClip::LoadSpriteSheetFromJson(shared_ptr<Texture2D> texture, const string& jsonPath)
{
    nlohmann::json j = nlohmann::json::parse(ifstream(jsonPath));

    auto sprites = j["sprites"];
    for (const auto& spriteJson : sprites)
    {
        string name = spriteJson["name"].get<string>();
        float x = spriteJson["x"].get<float>();
        float y = spriteJson["y"].get<float>();
        float w = spriteJson["width"].get<float>();
        float h = spriteJson["height"].get<float>();
        float pivotX = spriteJson["pivotX"].get<float>();
        float pivotY = spriteJson["pivotY"].get<float>();

        D2D1_RECT_F rect = { x, y, x + w, y + h };
        D2D1_POINT_2F pivot = { pivotX, pivotY };

        // Texture2D 일부 영역을 가지는 Sprite 생성
        auto sprite = ResourceManager::Get().CreateSprite(texture, name, rect, pivot);
        spritesList.push_back(sprite);
    }
}

/// LoadAnimationClipFromJson
/// Animation Clip에 대한 json을 읽고 어떤 AnimationFrame data를 저장한다.(sprite, time)
void AnimationClip::LoadAnimationClipFromJson(shared_ptr<Texture2D> texture, const string& jsonPath)
{
    nlohmann::json j = nlohmann::json::parse(ifstream(jsonPath));

    name = j["clipName"].get<std::string>();
    loop = j["loop"].get<bool>();
    duration = j["duration"].get<float>();

    const auto& jsonFrames = j["frames"];
    for (const auto& frameJson : jsonFrames)
    {
        string spriteName = frameJson["sprite"].get<string>();
        float time = frameJson["time"].get<float>();

        // 함수 자체는 Texture 전체 영역을 Sprite로 생성하는 생성자를 호출하지만,
        // 위에서 이미 sprite들을 만들었으므로 shared_prt만 받게 됨
        auto sprite = ResourceManager::Get().CreateSprite(texture, spriteName);

        // animation frame create
        AnimationFrame frame;
        frame.sprite = sprite;
        frame.time = time;
        frames.push_back(frame);
    }
}