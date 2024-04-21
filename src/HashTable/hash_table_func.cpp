#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../TextPreparer/text_preparer.h"

#include "hash_table_func.h"
#include "hash_table_dsl.h"

#include "hash_table_log.h"

HashTableFuncStatus HashTableCtor (HashTable *hash_table) {

    assert (hash_table);

    HASH_TABLE_SIZE_ = DEFAULT_HASH_TABLE_CAPACITY;

    if (HashTableDataCtor (hash_table) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDataCtor (HashTable *hash_table) {
    
    assert (hash_table);
    
    HASH_TABLE_CELL_ = (FastList *) calloc (HASH_TABLE_SIZE_, sizeof (FastList));
    assert (HASH_TABLE_CELL_);

    for (size_t i = 0; i < ((size_t) HASH_TABLE_SIZE_); i++)         
        FastListCtor (HASH_TABLE_CELL_ + i, DEFAULT_LIST_CAPACITY);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDataDtor (HashTable *hash_table) {

    assert (hash_table);
    assert (HASH_TABLE_CELL_);

    for (size_t i = 0; i < ((size_t) HASH_TABLE_SIZE_); i++)
        FastListDtor (HASH_TABLE_CELL_ + i);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableDtor (HashTable *hash_table) {

    assert (hash_table);

    HashTableDataDtor (hash_table);
    HASH_TABLE_SIZE_ = 0;

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableInsert (HashTable *hash_table, const HashTableElem_t data, const int64_t key) {

    assert (hash_table);

    if (key < 0)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    int64_t   cell_num = key % HASH_TABLE_SIZE_;
    FastList *cell_ptr = HASH_TABLE_CELL_ + cell_num;

    int64_t stub = 0;

    if (FastListFindElem (cell_ptr, data) == LIST_FUNC_STATUS_FAIL)
        FastListAddElemAfter (cell_ptr, DUMMY_ELEM_POS, &stub, data);

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableReadData (const char *input_file_name, HashTable *hash_table, 
                                       uint32_t (*hash_func) (const HashTableElem_t)) {

    assert (input_file_name);
    assert (hash_table);
    assert (hash_func);

    FILE *data_file = NULL;
    
    if (!(data_file = fopen (TextPreparedFileNameGen (input_file_name), "r"))) {

        TextPrepare (input_file_name);

        data_file = fopen (TextPreparedFileNameGen (input_file_name), "r");
    }
    
    char word[MAX_WORD_LENGTH] = {};

    while (!feof (data_file)) {

        int fscanf_status = fscanf (data_file, "%s", word);

        if (fscanf_status != EOF && fscanf_status != 0)
            HashTableInsert (hash_table, strdup (word), hash_func (word));
    }

    return HASH_TABLE_FUNC_STATUS_OK;
}