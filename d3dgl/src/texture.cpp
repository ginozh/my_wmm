#define NOMINMAX
#include "texture.hpp"

#include <limits>

#include "trace.hpp"
#include "glformat.hpp"
#include "d3dgl.hpp"
#include "device.hpp"
#include "adapter.hpp"
#include "private_iids.hpp"


void D3DGLTexture::initGL()
{
    UINT total_size = 0;
    GLint w = mDesc.Width;
    GLint h = mDesc.Height;
    for(D3DGLTextureSurface *surface : mSurfaces)
    {
        w = std::max(1, w);
        h = std::max(1, h);

        UINT level_size;
        if(mDesc.Format == D3DFMT_DXT1 || mDesc.Format == D3DFMT_DXT2 ||
           mDesc.Format == D3DFMT_DXT3 || mDesc.Format == D3DFMT_DXT4 ||
           mDesc.Format == D3DFMT_DXT5 || mDesc.Format == D3DFMT_ATI1 ||
           mDesc.Format == D3DFMT_ATI2)
        {
            level_size  = mGLFormat->calcBlockPitch(w, mGLFormat->bytesperblock);
            level_size *= ((h+3)/4);
        }
        else
        {
            level_size  = mGLFormat->calcPitch(w, mGLFormat->bytesperpixel);
            level_size *= h;
        }

        surface->init(total_size, level_size);
        total_size += (level_size+15) & ~15;

        w >>= 1;
        h >>= 1;
    }

    glGenTextures(1, &mTexId);
    glTextureParameteriEXT(mTexId, GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mSurfaces.size()-1);
    glTextureImage2DEXT(mTexId, GL_TEXTURE_2D, 0, mGLFormat->internalformat, mDesc.Width, mDesc.Height, 0,
                        mGLFormat->format, mGLFormat->type, nullptr);
    checkGLError();

    // Force allocation of mipmap levels, if any
    if(mSurfaces.size() > 1)
    {
        glGenerateTextureMipmapEXT(mTexId, GL_TEXTURE_2D);
        checkGLError();
    }

    if(mDesc.Pool != D3DPOOL_DEFAULT)
        mSysMem.assign(total_size, 0);

    mUpdateInProgress = 0;
}
class TextureInitCmd : public Command {
    D3DGLTexture *mTarget;

public:
    TextureInitCmd(D3DGLTexture *target) : mTarget(target) { }

    virtual ULONG execute()
    {
        mTarget->initGL();
        return sizeof(*this);
    }
};

class TextureDeinitCmd : public Command {
    GLuint mTexId;

public:
    TextureDeinitCmd(GLuint texid) : mTexId(texid) { }

    virtual ULONG execute()
    {
        glDeleteTextures(1, &mTexId);
        checkGLError();
        return sizeof(*this);
    }
};


void D3DGLTexture::genMipmapGL()
{
    glGenerateTextureMipmapEXT(mTexId, GL_TEXTURE_2D);
    checkGLError();
}
class TextureGenMipCmd : public Command {
    D3DGLTexture *mTarget;

public:
    TextureGenMipCmd(D3DGLTexture *target) : mTarget(target) { }

    virtual ULONG execute()
    {
        mTarget->genMipmapGL();
        return sizeof(*this);
    }
};


