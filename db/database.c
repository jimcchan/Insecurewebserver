//
// Created by philip metzger on 20.09.2020.
//

#include "database.h"
#include "../base_config.h"


#include <stdlib.h>

static const char memdb[] = ":memory";

database default_db(void) {
    database db;
    database_open(&db,"InsecureWebServer");
    return db;
}

void database_open(database *db, const char *filename) {
    if (!db || !filename)
        return;
    if (filename[0] == '\0')
        return;
    if (sqlite3_open_v2(filename, &db->backing_db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        server_log_error("Error opening database %s", sqlite3_errmsg(db->backing_db));
        return;
    }
    server_log_message("Opened database %s successfully", filename);
    mstring_init(&db->stmt_text);
}
void database_init(database *db, const char *filename, uint8_t flags) {
    int rc = 0;
    if (!db || !filename || flags == 0)
        return;
    if (filename[0] == '\0')
        return;
    if (flags & (DB_READWRITE | DB_MEMORY)) {
        if (sqlite3_open_v2(filename, &db->backing_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_MEMORY, NULL) != SQLITE_OK) {
            servre_log_error(&global_log, "Failed to open database: %s", sqlite3_errmsg(db->backing_db));
            return;
        }
    }
    mstring_init(db->stmt_text);
}


bool database_bind_text(database *db, int at, const char *text, void (*dispose)(void *)) {
    if (!db)
        return false;
    return sqlite3_bind_text(db->to_insert, at, text, -1, dispose ? dispose : free) == SQLITE_OK;
}
bool database_bind_mstring(database *db,int at,mstring *str) {
    if (!db || !str)
        return false;
    switch (mstring_get_encoding(str)) {
        case MSTRING_UTF32: {
            char *utf8_str;
            utf8_str = malloc(4 * mstring_length(str));
            if (utf8_str)
                return false;
            utf8_str = mstring_to_utf8(mstring_text(str));
            if (!str)
                return false;
            return sqlite3_bind_text(db->to_insert, at, utf_str, -1, free) == SQLITE_OK;
        }
        case MSTRING_UTF16:
            return sqlite3_bind_text16(db->to_insert,at,mstring_text(str),-1,SQLITE_STATIC) == SQLITE_OK;
        case MSTRING_UTF8:
            /* fallthrough */
        default:
            return sqlite3_bind_text(db->to_insert,at,mstring_text(str),-1,SQLITE_STATIC) == SQLITE_OK;
    }
    assert_unreachable();
}
bool database_bind_int(database *db, int at, int32_t num) {
    if (!db)
        return false;
    return sqlite3_bind_int(db->to_insert, at, num) == SQLITE_OK;
}
bool database_bind_real(database *db, int at, double flt) {
    if (!db)
        return false;
    return sqlite3_bind_double(db->to_insert, at, flt) == SQLITE_OK;
}
bool database_bind_blob(database *db, int at, void *blob, size_t blobsize, void (*dispose)(void *)) {
    if (!db || !blob)
        return false;
    return sqlite3_bind_blob(db->to_insert, at, blob, blobsize, dispose ? dispose : free) == SQLITE_OK;
}
