#ifndef LIST_FUNC_H
#define LIST_FUNC_H


#include <stdint.h>

#define LIST_VERIFY(list)   do {                                   \
                                if (FastListVerify (list) != 0) {  \
                                                                   \
                                    FAST_LIST_DUMP (list);         \
                                    return LIST_STATUS_FAIL;       \
                                }                                  \
                            } while (0)

#define DUMMY_ELEM_POS      0

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

    int prev;
    int next;
};

struct FastListControlItems {

    int free;
};

struct FastList {

    FastListControlItems controlItems;

    FastListMainItems *mainItems;

    size_t capacity;
    size_t list_size;
};

enum ListStatus {

    LIST_STATUS_OK,
    LIST_STATUS_FAIL
};

enum ListErrors {

    INVALID_LIST_POINTER,
    INVALID_LIST_SIZE,
    DAMAGED_LIST_DUMMY_NODE,
    INVALID_LIST_ELEM_NEXT,
    DAMAGED_CONNECTION_LIST,
    DAMAGED_LIST_FREE_ELEM
};

enum ListStatus FastListCtor (FastList *created_list, const size_t list_capacity);

enum ListStatus FastListDtor (FastList *list_for_destruct);

FastListMainItems *FastListStructArrayCtor (const FastList *const list_for_create_arrs);

enum ListStatus FastListStructArrayDtor (FastList *list_for_arrs_destruct);

enum ListStatus FastListStructArrayClear (FastList *const list_for_clear_struct);

enum ListStatus FillFastList (FastList *const list_for_fill, size_t start_pos);

enum ListStatus FastListConnectNeighbourElems (FastList *const list_for_connect_elems,
                                               const size_t first_elem_pos,
                                               const size_t second_elem_pos);

enum ListStatus FastListCreateDummyNode (FastList *const list_for_create_dummy_node);

unsigned int FastListVerify (const FastList *const list_to_verify);

enum ListStatus FastListAddElemAfter (FastList *const list_for_add_elem, const size_t index_in_list,
                                      size_t *inserted_index,            const FastListElem_t add_value);

enum ListStatus FastListRemoveElem (FastList *const list_for_remove_elem, const size_t index_in_list_remove);

enum ListStatus FastListFreeElem (FastList *const list_for_free_elem, const size_t index_in_list_free);

enum ListStatus FastListGetElem (const FastList *const list_for_get_elem, const size_t index_in_list,
                                 FastListElem_t *ret_value);

enum ListStatus FastListIncreaseCapacity (FastList *const list_for_increase_cap);

#endif