void D3DGLTexture::loadTexLevelGL(DWORD level, const RECT &rect, const GLubyte *dataPtr)
{
    UINT w = std::max(1u, mDesc.Width>>level);
    /*UINT h = std::max(1u, mDesc.Height>>Level);*/

    D3DGLTextureSurface *surface = mSurfaces[level];
    if(mIsCompressed)
    {
        GLsizei len = -1;
        if(mDesc.Format == D3DFMT_DXT1 || mDesc.Format == D3DFMT_DXT2 ||
           mDesc.Format == D3DFMT_DXT3 || mDesc.Format == D3DFMT_DXT4 ||
           mDesc.Format == D3DFMT_DXT5 || mDesc.Format == D3DFMT_ATI1 ||
           mDesc.Format == D3DFMT_ATI2)
        {
            int pitch = mGLFormat->calcBlockPitch(w, mGLFormat->bytesperblock);
            int offset = (rect.top/4*pitch) + (rect.left/4*mGLFormat->bytesperblock);
            len = surface->getDataLength() - offset;
            dataPtr += offset;
        }
        glCompressedTextureSubImage2DEXT(mTexId, GL_TEXTURE_2D, level,
            rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
            mGLFormat->internalformat, len, dataPtr
        );
    }
    else
    {
        int pitch = mGLFormat->calcPitch(w, mGLFormat->bytesperpixel);
        dataPtr += (rect.top*pitch) + (rect.left*mGLFormat->bytesperpixel);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, w);
        glTextureSubImage2DEXT(mTexId, GL_TEXTURE_2D, level,
            rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
            mGLFormat->format, mGLFormat->type, dataPtr
        );
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }

    if(level == 0 && (mDesc.Usage&D3DUSAGE_AUTOGENMIPMAP) && mSurfaces.size() > 1)
        glGenerateTextureMipmapEXT(mTexId, GL_TEXTURE_2D);
    checkGLError();

    --mUpdateInProgress;
}
class TextureLoadLevelCmd : public Command {
    D3DGLTexture *mTarget;
    DWORD mLevel;
    RECT mRect;
    const GLubyte *mDataPtr;

public:
    TextureLoadLevelCmd(D3DGLTexture *target, DWORD level, const RECT &rect, const GLubyte *dataPtr)
      : mTarget(target), mLevel(level), mRect(rect), mDataPtr(dataPtr)
    { }

    virtual ULONG execute()
    {
        mTarget->loadTexLevelGL(mLevel, mRect, mDataPtr);
        return sizeof(*this);
    }
};


D3DGLTexture::D3DGLTexture(D3DGLDevice *parent)
  : mRefCount(0)
  , mIfaceCount(0)
  , mParent(parent)
  , mGLFormat(nullptr)
  , mTexId(0)
  , mDirtyRect({std::numeric_limits<LONG>::max(), std::numeric_limits<LONG>::max(),
                std::numeric_limits<LONG>::min(), std::numeric_limits<LONG>::min()})
  , mUpdateInProgress(0)
  , mLodLevel(0)
{
}

D3DGLTexture::~D3DGLTexture()
{
    if(mTexId)
    {
        mParent->getQueue().send<TextureDeinitCmd>(mTexId);
        while(mUpdateInProgress)
            mParent->getQueue().wakeAndSleep();
        mTexId = 0;
    }

    for(auto surface : mSurfaces)
        delete surface;
    mSurfaces.clear();
}


bool D3DGLTexture::init(const D3DSURFACE_DESC *desc, UINT levels)
{
    mDesc = *desc;

    if(mDesc.Width == 0 || mDesc.Height == 0)
    {
        ERR("Width of height of 0: %ux%u\n", mDesc.Width, mDesc.Height);
        return false;
    }

    auto fmtinfo = gFormatList.find(mDesc.Format);
    if(fmtinfo == gFormatList.end())
    {
        ERR("Failed to find info for format %s\n", d3dfmt_to_str(mDesc.Format));
        return false;
    }
    mGLFormat = &fmtinfo->second;

    if((mDesc.Usage&D3DUSAGE_RENDERTARGET))
    {
        if(mDesc.Pool != D3DPOOL_DEFAULT)
        {
            WARN("RenderTarget not allowed in non-default pool\n");
            return false;
        }
    }
    else if((mDesc.Usage&D3DUSAGE_DEPTHSTENCIL))
    {
        if(mDesc.Pool != D3DPOOL_DEFAULT)
        {
            WARN("DepthStencil target not allowed in non-default pool\n");
            return false;
        }
    }

    if((mDesc.Usage&D3DUSAGE_AUTOGENMIPMAP))
    {
        if(mDesc.Pool == D3DPOOL_SYSTEMMEM)
        {
            WARN("AutoGenMipMap not allowed in systemmem\n");
            return false;
        }
        if(mDesc.Pool == D3DPOOL_MANAGED)
        {
            if(levels > 1)
            {
                WARN("Cannot AutoGenMipMap managed textures\n");
                return false;
            }
            levels = 1;
        }
    }

    mIsCompressed = (mDesc.Format == D3DFMT_DXT1 || mDesc.Format == D3DFMT_DXT2 ||
                     mDesc.Format == D3DFMT_DXT3 || mDesc.Format == D3DFMT_DXT4 ||
                     mDesc.Format == D3DFMT_DXT5 || mDesc.Format == D3DFMT_ATI1 ||
                     mDesc.Format == D3DFMT_ATI2);
    if(mDesc.Format == D3DFMT_DXT2 || mDesc.Format == D3DFMT_DXT4)
        WARN("Pre-mulitplied alpha textures not supported; loading anyway.");

    UINT maxLevels = 0;
    UINT m = std::max(mDesc.Width, mDesc.Height);
    while(m > 0)
    {
        maxLevels++;
        m >>= 1;
    }
    TRACE("Calculated max mipmap levels: %u (requested: %u)\n", maxLevels, levels);

    if(!levels || levels > maxLevels)
        levels = maxLevels;
    for(UINT i = 0;i < levels;++i)
        mSurfaces.push_back(new D3DGLTextureSurface(this, i));

    if(mDesc.Format != D3DFMT_NULL)
    {
        mUpdateInProgress = 1;
        mParent->getQueue().sendSync<TextureInitCmd>(this);
    }

    return true;
}

