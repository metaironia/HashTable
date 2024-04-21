#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../TextPreparer/text_preparer.h"

#include "hash_func.h"
#include "hash_table_func.h"
#include "hash_table_dsl.h"
#include "hash_table_log.h"

HashTableFuncStatus HashTableCtor (HashTable *hash_table, int64_t hash_table_capacity) {

    assert (hash_table);

    HASH_TABLE_SIZE_ = hash_table_capacity;

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

        if (TextPrepare (input_file_name) == PREPARING_FUNC_STATUS_FAIL)
            return HASH_TABLE_FUNC_STATUS_FAIL;

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

HashTableFuncStatus HashTableClear (HashTable *hash_table) {

    assert (hash_table);

    for (size_t i = 0; i < (size_t) HASH_TABLE_SIZE_; i++) {
    
        FastList *curr_cell = HASH_TABLE_CELL_ + i;

        (curr_cell -> controlItems).free = 0;
        curr_cell  -> list_size          = 0;

        FillFastList                  (curr_cell, DUMMY_ELEM_POS);
        FastListConnectNeighbourElems (curr_cell, DUMMY_ELEM_POS, DUMMY_ELEM_POS);
    }

    return HASH_TABLE_FUNC_STATUS_OK;
}

HashTableFuncStatus HashTableTestHashes (const char *input_file_name) {

    assert (input_file_name);

    HashTable hash_table = {};
    HashTableCtor (&hash_table, DEFAULT_HASH_TABLE_CAPACITY);

    if (HashTableReadData (input_file_name, &hash_table, FirstHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);
    HashTableClear    (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, SecondHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);
    HashTableClear    (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, ThirdHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);
    HashTableClear    (&hash_table);

    HashTable hash_table_cap_101 = {};
    HashTableCtor (&hash_table_cap_101, 101);

    if (HashTableReadData (input_file_name, &hash_table_cap_101, FourthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table_cap_101);
    
    HashTableDtor     (&hash_table_cap_101);

    if (HashTableReadData (input_file_name, &hash_table, FourthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);
    HashTableClear    (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, FifthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);
    HashTableClear    (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, SixthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);
    HashTableClear    (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, SeventhHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);
    HashTableClear    (&hash_table);

    if (HashTableReadData (input_file_name, &hash_table, EighthHash) == HASH_TABLE_FUNC_STATUS_FAIL)
        return HASH_TABLE_FUNC_STATUS_FAIL;

    HashTableLoadDump (&hash_table);

    HashTableDtor (&hash_table);

    return HASH_TABLE_FUNC_STATUS_OK;
}