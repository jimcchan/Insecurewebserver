//
// Created by philip metzger on 05.10.2020.
//

#include "blob.h"

#include <gmpxx.h>
#include <stdlib.h>
#include <string.h>

/**Blob section: */
WEBSERVER_INTERNAL bool blob_expand(blob *blob, size_t newsize);
WEBSERVER_INTERNAL bool blob_expand(blob *blob, size_t newsize) {
    assert(blob && newsize > 0);
    void *ptr = realloc(blob->buf,newsize);
    if (!ptr)
        return false;
    blob->buf = ptr;
    return true;
}

void blob_init(blob *b, uint8_t *buf, size_t size) {
    b->allocated = 0;
    b->size = size;
    b->buf = buf;
}
void blob_reset(blob *b) {
    assert(b && b->buf);
    if (b->allocated && b->buf) {
        free(b->buf);
        b->buf = NULL;
    }
    memset(b, 0, sizeof(*b));
}
void blob_view(blob *view, const blob *src, uint32_t offset, uint32_t size) {
    assert(view && src);
    assert(offset >= 0 && size <= src->size);
    assert(offset <= src->allocated && size != src->allocated);
    /* clang-format off */
    *view = (blob){
            .size = size,
            .allocated = 0,
            .buf = &src->buf[offset]
    };
    /* clang-format on */
}

void blob_zero(blob *b) {
    assert(b && b->size);
    memset(b->buf, 0, b->allocated);
}

bool blob_append(blob *b, const uint8_t *src, size_t size) {
    assert(b && src && size > 0);
    if (b->size + size >= b->allocated) {
        b->allocated *= 2;
        if (!blob_expand(b, b->allocated)) {
            b->allocated /= 2;
            return false;
        }
        memcpy(&b->buf[b->size], src, size);
        b->size += size;
        return true;
    }
    assert(b->size + size < b->allocated);
    memcpy(&b->buf[b->size], src, size);
    b->size += size;
    return true;
}


bool blob_append_mstring(blob *b, const mstring *mstring) {
    assert(b && mstring);
    return blob_append(b, mstring_bytes(mstring), mstring_length(mstring));
}


/**Blobstream Section: */

void blobstream_init(blobstream *stream) {

    stream->base = blob_default();
    stream->reader_pos = stream->writer_pos = 0;
}
blobstream *blobstream_new(void) {
    blobstream *stream = malloc(sizeof(*stream));
    if (!stream)
        return NULL;
    stream->reader_pos = stream->writer_pos = 0;
    return stream;
}
size_t blobstream_write(blobstream *stream, blob *buf, size_t size) {
}
size_t blobstream_read(blobstream *stream, const blob *buf, size_t size) {
    if (!stream || !buf)
        return 0;
    if (!blob_append(&stream->base, buf->buf, size))
        return 0;
    return size;
}
size_t blobstream_write_full(blobstream *stream, blob *buf, size_t maxsize) {
    ssize_t size = 0;
    do {
        size = blob_write(&stream->base, buf->buf, maxsize);

    } while (size < blob_size(&stream->base) && size != -1);
    return size;
}
size_t blobstream_read_full(blobstream *stream, const blob *buf, size_t max) {
}


blobstream *blob_stream_from_file(FILE *file) {
    static uint8_t line[1000] = {0};
    blobstream *stream = blobstream_new();
    if (!stream)
        goto alloc_fail;
    size_t fsize = 0;
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    while (fread(&line, sizeof(line), 1, file) != -1) {
        if (!blob_append(&stream->base, line, sizeof(line))) {
            goto append_fail;
        }
    }
    return stream;

append_fail:
    blob_reset(&stream->base);
alloc_fail:
    free(stream);
    return NULL;
}
bool blobstream_skip_bytes(blobstream *stream, uint32_t bytes) {
    assert(stream && bytes > 0 && bytes <= UINT32_MAX - 1);
    stream->reader_pos += bytes;
    return stream->reader_pos == stream->writer_pos || stream->reader_pos == blobstream_eoc(stream) ? false : true;
}

bool blobstream_eoc(const blobstream *stream) {
    return stream && stream->reader_pos == blob_length(&stream->base);
}