void D3DGLTexture::updateTexture(DWORD level, const RECT &rect, const GLubyte *dataPtr)
{
    CommandQueue &queue = mParent->getQueue();
    queue.lock();
    ++mUpdateInProgress;
    queue.doSend<TextureLoadLevelCmd>(this, level, rect, dataPtr);
    queue.unlock();
}

void D3DGLTexture::addIface()
{
    ++mIfaceCount;
}

void D3DGLTexture::releaseIface()
{
    if(--mIfaceCount == 0)
        delete this;
}


HRESULT D3DGLTexture::QueryInterface(REFIID riid, void **obj)
{
    TRACE("iface %p, riid %s, obj %p\n", this, debugstr_guid(riid), obj);

    *obj = NULL;
    RETURN_IF_IID_TYPE(obj, riid, D3DGLTexture);
    RETURN_IF_IID_TYPE(obj, riid, IDirect3DTexture9);
    RETURN_IF_IID_TYPE(obj, riid, IDirect3DBaseTexture9);
    RETURN_IF_IID_TYPE(obj, riid, IDirect3DResource9);
    RETURN_IF_IID_TYPE(obj, riid, IUnknown);

    TRACE("Unsupported interface %s\n", debugstr_guid(riid));
    return E_NOINTERFACE;
}

ULONG D3DGLTexture::AddRef()
{
    ULONG ret = ++mRefCount;
    TRACE("%p New refcount: %lu\n", this, ret);
    if(ret == 1)
    {
        mParent->AddRef();
        addIface();
    }
    return ret;
}

ULONG D3DGLTexture::Release()
{
    ULONG ret = --mRefCount;
    TRACE("%p New refcount: %lu\n", this, ret);
    if(ret == 0)
    {
        D3DGLDevice *parent = mParent;
        releaseIface();
        parent->Release();
    }
    return ret;
}


HRESULT D3DGLTexture::GetDevice(IDirect3DDevice9 **device)
{
    TRACE("iface %p, device %p\n", this, device);
    *device = mParent;
    (*device)->AddRef();
    return D3D_OK;
}

HRESULT D3DGLTexture::SetPrivateData(REFGUID refguid, const void *data, DWORD size, DWORD flags)
{
    FIXME("iface %p, refguid %s, data %p, size %lu, flags 0x%lx : stub!\n", this, debugstr_guid(refguid), data, size, flags);
    return E_NOTIMPL;
}

HRESULT D3DGLTexture::GetPrivateData(REFGUID refguid, void *data, DWORD *size)
{
    FIXME("iface %p, refguid %s, data %p, size %p : stub!\n", this, debugstr_guid(refguid), data, size);
    return E_NOTIMPL;
}

HRESULT D3DGLTexture::FreePrivateData(REFGUID refguid)
{
    FIXME("iface %p, refguid %s : stub!\n", this, debugstr_guid(refguid));
    return E_NOTIMPL;
}

