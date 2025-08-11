#pragma once
#include <dwrite.h>
#include <wrl/client.h>
#include <cstdint>

class FontFileLoader : public IDWriteFontFileLoader
{
private:
    ULONG refCount = 1;
    const void* fontData = nullptr;
    UINT32 fontSize = 0;

public:
    FontFileLoader(const void* data, UINT32 size);
    ~FontFileLoader();

    // IUnknown
    IFACEMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    IFACEMETHOD_(ULONG, AddRef)() override;
    IFACEMETHOD_(ULONG, Release)() override;

    // IDWriteFontFileLoader
    IFACEMETHOD(CreateStreamFromKey)(
        const void* fontFileReferenceKey,
        UINT32 fontFileReferenceKeySize,
        IDWriteFontFileStream** fontFileStream) override;

    // Å°¿ë getter
    const void* GetLoaderKey() const { return fontData; }
    UINT32 GetLoaderKeySize() const { return fontSize; }
};
