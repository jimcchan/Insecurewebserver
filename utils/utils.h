//
// Created by philip metzger on 24.09.2020.
//

#ifndef INSECUREWEBSERVER_UTILS_H
#define INSECUREWEBSERVER_UTILS_H
#include "../base_config.h"

#include <stdbool.h>

#define SWAP(t, a, b)     \
    {                     \
        (t) _temp_ = (a); \
        (a) = (b);        \
        (b) = _temp_;     \
    }
typedef enum ByteOrder ByteOrder;
enum ByteOrder {
    kLITTLE_ENDIAN = 0,
    kBIG_ENDIAN = 0xFF
};

ByteOrder system_byteorder(void);
const size_t system_pagesize(void);
bool system_windows(void);

#endif//INSECUREWEBSERVER_UTILS_H