DWORD D3DGLTexture::SetPriority(DWORD priority)
{
    FIXME("iface %p, priority %lu : stub!\n", this, priority);
    return 0;
}

DWORD D3DGLTexture::GetPriority()
{
    FIXME("iface %p : stub!\n", this);
    return 0;
}

void D3DGLTexture::PreLoad()
{
    FIXME("iface %p : stub!\n", this);
}

D3DRESOURCETYPE D3DGLTexture::GetType()
{
    TRACE("iface %p\n", this);
    return D3DRTYPE_TEXTURE;
}


DWORD D3DGLTexture::SetLOD(DWORD lod)
{
    TRACE("iface %p, lod %lu\n", this, lod);

    if(mDesc.Pool != D3DPOOL_MANAGED || mDesc.Format == D3DFMT_NULL)
        return 0;

    lod = std::min(lod, (DWORD)mSurfaces.size()-1);

    // FIXME: Set GL_TEXTURE_BASE_LEVEL? Or GL_TEXTURE_MIN_LOD?
    lod = mLodLevel.exchange(lod);

    return lod;
}

DWORD D3DGLTexture::GetLOD()
{
    TRACE("iface %p\n", this);
    return mLodLevel.load();
}

DWORD D3DGLTexture::GetLevelCount()
{
    TRACE("iface %p\n", this);
    return mSurfaces.size();
}

HRESULT D3DGLTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE type)
{
    FIXME("iface %p, type 0x%x : stub!\n", this, type);
    return D3D_OK;
}

D3DTEXTUREFILTERTYPE D3DGLTexture::GetAutoGenFilterType()
{
    FIXME("iface %p\n", this);
    return D3DTEXF_LINEAR;
}

void D3DGLTexture::GenerateMipSubLevels()
{
    TRACE("iface %p\n", this);
    if(mDesc.Format != D3DFMT_NULL)
        mParent->getQueue().send<TextureGenMipCmd>(this);
}


HRESULT D3DGLTexture::GetLevelDesc(UINT level, D3DSURFACE_DESC *desc)
{
    TRACE("iface %p, level %u, desc %p\n", this, level, desc);

    if(level >= mSurfaces.size())
    {
        WARN("Level out of range (%u >= %u)\n", level, mSurfaces.size());
        return D3DERR_INVALIDCALL;
    }

    return mSurfaces[level]->GetDesc(desc);
}

HRESULT D3DGLTexture::GetSurfaceLevel(UINT level, IDirect3DSurface9 **surface)
{
    TRACE("iface %p, level %u, surface %p\n", this, level, surface);

    if(level >= mSurfaces.size())
    {
        WARN("Level out of range (%u >= %u)\n", level, mSurfaces.size());
        return D3DERR_INVALIDCALL;
    }

    *surface = mSurfaces[level];
    (*surface)->AddRef();
    return D3D_OK;
}

HRESULT D3DGLTexture::LockRect(UINT level, D3DLOCKED_RECT *lockedRect, const RECT *rect, DWORD flags)
{
    TRACE("iface %p, level %u, lockedRect %p, rect %p, flags 0x%lx\n", this, level, lockedRect, rect, flags);

    if(level >= mSurfaces.size())
    {
        WARN("Level out of range (%u >= %u)\n", level, mSurfaces.size());
        return D3DERR_INVALIDCALL;
    }

    return mSurfaces[level]->LockRect(lockedRect, rect, flags);
}

HRESULT D3DGLTexture::UnlockRect(UINT level)
{
    TRACE("iface %p, level %u\n", this, level);

    if(level >= mSurfaces.size())
    {
        WARN("Level out of range (%u >= %u)\n", level, mSurfaces.size());
        return D3DERR_INVALIDCALL;
    }

    return mSurfaces[level]->UnlockRect();
}

HRESULT D3DGLTexture::AddDirtyRect(const RECT *rect)
{
    TRACE("iface %p, rect %p\n", this, rect);
    mDirtyRect.left = std::min(mDirtyRect.left, rect->left);
    mDirtyRect.top = std::min(mDirtyRect.top, rect->top);
    mDirtyRect.right = std::max(mDirtyRect.right, rect->right);
    mDirtyRect.bottom = std::max(mDirtyRect.bottom, rect->bottom);
    return D3D_OK;
}



