#include "FontLoaderUtil.h"
#include "FontCollectionLoader.h"

FontLoaderUtil::FontLoaderUtil(IDWriteFactory* factory)
    : dwriteFactory(factory)
{
}

HRESULT FontLoaderUtil::LoadFontFromMemory(const void* data, size_t size)
{
    if (!dwriteFactory)
        return E_FAIL;

    // FontCollectionLoader 생성 및 등록
    collectionLoader = Microsoft::WRL::ComPtr<FontCollectionLoader>(new FontCollectionLoader(data, size));

    HRESULT hr = dwriteFactory->RegisterFontCollectionLoader(collectionLoader.Get());
    /*if (FAILED(hr) && hr != DWRITE_E_LOADER_ALREADY_REGISTERED)
        return hr;*/

    // 폰트 컬렉션 생성
    hr = dwriteFactory->CreateCustomFontCollection(
        collectionLoader.Get(),
        nullptr,  // collection key (null이면 loader 내부에서 처리)
        0,
        &customFontCollection
    );
    return hr;
}
