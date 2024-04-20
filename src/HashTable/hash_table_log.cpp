#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

#include "../DoubleLinkedList/fast_list_log.h"

#include "hash_table_dsl.h"
#include "hash_table_func.h"
#include "hash_table_log.h"

HashTableFuncStatus HashTableGraphDump (const HashTable *hash_table) {

    assert (hash_table);

    FILE *hash_table_dot = fopen (HASH_TABLE_DOT_FILE_NAME, "w"); 

    HashTableDotFileBegin (hash_table_dot);
    HashTableDotFilePrint (hash_table_dot, hash_table);
    HashTableDotFileEnd   (hash_table_dot);

    fclose (hash_table_dot);
    hash_table_dot = NULL;

    ImagesFolderCreate();
    ImageCreate ("hash_table");

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDotFileBegin (FILE *hash_table_dot) {

    assert (hash_table_dot);

    fprintf (hash_table_dot, "digraph HashTable {\n"
                             "rankdir = LR;\n");

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDotFileEnd (FILE *hash_table_dot) {

    assert (hash_table_dot);

    fprintf (hash_table_dot, "}");

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDotFilePrint (FILE *hash_table_dot, const HashTable *hash_table) {

    assert (hash_table_dot);
    assert (hash_table);

    for (int64_t i = HASH_TABLE_SIZE_ - 1; i >= 0; i--) {

        fprintf              (hash_table_dot, "subgraph cluster_%zu {\n"
                                              "label = \"cell #%zu\"\n", i, i);                  
        FastListDotFilePrint (hash_table_dot, HASH_TABLE_CELL_ + i);
        fprintf              (hash_table_dot, "};\n");
    } 

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableLoadDump (const HashTable *hash_table) {

    assert (hash_table);

    FILE *load_dump = fopen (HashTableLoadFileNameGen(), "w");

    for (size_t i = 0; i < (size_t) HASH_TABLE_SIZE_; i++)
        fprintf (load_dump, "%zu ; %" PRId64 "\n", i, (HASH_TABLE_CELL_ + i) -> list_size);

    fclose (load_dump);
    load_dump = NULL;

    return HASH_TABLE_FUNC_STATUS_OK;
}

const char *HashTableLoadFileNameGen (void) {

    static char load_file_name[MAX_FILE_NAME_LENGTH] = {};
    static int  file_number                          = 0;

    snprintf (load_file_name, MAX_FILE_NAME_LENGTH, "load_dump_%d.txt", file_number);

    file_number++;

    return load_file_name;
} 