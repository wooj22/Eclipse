#include "FontFileEnumerator.h"
#include "RenderSystem.h"

FontFileEnumerator::FontFileEnumerator(Microsoft::WRL::ComPtr<FontFileLoader>  loader)
    : fontFileLoader(loader), moved(false)
{
    if (fontFileLoader) fontFileLoader->AddRef();
}

HRESULT FontFileEnumerator::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject) return E_POINTER;

    if (riid == __uuidof(IUnknown) || riid == __uuidof(IDWriteFontFileEnumerator))
    {
        *ppvObject = static_cast<IDWriteFontFileEnumerator*>(this);
        AddRef();
        return S_OK;
    }

    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG FontFileEnumerator::AddRef()
{
    return InterlockedIncrement(&refCount);
}

ULONG FontFileEnumerator::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount);
    if (newCount == 0)
        delete this;
    return newCount;
}

HRESULT FontFileEnumerator::MoveNext(BOOL* hasCurrentFile)
{
    if (!hasCurrentFile) return E_POINTER;

    if (!moved)
    {
        *hasCurrentFile = TRUE;
        moved = true;
        return S_OK;
    }
    *hasCurrentFile = FALSE;
    return S_OK;
}

HRESULT FontFileEnumerator::GetCurrentFontFile(IDWriteFontFile** fontFile)
{
    if (!fontFile) return E_POINTER;

    // fontFileLoader의 키와 크기
    const void* key = fontFileLoader->GetLoaderKey();
    UINT32 keySize = fontFileLoader->GetLoaderKeySize();

    // IDWriteFactory::CreateCustomFontFileReference 호출
    return RenderSystem::Get().dWriteFactory->CreateCustomFontFileReference(
        key,                     // const void* fontFileReferenceKey
        keySize,                 // UINT32 fontFileReferenceKeySize
        fontFileLoader.Get(),    // IDWriteFontFileLoader* fontFileLoader
        fontFile                 // IDWriteFontFile**
    );
}
