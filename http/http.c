//
// Created by philip metzger on 22.09.2020.
//

#include "http.h"

#define HTTP_MAX_HEADER (1 << 12) //4096
#define HTTP_MAX_BODY (1 << 16) //65536


static const http_header errors[];