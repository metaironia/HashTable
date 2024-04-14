#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "fast_list_func.h"
#include "fast_list_log.h"


ListStatus FastListCtor (FastList *created_list, const size_t list_capacity) {

    assert (created_list);

    if (list_capacity <= 0)
        return LIST_STATUS_FAIL;

    created_list -> capacity  = list_capacity + 1;
    created_list -> list_size = 0;
    
    FastListStructArrayCtor (created_list);

    ON_DEBUG (FAST_LIST_DUMP (created_list));

    FastListCreateDummyNode (created_list);
    FillFastList            (created_list, DUMMY_ELEM_POS);

    LIST_VERIFY (created_list);

    return LIST_STATUS_OK;
}

ListStatus FastListDtor (FastList *list_for_destruct) {

    LIST_VERIFY (list_for_destruct);

    FastListStructArrayDtor (list_for_destruct);
    
    memset (list_for_destruct, 0, sizeof (FastList));

    return LIST_STATUS_OK;
}

ListStatus FastListStructArrayCtor (const FastList *const list_for_create_arrs) {

    assert (list_for_create_arrs);

    FastListMainItems *main_items_ptr = (FastListMainItems *) calloc (list_for_create_arrs -> capacity,
                                                                      sizeof (FastListMainItems));

    assert (main_items_ptr);

    return LIST_STATUS_OK;
}

ListStatus FastListStructArrayDtor (FastList *list_for_arr_destruct) {

    LIST_VERIFY (list_for_arr_destruct);

    FastListStructArrayClear (list_for_arr_destruct);

    free (list_for_arr_destruct -> mainItems);
    list_for_arr_destruct -> mainItems = NULL;

    return LIST_STATUS_OK;
}

ListStatus FillFastList (FastList *const list_for_fill, size_t start_pos) {

    assert (list_for_fill);

    for (size_t i = list_for_fill -> capacity - 1; i > start_pos; i--)
        FastListFreeElem (list_for_fill, i);

    ON_DEBUG (FAST_LIST_DUMP (list_for_fill));

    return LIST_STATUS_OK;
}

ListStatus FastListConnectNeighbourElems (FastList *const list_for_connect_elems,
                                          const size_t first_elem_pos,
                                          const size_t second_elem_pos) {

    assert (list_for_connect_elems);

    (list_for_connect_elems -> mainItems)[first_elem_pos].next  = second_elem_pos;
    (list_for_connect_elems -> mainItems)[second_elem_pos].prev = first_elem_pos;

    ON_DEBUG (FAST_LIST_DUMP (list_for_connect_elems));

    return LIST_STATUS_OK;
}

ListStatus FastListCreateDummyNode (FastList *const list_for_create_dummy_node) {

    assert (list_for_create_dummy_node);

    (list_for_create_dummy_node -> mainItems)[DUMMY_ELEM_POS].value = POISON;

    FastListConnectNeighbourElems (list_for_create_dummy_node, DUMMY_ELEM_POS, DUMMY_ELEM_POS);

    ON_DEBUG (FAST_LIST_DUMP (list_for_create_dummy_node));

    return LIST_STATUS_OK;
}

ListStatus FastListStructArrayClear (FastList *const list_for_clear_struct) {

    LIST_VERIFY (list_for_clear_struct);

    size_t size_of_main_items = sizeof (FastListMainItems) * (list_for_clear_struct -> capacity);

    memset (list_for_clear_struct -> mainItems, 0, size_of_main_items);

    ON_DEBUG (FAST_LIST_DUMP (list_for_clear_struct));

    return LIST_STATUS_OK;
}

unsigned int FastListVerify (const FastList *const list_to_verify) {

    unsigned int list_errors = 0;

    if (list_to_verify == NULL) {

        list_errors |= INVALID_LIST_POINTER;

        LogPrintListError ("INVALID_LIST_POINTER");
    }

    if ((list_to_verify -> list_size) > (list_to_verify -> capacity)) {

        list_errors |= INVALID_LIST_SIZE;

        LogPrintListError ("INVALID_LIST_SIZE");
    }

    if ((list_to_verify -> mainItems)[DUMMY_ELEM_POS].value != POISON) {

        list_errors |= DAMAGED_LIST_DUMMY_NODE;

        LogPrintListError ("DAMAGED_LIST_DUMMY_NODE");
    }

    for (int64_t i = 0, first_elem_next_index = 0, second_elem_prev_index = 0;
         i < (list_to_verify -> capacity); i++) {

        first_elem_next_index  = (list_to_verify -> mainItems)[i].next;

        // if next is -1 and value is POISON then cell is free

        if (first_elem_next_index == -1 && (list_to_verify -> mainItems)[i].value == POISON)
            continue;

        if (first_elem_next_index >= (list_to_verify -> capacity) || first_elem_next_index < 0) {

            list_errors |= INVALID_LIST_ELEM_NEXT;

            LogPrintListError ("INVALID_LIST_ELEM_NEXT");
        }

        second_elem_prev_index = (list_to_verify -> mainItems)[first_elem_next_index].prev;

        if (i != second_elem_prev_index) {

            list_errors |= DAMAGED_CONNECTION_LIST;

            LogPrintListError ("DAMAGED_CONNECTION_LIST");

            break;
        }
    }

    size_t free_elem_index = ((list_to_verify -> controlItems).free);

    if ((list_to_verify -> mainItems)[free_elem_index].value != POISON) {

        list_errors |= DAMAGED_LIST_FREE_ELEM;

        LogPrintListError ("DAMAGED_LIST_FREE_ELEM");
    }

    return list_errors;
}

