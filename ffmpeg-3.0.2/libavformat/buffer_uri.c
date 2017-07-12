/*
 * storm
 */

#include <string.h>
#include "libavutil/avstring.h"
#include "libavutil/base64.h"
#include "url.h"
#include "ffmpeg.h"

typedef struct {
    //const uint8_t *data;
    uint8_t *data;
    void *tofree;
    size_t size;
    size_t pos;
    int flags;
    size_t filesize; //write
    size_t* out_size; // for write size
} BufferContext;

static av_cold int buffer_open(URLContext *h, const char *uri, int flags)
{
    BufferContext *dc = h->priv_data;
    const char *data, *opt, *next;
    char *ddata;
    int ret, base64 = 0;
    size_t in_size;
    struct to_buffer* sbuffer=NULL;
    dc->flags = flags;
    /* data:content/type[;base64],payload */

    av_strstart(uri, "buffer:", &uri);
    data = strchr(uri, ',');
    if (!data) {
        av_log(h, AV_LOG_ERROR, "No ',' delimiter in URI\n");
        return AVERROR(EINVAL);
    }
    opt = uri;
    while (opt < data) {
        next = av_x_if_null(memchr(opt, ';', data - opt), data);
        if (opt == uri) {
            if (!memchr(opt, '/', next - opt)) { /* basic validity check */
                av_log(h, AV_LOG_ERROR, "Invalid content-type '%.*s'\n",
                        (int)(next - opt), opt);
                return AVERROR(EINVAL);
            }
            av_log(h, AV_LOG_VERBOSE, "Content-type: %.*s\n",
                    (int)(next - opt), opt);
        } else {
            if (!av_strncasecmp(opt, "base64", next - opt)) {
                base64 = 1;
            } else {
                av_log(h, AV_LOG_VERBOSE, "Ignoring option '%.*s'\n",
                        (int)(next - opt), opt);
            }
        }
        opt = next + 1;
    }

    data++;
    in_size = strlen(data);

    //将buffer地址放入到字符串里面
    if(atol  (data))
    {
        sbuffer = (struct to_buffer*)atol(data);
        data = sbuffer->ptr;
        in_size = sbuffer->in_len;
        dc->out_size = sbuffer->out_len;
        av_log(NULL, AV_LOG_DEBUG, "sbuffer: %p ptr: %p\n", sbuffer, sbuffer->ptr);
#if 0
        data=(void*)atol  (data);
        next = av_x_if_null(memchr(opt, ',', data), data);
        in_size = atoi  (next+1);
#endif
    }
    if(flags & AVIO_FLAG_READ)
    {
        if (base64) {
            size_t out_size = 3 * (in_size / 4) + 1;

            if (out_size > INT_MAX || !(ddata = av_malloc(out_size)))
                return AVERROR(ENOMEM);
            if ((ret = av_base64_decode(ddata, data, out_size)) < 0) {
                av_free(ddata);
                av_log(h, AV_LOG_ERROR, "Invalid base64 in URI\n");
                return ret;
            }
            dc->data = dc->tofree = ddata;
            dc->size = ret;
        } else {
            dc->data = data;
            dc->size = in_size;
        }
        dc->filesize=dc->size;
        if(dc->out_size) *dc->out_size=dc->filesize;
    }
    else if(flags & AVIO_FLAG_WRITE)
    {
        dc->size = in_size;
        dc->data = data;
    }
    av_log(NULL, AV_LOG_DEBUG, "buffer_open flags: %d sbuffer: %p dc->data: %p dc->size: %zu dc->filesize: %zu uri: %.100s h: %p dc: %p data: %.100s\n", 
            flags,sbuffer, dc->data, dc->size, dc->filesize,  uri, h, dc, dc->data);
    return 0;
}

static av_cold int buffer_close(URLContext *h)
{
    BufferContext *dc = h->priv_data;
    av_log(NULL, AV_LOG_DEBUG, "buffer_close h: %p dc: %p\n", h, dc);
#if 0
    if(dc->flags & AVIO_FLAG_WRITE)
    {
        av_freep(&dc->data);
    }
#endif
    av_freep(&dc->tofree);
    return 0;
}

static int buffer_read(URLContext *h, unsigned char *buf, int size)
{
    BufferContext *dc = h->priv_data;
    av_log(NULL, AV_LOG_DEBUG, "buffer_read size: %d h: %p dc: %p\n", size, h, dc);

    if (dc->pos >= dc->size)
        return AVERROR_EOF;
    size = FFMIN(size, dc->size - dc->pos);
    memcpy(buf, dc->data + dc->pos, size);
    dc->pos += size;
    return size;
}
static int buffer_write(URLContext *h, const unsigned char *buf, int size)
{
    BufferContext *dc = h->priv_data;
    size = FFMIN(size, dc->size - dc->pos);
    //size = FFMIN(size, c->blocksize);
    memcpy(dc->data + dc->pos, buf, size);
    dc->pos += size;
    if(dc->filesize<dc->pos)
    {
        dc->filesize=dc->pos;
        if(dc->out_size) *dc->out_size=dc->filesize;
    }
    av_log(NULL, AV_LOG_DEBUG, "buffer_write size: %d h: %p dc: %p\n", size, h, dc);
    return size;
}
static int64_t buffer_seek(URLContext *h, int64_t pos, int whence)
{
    BufferContext *dc = h->priv_data;
    int64_t ret;
    if (whence == AVSEEK_SIZE) {
        ret=dc->filesize;//10*1024*1024;
        av_log(NULL, AV_LOG_DEBUG, "buffer_seek0 ret: %ld pos: %ld whence: %d h: %p dc: %p\n",ret, pos, whence, h, dc);
        return ret;
    }
    ret = FFMIN(pos, dc->size);
    dc->pos = ret;
    av_log(NULL, AV_LOG_DEBUG, "buffer_seek1 ret: %ld pos: %ld whence: %d h: %p dc: %p\n",ret, pos, whence, h, dc);
    return ret;
}

URLProtocol ff_buffer_protocol = {
    .name           = "buffer",
    .url_open       = buffer_open,
    .url_read       = buffer_read,
    .url_write      = buffer_write,
    .url_seek       = buffer_seek,
    .url_close      = buffer_close,
    .priv_data_size = sizeof(BufferContext),
};
