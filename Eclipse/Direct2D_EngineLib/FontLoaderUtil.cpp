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

    // FontCollectionLoader ���� �� ���
    collectionLoader = Microsoft::WRL::ComPtr<FontCollectionLoader>(new FontCollectionLoader(data, size));

    HRESULT hr = dwriteFactory->RegisterFontCollectionLoader(collectionLoader.Get());
    /*if (FAILED(hr) && hr != DWRITE_E_LOADER_ALREADY_REGISTERED)
        return hr;*/

    // ��Ʈ �÷��� ����
    hr = dwriteFactory->CreateCustomFontCollection(
        collectionLoader.Get(),
        nullptr,  // collection key (null�̸� loader ���ο��� ó��)
        0,
        &customFontCollection
    );
    return hr;
}
