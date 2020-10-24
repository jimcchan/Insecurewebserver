//
// Created by philip metzger on 05.10.2020.
//

#ifndef INSECUREWEBSERVER_BLOB_H
#define INSECUREWEBSERVER_BLOB_H
#include "../base_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

//Idea from: https://github.com/awslabs/s2n
//Blob based on https://github.com/awslabs/s2n/blob/main/utils/s2n_blob.h
//Blobstream based on https://github.com/awslabs/s2n/blob/main/stuffer/s2n_stuffer.h
typedef struct blob blob;
typedef struct blobstream blobstream;

// A blob is the most basic input/output type it's just a bunch of bytes
//
struct blob {
    uint8_t *buf;
    uint64_t size;
    uint64_t allocated;
};
// A blobstream is a reader/writer over some bytes
// where reader < writter
struct blobstream {
    blob base;
    uint32_t reader_pos;
    uint32_t writer_pos;
};

/** Blob functions */
void blob_init(blob *b, uint8_t *buf, size_t size);
blob blob_default(void);
void blob_reset(blob *b);
void blob_zero(blob *b);
bool blob_append(blob *b, const uint8_t *src, size_t size);
bool blob_append_mstring(blob *b, const mstring *mstring);
void blob_view(blob *view, const blob *src, uint32_t offset, uint32_t size);

/**Stream functions */

void blobstream_init(blobstream *stream);
blobstream *blobstream_new(void);
size_t blobstream_write(blobstream *stream, blob *buf, size_t size);
size_t blobstream_read(blobstream *stream, const blob *buf, size_t size);
size_t blobstream_write_full(blobstream *stream, blob *buf, size_t maxsize);
size_t blobstream_read_full(blobstream *stream, const blob *buf, size_t max);
blobstream *blob_stream_from_file(FILE *file);
bool blobstream_skip_bytes(blobstream *stream, uint32_t bytes);
/**
 * Have we reached End-of-content aka EOF
 */
bool blobstream_eoc(const blobstream *stream);

#endif//INSECUREWEBSERVER_BLOB_H
