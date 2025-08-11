#include "FontCollectionLoader.h"
#include "FontFileLoader.h"
#include "FontFileEnumerator.h"
#include "RenderSystem.h"

FontCollectionLoader::FontCollectionLoader(const void* data, size_t size)
    : fontData((uint8_t*)data, (uint8_t*)data + size)
{
    fileLoader = Microsoft::WRL::ComPtr<FontFileLoader>(new FontFileLoader(fontData.data(), static_cast<UINT32>(fontData.size())));
}

FontCollectionLoader::~FontCollectionLoader()
{
}

HRESULT FontCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
{
    if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontCollectionLoader))
    {
        *ppvObject = static_cast<IDWriteFontCollectionLoader*>(this);
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG FontCollectionLoader::AddRef()
{
    return InterlockedIncrement(&refCount);
}

ULONG FontCollectionLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount);
    if (newCount == 0)
        delete this;
    return newCount;
}

HRESULT FontCollectionLoader::CreateEnumeratorFromKey(
    IDWriteFactory* factory,
    const void* collectionKey,
    UINT32 collectionKeySize,
    IDWriteFontFileEnumerator** fontFileEnumerator)
{
    if (!factory || !fontFileEnumerator)
        return E_POINTER;

    // 새 FontFileEnumerator를 생성해서 반환
    *fontFileEnumerator = new FontFileEnumerator(fileLoader);
    if (*fontFileEnumerator == nullptr)
        return E_OUTOFMEMORY;

    (*fontFileEnumerator)->AddRef();
    return S_OK;
}
