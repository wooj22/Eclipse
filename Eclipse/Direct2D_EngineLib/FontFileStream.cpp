#include "FontFileStream.h"
#include <cstring>

FontFileStream::FontFileStream(const void* data, UINT32 size)
    : fontData(reinterpret_cast<const uint8_t*>(data)), fontSize(size)
{
}

FontFileStream::~FontFileStream()
{
}

HRESULT FontFileStream::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject) return E_POINTER;

    if (riid == __uuidof(IUnknown) || riid == __uuidof(IDWriteFontFileStream))
    {
        *ppvObject = static_cast<IDWriteFontFileStream*>(this);
        AddRef();
        return S_OK;
    }

    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG FontFileStream::AddRef()
{
    return InterlockedIncrement(&refCount);
}

ULONG FontFileStream::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount);
    if (newCount == 0)
        delete this;
    return newCount;
}

HRESULT FontFileStream::ReadFileFragment(
    const void** fragmentStart,
    UINT64 fileOffset,
    UINT64 fragmentSize,
    void** fragmentContext)
{
    if (fileOffset + fragmentSize > fontSize)
        return E_FAIL;

    *fragmentStart = fontData + fileOffset;
    *fragmentContext = nullptr;

    return S_OK;
}

void FontFileStream::ReleaseFileFragment(void* fragmentContext)
{
    // 할 일 없음
}

HRESULT FontFileStream::GetFileSize(UINT64* fileSize)
{
    if (!fileSize) return E_POINTER;
    *fileSize = fontSize;
    return S_OK;
}

HRESULT FontFileStream::GetLastWriteTime(UINT64* lastWriteTime)
{
    if (!lastWriteTime) return E_POINTER;
    *lastWriteTime = 0; // 무시
    return S_OK;
}
