#pragma once
#include <dwrite.h>
#include <wrl/client.h>
#include <string>

class FontCollectionLoader;

class FontLoaderUtil
{
private:
    Microsoft::WRL::ComPtr<IDWriteFactory> dwriteFactory;
    Microsoft::WRL::ComPtr<FontCollectionLoader> collectionLoader;
    Microsoft::WRL::ComPtr<IDWriteFontCollection> customFontCollection;

public:
    FontLoaderUtil(IDWriteFactory* factory);

    HRESULT LoadFontFromMemory(const void* data, size_t size);

    IDWriteFontCollection* GetFontCollection() const { return customFontCollection.Get(); }
};