D3DGLTextureSurface::D3DGLTextureSurface(D3DGLTexture *parent, UINT level)
  : mParent(parent)
  , mLevel(level)
  , mLock(LT_Unlocked)
  , mDataOffset(0)
  , mDataLength(0)
{
}

D3DGLTextureSurface::~D3DGLTextureSurface()
{
}

void D3DGLTextureSurface::init(UINT offset, UINT length)
{
    mDataOffset = offset;
    mDataLength = length;
}


HRESULT D3DGLTextureSurface::QueryInterface(REFIID riid, void **obj)
{
    TRACE("iface %p, riid %s, obj %p\n", this, debugstr_guid(riid), obj);

    *obj = NULL;
    RETURN_IF_IID_TYPE(obj, riid, D3DGLTextureSurface);
    RETURN_IF_IID_TYPE(obj, riid, IDirect3DSurface9);
    RETURN_IF_IID_TYPE(obj, riid, IDirect3DResource9);
    RETURN_IF_IID_TYPE(obj, riid, IUnknown);

    TRACE("Unsupported interface %s\n", debugstr_guid(riid));
    return E_NOINTERFACE;
}

ULONG D3DGLTextureSurface::AddRef()
{
    TRACE("iface %p\n", this);
    return mParent->AddRef();
}

ULONG D3DGLTextureSurface::Release()
{
    TRACE("iface %p\n", this);
    return mParent->Release();
}


HRESULT D3DGLTextureSurface::GetDevice(IDirect3DDevice9 **device)
{
    TRACE("iface %p, device %p\n", this, device);
    return mParent->GetDevice(device);
}

HRESULT D3DGLTextureSurface::SetPrivateData(REFGUID refguid, const void *data, DWORD size, DWORD flags)
{
    FIXME("iface %p, refguid %s, data %p, size %lu, flags 0x%lx : stub!\n", this, debugstr_guid(refguid), data, size, flags);
    return E_NOTIMPL;
}

HRESULT D3DGLTextureSurface::GetPrivateData(REFGUID refguid, void *data, DWORD *size)
{
    FIXME("iface %p, refguid %s, data %p, size %p : stub!\n", this, debugstr_guid(refguid), data, size);
    return E_NOTIMPL;
}

HRESULT D3DGLTextureSurface::FreePrivateData(REFGUID refguid)
{
    FIXME("iface %p, refguid %s : stub!\n", this, debugstr_guid(refguid));
    return E_NOTIMPL;
}

DWORD D3DGLTextureSurface::SetPriority(DWORD priority)
{
    FIXME("iface %p, priority %lu : stub!\n", this, priority);
    return 0;
}

DWORD D3DGLTextureSurface::GetPriority()
{
    FIXME("iface %p : stub!\n", this);
    return 0;
}

void D3DGLTextureSurface::PreLoad()
{
    FIXME("iface %p : stub!\n", this);
}

D3DRESOURCETYPE D3DGLTextureSurface::GetType()
{
    TRACE("iface %p\n", this);
    return D3DRTYPE_SURFACE;
}


HRESULT D3DGLTextureSurface::GetContainer(REFIID riid, void **container)
{
    TRACE("iface %p, riid %s, container %p\n", this, debugstr_guid(riid), container);
    return mParent->QueryInterface(riid, container);
}

HRESULT D3DGLTextureSurface::GetDesc(D3DSURFACE_DESC *desc)
{
    TRACE("iface %p, desc %p\n", this, desc);

    desc->Format = mParent->mDesc.Format;
    desc->Usage = mParent->mDesc.Usage;
    desc->Pool = mParent->mDesc.Pool;
    desc->MultiSampleType = mParent->mDesc.MultiSampleType;
    desc->MultiSampleQuality = mParent->mDesc.MultiSampleQuality;
    desc->Width = std::max(1u, mParent->mDesc.Width>>mLevel);
    desc->Height = std::max(1u, mParent->mDesc.Height>>mLevel);
    return D3D_OK;
}