ListStatus FastListAddElemAfter (FastList *const list_for_add_elem, const size_t index_in_list,
                                      size_t *inserted_index,            const FastListElem_t add_value) {

    assert (inserted_index);

    LIST_VERIFY (list_for_add_elem);

    if ((list_for_add_elem -> controlItems).free == DUMMY_ELEM_POS)
        if (FastListIncreaseCapacity (list_for_add_elem) == LIST_STATUS_FAIL)
            return LIST_STATUS_FAIL;

    const size_t free_elem_index    = (list_for_add_elem -> controlItems).free;
    const size_t index_in_list_next = (list_for_add_elem -> mainItems)[index_in_list].next;

    (list_for_add_elem -> mainItems)[free_elem_index].value = add_value;

    (list_for_add_elem -> controlItems).free = (list_for_add_elem -> mainItems)[free_elem_index].prev;

    FastListConnectNeighbourElems (list_for_add_elem, index_in_list,   free_elem_index);
    FastListConnectNeighbourElems (list_for_add_elem, free_elem_index, index_in_list_next);

    *inserted_index = free_elem_index;

    (list_for_add_elem -> list_size) += 1;

    ON_DEBUG (FAST_LIST_DUMP (list_for_add_elem));

    return LIST_STATUS_OK;
}

ListStatus FastListRemoveElem (FastList *const list_for_remove_elem, const size_t index_in_list_remove) {

    LIST_VERIFY (list_for_remove_elem);

    size_t prev_of_index_remove = (list_for_remove_elem -> mainItems)[index_in_list_remove].prev;
    size_t next_of_index_remove = (list_for_remove_elem -> mainItems)[index_in_list_remove].next;

    if (FastListFreeElem (list_for_remove_elem, index_in_list_remove) == LIST_STATUS_FAIL)
        return LIST_STATUS_FAIL;

    FastListConnectNeighbourElems (list_for_remove_elem, prev_of_index_remove, next_of_index_remove);

    (list_for_remove_elem -> list_size) -= 1;

    ON_DEBUG (FAST_LIST_DUMP (list_for_remove_elem));

    return LIST_STATUS_OK;
}

ListStatus FastListFreeElem (FastList *const list_for_free_elem, const size_t index_in_list_free) {

    assert (list_for_free_elem);

    if (index_in_list_free == DUMMY_ELEM_POS)
        return LIST_STATUS_FAIL;

    (list_for_free_elem -> mainItems)[index_in_list_free].value = POISON;
    (list_for_free_elem -> mainItems)[index_in_list_free].next  = -1;
    (list_for_free_elem -> mainItems)[index_in_list_free].prev  = (list_for_free_elem -> controlItems).free;

    (list_for_free_elem -> controlItems).free = index_in_list_free;

    ON_DEBUG (FAST_LIST_DUMP (list_for_free_elem));

    return LIST_STATUS_OK;
}

ListStatus FastListGetElem (const FastList *const list_for_get_elem, const size_t index_in_list,
                                 FastListElem_t *ret_value) {

    assert (ret_value);

    LIST_VERIFY (list_for_get_elem);

    *ret_value = (list_for_get_elem -> mainItems)[index_in_list].value;

    ON_DEBUG (FAST_LIST_DUMP (list_for_get_elem));

    return LIST_STATUS_OK;
}

ListStatus FastListIncreaseCapacity (FastList *const list_for_increase_cap) {

    LIST_VERIFY (list_for_increase_cap);

    size_t prev_capacity = (list_for_increase_cap -> capacity);
    size_t new_capacity  = prev_capacity * 2;

    size_t size_of_all_elems_in_bytes = sizeof (FastListMainItems) * new_capacity;
    size_t size_of_new_elems_in_bytes = sizeof (FastListMainItems) * (new_capacity - prev_capacity);

    (list_for_increase_cap -> capacity) = new_capacity;

    FastListMainItems **ptr_to_fast_list_elems = &(list_for_increase_cap -> mainItems);

    *ptr_to_fast_list_elems = (FastListMainItems *) realloc (*ptr_to_fast_list_elems,
                                                             size_of_all_elems_in_bytes);

    if (ptr_to_fast_list_elems == NULL)
        return LIST_STATUS_FAIL;

    memset (*ptr_to_fast_list_elems + prev_capacity, 0, size_of_new_elems_in_bytes);

    for (size_t i = new_capacity - 1; i >= prev_capacity; i--)
        FastListFreeElem (list_for_increase_cap, i);

    ON_DEBUG (FAST_LIST_DUMP (list_for_increase_cap));

    return LIST_STATUS_OK;
}
