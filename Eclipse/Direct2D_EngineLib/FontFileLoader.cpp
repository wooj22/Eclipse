#include "FontFileLoader.h"
#include "FontFileStream.h"

FontFileLoader::FontFileLoader(const void* data, UINT32 size)
    : fontData(data), fontSize(size)
{
}

FontFileLoader::~FontFileLoader()
{
}

HRESULT FontFileLoader::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject) return E_POINTER;

    if (riid == __uuidof(IUnknown) || riid == __uuidof(IDWriteFontFileLoader))
    {
        *ppvObject = static_cast<IDWriteFontFileLoader*>(this);
        AddRef();
        return S_OK;
    }

    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG FontFileLoader::AddRef()
{
    return InterlockedIncrement(&refCount);
}

ULONG FontFileLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount);
    if (newCount == 0)
        delete this;
    return newCount;
}

HRESULT FontFileLoader::CreateStreamFromKey(
    const void* fontFileReferenceKey,
    UINT32 fontFileReferenceKeySize,
    IDWriteFontFileStream** fontFileStream)
{
    if (!fontFileReferenceKey || fontFileReferenceKeySize != fontSize)
        return E_FAIL;

    if (!fontFileStream) return E_POINTER;

    if (fontFileReferenceKey != fontData)
        return E_FAIL;

    *fontFileStream = new FontFileStream(fontData, fontSize);
    (*fontFileStream)->AddRef();

    return S_OK;
}
