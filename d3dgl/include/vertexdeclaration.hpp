#ifndef VERTEXDECLARATION_HPP
#define VERTEXDECLARATION_HPP

#include <atomic>
#include <vector>
#include "d3d9.h"

#include "glew.h"


class D3DGLDevice;

struct D3DGLVERTEXELEMENT : public D3DVERTEXELEMENT9 {
    GLenum mGLType;
    GLint mGLCount;
    GLenum mNormalize;
};

class D3DGLVertexDeclaration : public IDirect3DVertexDeclaration9 {
    std::atomic<ULONG> mRefCount;
    std::atomic<ULONG> mIfaceCount;

    D3DGLDevice *mParent;
    bool mIsAuto;

    std::vector<D3DGLVERTEXELEMENT> mElements;
    DWORD mFvf;

public:
    D3DGLVertexDeclaration(D3DGLDevice *parent);
    virtual ~D3DGLVertexDeclaration();

    HRESULT init(DWORD fvf, bool isauto=false);
    HRESULT init(const D3DVERTEXELEMENT9 *elems, bool isauto=false);

    ULONG addIface() { return ++mIfaceCount; }
    ULONG releaseIface();

    DWORD getFvf() const { return mFvf; }

    const std::vector<D3DGLVERTEXELEMENT> &getVtxElements() const
    { return mElements; }

    static bool isEnd(const D3DVERTEXELEMENT9 &elem)
    { return elem.Stream==0xff; }

    /*** IUnknown methods ***/
    virtual HRESULT WINAPI QueryInterface(REFIID riid, void **obj);
    virtual ULONG WINAPI AddRef();
    virtual ULONG WINAPI Release();
    /*** IDirect3DVertexDeclaration9 methods ***/
    virtual HRESULT WINAPI GetDevice(IDirect3DDevice9 **device);
    virtual HRESULT WINAPI GetDeclaration(D3DVERTEXELEMENT9 *elems, UINT *count);
};

#endif /* VERTEXDECLARATION_HPP */
