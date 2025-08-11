#pragma once
#include <dwrite.h>
#include <vector>
#include <wrl/client.h>

class FontFileLoader;

class FontFileEnumerator;

class FontCollectionLoader : public IDWriteFontCollectionLoader
{
private:
    ULONG refCount = 1;
    Microsoft::WRL::ComPtr<FontFileLoader> fileLoader;
    std::vector<uint8_t> fontData;

public:
    FontCollectionLoader(const void* data, size_t size);
    ~FontCollectionLoader();

    // IUnknown
    IFACEMETHOD(QueryInterface)(REFIID iid, void** ppvObject) override;
    IFACEMETHOD_(ULONG, AddRef)() override;
    IFACEMETHOD_(ULONG, Release)() override;

    // IDWriteFontCollectionLoader
    IFACEMETHOD(CreateEnumeratorFromKey)(
        IDWriteFactory* factory,
        const void* collectionKey,
        UINT32 collectionKeySize,
        IDWriteFontFileEnumerator** fontFileEnumerator
        ) override;
};