HRESULT D3DGLTextureSurface::LockRect(D3DLOCKED_RECT *lockedRect, const RECT *rect, DWORD flags)
{
    TRACE("iface %p, lockedRect %p, rect %p, flags 0x%lx\n", this, lockedRect, rect, flags);

    if(mParent->mDesc.Format == D3DFMT_NULL)
    {
        FIXME("Attempting to lock NULL format texture\n");
        return D3DERR_INVALIDCALL;
    }

    if(mParent->mDesc.Pool == D3DPOOL_DEFAULT)
    {
        FIXME("Trying to lock texture in default pool\n");
        return D3DERR_INVALIDCALL;
    }

    DWORD unknown_flags = flags & ~(D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE|D3DLOCK_READONLY|D3DLOCK_NO_DIRTY_UPDATE);
    if(unknown_flags) FIXME("Unknown lock flags: 0x%lx\n", unknown_flags);

    UINT w = std::max(1u, mParent->mDesc.Width>>mLevel);
    UINT h = std::max(1u, mParent->mDesc.Height>>mLevel);
    RECT full = { 0, 0, (LONG)w, (LONG)h };
    if((flags&D3DLOCK_DISCARD))
    {
        if((flags&D3DLOCK_READONLY))
        {
            WARN("Read-only discard specified\n");
            return D3DERR_INVALIDCALL;
        }
        if(rect)
        {
            WARN("Discardable rect specified\n");
            return D3DERR_INVALIDCALL;
        }
    }
    if(!rect)
        rect = &full;

    {
        LockType lt = ((flags&D3DLOCK_READONLY) ? LT_ReadOnly : LT_Full);
        LockType nolock = LT_Unlocked;
        if(!mLock.compare_exchange_strong(nolock, lt))
        {
            WARN("Texture surface %u already locked!\n", mLevel);
            return D3DERR_INVALIDCALL;
        }
    }

    // No need to wait if we're not writing over previous data.
    if(!(flags&D3DLOCK_NOOVERWRITE) && !(flags&D3DLOCK_READONLY))
    {
        while(mParent->mUpdateInProgress)
            mParent->mParent->getQueue().wakeAndSleep();
    }

    GLubyte *memPtr = &mParent->mSysMem[mDataOffset];
    mLockRegion = *rect;
    if(mParent->mIsCompressed && !(mParent->mGLFormat->flags&GLFormatInfo::BadPitch))
    {
        int pitch = GLFormatInfo::calcBlockPitch(w, mParent->mGLFormat->bytesperblock);
        memPtr += (rect->top/4*pitch) + (rect->left/4*mParent->mGLFormat->bytesperblock);
        lockedRect->Pitch = pitch;
    }
    else
    {
        int pitch = GLFormatInfo::calcPitch(w, mParent->mGLFormat->bytesperpixel);
        memPtr += (rect->top*pitch) + (rect->left*mParent->mGLFormat->bytesperpixel);
        lockedRect->Pitch = pitch;
    }
    lockedRect->pBits = memPtr;

    if(!(flags&(D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_READONLY)))
    {
        RECT dirty = { rect->left<<mLevel, rect->top<<mLevel,
                       rect->right<<mLevel, rect->bottom<<mLevel };
        mParent->AddDirtyRect(&dirty);
    }

    TRACE("Locked region: pBits=%p, Pitch=%d\n", lockedRect->pBits, lockedRect->Pitch);
    return D3D_OK;
}

HRESULT D3DGLTextureSurface::UnlockRect()
{
    TRACE("iface %p\n", this);

    if(mLock == LT_Unlocked)
    {
        ERR("Attempted to unlock an unlocked surface\n");
        return D3DERR_INVALIDCALL;
    }

    if(mLock != LT_ReadOnly)
        mParent->updateTexture(mLevel, mLockRegion, &mParent->mSysMem[mDataOffset]);

    mLock = LT_Unlocked;
    return D3D_OK;
}

HRESULT D3DGLTextureSurface::GetDC(HDC *hdc)
{
    FIXME("iface %p, hdc %p : stub!\n", this, hdc);
    return E_NOTIMPL;
}

HRESULT D3DGLTextureSurface::ReleaseDC(HDC hdc)
{
    FIXME("iface %p, hdc %p : stub!\n", this, hdc);
    return E_NOTIMPL;
}
