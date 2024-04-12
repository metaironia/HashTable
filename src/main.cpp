#include <stdio.h>
#include <assert.h>

#include "HashTable/hash_table_func.h"

int main (const int argc, const char *argv[]) {

    HashTable hash_table = {};
    HashTableCtor (&hash_table);

    HashTableDtor (&hash_table);

    /*
    int list_capacity = 5;
    
    FastList lst = {};
    FastListCtor (&lst, list_capacity);

    FAST_LIST_DUMP (&lst);

    size_t test_pos = DUMMY_ELEM_POS;

    FastListAddElemAfter (&lst, test_pos, &test_pos, 10);
    FastListAddElemAfter (&lst, test_pos, &test_pos, 13);
    FastListAddElemAfter (&lst, test_pos, &test_pos, 20);
    FastListAddElemAfter (&lst, test_pos, &test_pos, 167);

    FastListRemoveElem (&lst, 3);

    FastListGraphDump (&lst);

    FastListDtor (&lst); */

//    ClassicList lst = {};
//
//    ClassicListCtor (&lst, list_capacity);
//
//    CLASSIC_LIST_DUMP (&lst);
//
//    ClassicListMainItems *test_pos = &(lst.mainItems)[DUMMY_ELEM_POS];
//
//    ClassicListAddElemAfter (&lst, test_pos, &test_pos, 10);
//
//    ClassicListGraphDump (&lst);

    return 0;
}
