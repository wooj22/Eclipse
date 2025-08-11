#include "FontLoaderUtil.h"
#include "FontCollectionLoader.h"
#include "RenderSystem.h"
#include <iostream> 

std::vector<uint8_t> FontLoaderUtil::LoadFontFileToMemory(const std::wstring& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open font file");
    }

    size_t size = static_cast<size_t>(file.tellg());
    std::vector<uint8_t> buffer(size);

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer.data()), size);

    return buffer;
}

void FontLoaderUtil::LoadFontFromFile(const std::wstring& filepath)
{
    // 1. ttf 파일을 메모리로 읽기
    std::vector<uint8_t> fontData;
    try
    {
        fontData = LoadFontFileToMemory(filepath);
    }
    catch (const std::exception& e)
    {
        std::wcerr << L"FontLoaderUtil::LoadFontFromFile - " << e.what() << L"\n";
        return;
    }

    // 2. FontCollectionLoader 생성 (내부에서 FontFileLoader 등도 생성됨)
    collectionLoader = Microsoft::WRL::ComPtr<FontCollectionLoader>(new FontCollectionLoader(fontData.data(), fontData.size()));

    // 3. DirectWrite Factory에서 커스텀 폰트 컬렉션 생성
    // RenderSystem::Get().dWriteFactory 는 IDWriteFactory* 인 것으로 가정
    HRESULT hr = RenderSystem::Get().dWriteFactory->CreateCustomFontCollection(
        collectionLoader.Get(),
        nullptr,
        0,
        &customFontCollection);

    if (FAILED(hr))
    {
        std::wcerr << L"Failed to create custom font collection. HRESULT=" << hr << L"\n";
        customFontCollection = nullptr;
        return;
    }

    std::wcout << L"Custom font collection created successfully.\n";
}

void FontLoaderUtil::LoadFontFromMemory(const void* data, size_t size)
{
    // FontCollectionLoader 생성 및 등록
    collectionLoader = Microsoft::WRL::ComPtr<FontCollectionLoader>(new FontCollectionLoader(data, size));
    RenderSystem::Get().dWriteFactory->RegisterFontCollectionLoader(collectionLoader.Get());

    // 폰트 컬렉션 생성
    RenderSystem::Get().dWriteFactory->CreateCustomFontCollection(
        collectionLoader.Get(),
        nullptr,  // collection key (null이면 loader 내부에서 처리)
        0,
        &customFontCollection
    );
}
