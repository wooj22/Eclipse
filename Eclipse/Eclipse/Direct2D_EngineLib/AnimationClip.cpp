#include "AnimationClip.h"

/// LoadSpritesFromJson
/// �Ѱܹ��� Texture(sprite sheet)�� �������� json data�� ���� sprite���� �����ϰ�, spriteList�� �����Ѵ�.
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

        // Texture2D �Ϻ� ������ ������ Sprite ����
        auto sprite = ResourceManager::Get().CreateSprite(texture, name, rect, pivot);
        spritesList.push_back(sprite);
    }
}

/// LoadAnimationClipFromJson
/// Animation Clip�� ���� json�� �а� � AnimationFrame data�� �����Ѵ�.(sprite, time)
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

        // �Լ� ��ü�� Texture ��ü ������ Sprite�� �����ϴ� �����ڸ� ȣ��������,
        // ������ �̹� sprite���� ��������Ƿ� shared_prt�� �ް� ��
        auto sprite = ResourceManager::Get().CreateSprite(texture, spriteName);

        // animation frame create
        AnimationFrame frame;
        frame.sprite = sprite;
        frame.time = time;
        frames.push_back(frame);
    }
}