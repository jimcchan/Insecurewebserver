//
// Created by philip metzger on 22.09.2020.
//


#ifndef INSECUREWEBSERVER_HTTP_H
#define INSECUREWEBSERVER_HTTP_H
#include "../base_config.h"

#include <stdbool.h>
#include <stdint.h>

#include "../utils/blob.h"
#include "mstring.h"


typedef struct http_header http_header;
typedef struct http_body http_body;
typedef struct http_message http_message;

struct http_header {
    mstring **kv; //Decoded Key-Value pairs e.g Encoding UTF-8
    blob src; //pure data source,
};

struct http_body {
    blob content;
};

struct http_message {
    http_header header;
    http_body body;
};

void http_404(http_message *msg);
void http_200(http_message *msg);
void http_403(http_message *msg);

bool http_header_parse(const blob *restrict src, const size_t srcsize, blobstream *restrict out);
bool http_body_create(http_body *body, mstring *utf_content);

bool http_message_create(http_message *msg, const http_header *header, const http_body *body);
#endif//INSECUREWEBSERVER_HTTP_H
