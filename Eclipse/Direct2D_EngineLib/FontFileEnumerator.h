#pragma once
#include <dwrite.h>
#include <wrl/client.h>
#include "FontFileLoader.h"

class FontFileEnumerator : public IDWriteFontFileEnumerator
{
private:
    ULONG refCount = 1;
    Microsoft::WRL::ComPtr<FontFileLoader> fontFileLoader;
    bool moved = false;

public:
    FontFileEnumerator(Microsoft::WRL::ComPtr<FontFileLoader> loader);

    // IUnknown
    IFACEMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    IFACEMETHOD_(ULONG, AddRef)() override;
    IFACEMETHOD_(ULONG, Release)() override;

    // IDWriteFontFileEnumerator
    IFACEMETHOD(MoveNext)(BOOL* hasCurrentFile) override;
    IFACEMETHOD(GetCurrentFontFile)(IDWriteFontFile** fontFile) override;
};
