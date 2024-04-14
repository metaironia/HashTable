#ifndef LIST_FUNC_H
#define LIST_FUNC_H


#include <stdint.h>

#define LIST_VERIFY(list)   do {                                   \
                                if (FastListVerify (list) != 0) {  \
                                                                   \
                                    FAST_LIST_DUMP (list);         \
                                    return LIST_FUNC_STATUS_FAIL;  \
                                }                                  \
                            } while (0)


#define IS_ELEM_TYPE_FLOAT  0

#define DEBUG

#if IS_ELEM_TYPE_FLOAT

    #define POISON          NAN
#else

    #define POISON          0x50DEAD
#endif


#ifdef DEBUG

    #define ON_DEBUG(...)   __VA_ARGS__
#else

    #define ON_DEBUG(...)
#endif

typedef int FastListElem_t;

struct FastListMainItems {

    FastListElem_t value;

    int64_t prev;
    int64_t next;
};

struct FastListControlItems {

    int64_t free;
};

struct FastList {

    FastListControlItems controlItems;

    FastListMainItems *mainItems;

    int64_t capacity;
    int64_t list_size;
};

const int64_t DUMMY_ELEM_POS = 0;

enum ListFuncStatus {

    LIST_FUNC_STATUS_OK,
    LIST_FUNC_STATUS_FAIL
};

enum ListErrors {

    INVALID_LIST_POINTER,
    INVALID_LIST_SIZE,
    NEGATIVE_LIST_CAPACITY,
    NEGATIVE_LIST_SIZE,
    DAMAGED_LIST_DUMMY_NODE,
    INVALID_LIST_ELEM_NEXT,
    DAMAGED_CONNECTION_LIST,
    DAMAGED_LIST_FREE_ELEM
};

ListFuncStatus FastListCtor (FastList *created_list, const int64_t list_capacity);

ListFuncStatus FastListDtor (FastList *list_for_destruct);

ListFuncStatus FastListStructArrayCtor (FastList *list_for_create_arrs);

ListFuncStatus FastListStructArrayDtor (FastList *list_for_arrs_destruct);

ListFuncStatus FastListStructArrayClear (FastList *const list_for_clear_struct);

ListFuncStatus FillFastList (FastList *const list_for_fill, const int64_t start_pos);

ListFuncStatus FastListConnectNeighbourElems (FastList *const list_for_connect_elems,
                                              const int64_t first_elem_pos,
                                              const int64_t second_elem_pos);

ListFuncStatus FastListCreateDummyNode (FastList *const list_for_create_dummy_node);

unsigned int FastListVerify (const FastList *const list_to_verify);

ListFuncStatus FastListAddElemAfter (FastList *const list_for_add_elem, const int64_t        index_in_list,
                                     int64_t        *inserted_index,    const FastListElem_t add_value);

ListFuncStatus FastListRemoveElem (FastList *const list_for_remove_elem, const int64_t index_in_list_remove);

ListFuncStatus FastListFreeElem (FastList *const list_for_free_elem, const int64_t index_in_list_free);

ListFuncStatus FastListGetElem (const FastList *const list_for_get_elem, const int64_t index_in_list,
                                      FastListElem_t *ret_value);

ListFuncStatus FastListIncreaseCapacity (FastList *const list_for_increase_cap);

#endif
