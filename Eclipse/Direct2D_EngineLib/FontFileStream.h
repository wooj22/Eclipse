#pragma once
#include <dwrite.h>
#include <wrl/client.h>
#include <cstdint>

class FontFileStream : public IDWriteFontFileStream
{
private:
    ULONG refCount = 1;
    const uint8_t* fontData;
    UINT32 fontSize;

public:
    FontFileStream(const void* data, UINT32 size);
    virtual ~FontFileStream();

    // IUnknown
    IFACEMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    IFACEMETHOD_(ULONG, AddRef)() override;
    IFACEMETHOD_(ULONG, Release)() override;

    // IDWriteFontFileStream
    IFACEMETHOD(ReadFileFragment)(
        const void** fragmentStart,
        UINT64 fileOffset,
        UINT64 fragmentSize,
        void** fragmentContext) override;

    IFACEMETHOD_(void, ReleaseFileFragment)(void* fragmentContext) override;

    IFACEMETHOD(GetFileSize)(UINT64* fileSize) override;
    IFACEMETHOD(GetLastWriteTime)(UINT64* lastWriteTime) override;
};
