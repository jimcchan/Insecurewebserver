//
// Created by philip metzger on 20.09.2020.
//

/**
 * This file contains a thin wrapper over a sqlite3 database
 *
 */
#ifndef INSECUREWEBSERVER_DATABASE_H
#define INSECUREWEBSERVER_DATABASE_H
#include "../base_config.h"

#include <sqlite3.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../mstring.h"
#include "../utils/blob.h"

typedef struct database database;
typedef struct database_result database_result;
typedef struct database_stats database_stats;
typedef struct database_blob_or_text database_blob_or_text;
typedef struct database_statement database_stmnt;
typedef void (*disposer)(void *);
struct database {
    database_stmnt *stmnts;
    sqlite3 *backing_db;
    sqlite3_stmt *cur_to_insert;
    char flags;
};

struct database_statement {
    mstring stmnt_text;
    sqlite3_stmt *sql;
    bool valid;
    database_stmnt *next;
};

struct database_result {
    uint64_t num_columns;
    sqlite3_value *result;
};

struct database_blob {
    blob blob;
    void (*dispose)(void *);
};

/*
 * Opens database name with the default options
 */
void database_open(database *db, const char *name);
#define DB_MEMORY 1
#define DB_READONLY 2
#define DB_READWRITE 3
#define DB_URI 4
#define DB_NO_MUTEX 5
#define DB_FULLMUTEX 6
#define DB_PRIVATE_CACHE 7
#define DB_SHARED_CACHE 8
#define DB_NO_FOLLOW 9
void database_init(database *db, const char *name, uint8_t flags);
database default_db(void);
void database_destroy(database *db);
bool database_stmt_create(database *db, const char *initial_text);
bool database_prep_stmt(database *db);
bool database_prep_all(database *db);
/***
 * Binds the variable arguments to the current statement, where each format parameter is the position in the statement
 * So if you wanted to fill this query
 * SELECT name,lastname,address from TABLE where name = ? AND lastname = ? AND address LIKE ?;
 * it would be
 * res = database_bind_fmt(db,"%s %s %s","Peter","Pandora","Elm%%");
 * for blobs use
 * database_blob my_db_blob = {
 *  .data = blob_view(&src,0,sizeof(src),
 *  .dispose = png_dispose,
 *  .data_size = png_size(image)
 * };
 * res = database_bind_fmt(db,"%b %d %d,&my_db_blob,1,2,3);
 * @param db, database to bind to
 * @param fmt, the format, %d for integers, %f for floats and doubles, %b for blobs, %s for text
 * @param ..., the variable arguments
 * @return true on success, false on error
 */
bool database_bind_fmt(database *db, const char *fmt, ...);
/**
 * Binds text to the statement, if dispose is not set, free will be used
 */
bool database_bind_text(database *db, int at, const char *text, void (*dispose)(void *));
bool database_bind_mstring(database *db, int at, mstring *str);
bool database_bind_int(database *db, int at, int32_t num);
bool database_bind_real(database *db, int at, double flt);
/**
 * Bind a blob to the statement, if dispose is not set, free will be used
 */
bool database_bind_blob(database *db, int at, const blob *blob, void (*dispose)(void *));
database_result *get_query_result(database *db);
bool database_result_count_rows(database_result *result);
bool database_commit(database *db);
bool database_rollback(database *db);
bool database_query_stats(database *, database_stats *stats);

#endif//INSECUREWEBSERVER_DATABASE_H
