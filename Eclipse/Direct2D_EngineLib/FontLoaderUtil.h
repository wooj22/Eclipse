#pragma once
#include <dwrite.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include "FontCollectionLoader.h"

class FontCollectionLoader;

class FontLoaderUtil
{
private:
    Microsoft::WRL::ComPtr<FontCollectionLoader> collectionLoader;
    Microsoft::WRL::ComPtr<IDWriteFontCollection> customFontCollection;

    std::vector<uint8_t> LoadFontFileToMemory(const std::wstring& path);

public:
    // ttf 파일 경로로부터 커스텀 폰트 컬렉션 생성
    void LoadFontFromFile(const std::wstring& filepath);

    void LoadFontFromMemory(const void* data, size_t size);
    IDWriteFontCollection* GetFontCollection() const { return customFontCollection.Get(); }
};
