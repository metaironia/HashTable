#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "fast_list_log.h"
#include "fast_list_func.h"


FILE *LOG_FILE = CreateLogFile ("fast_list_log.txt");

FILE *CreateLogFile (const char *log_file_name) {

    assert (log_file_name);

    FILE *log_file_ptr = fopen (log_file_name, "w");

    assert (log_file_ptr);

    atexit (CloseLogFile);
    return log_file_ptr;
}

void CloseLogFile (void) {

    if (LOG_FILE)
        fclose (LOG_FILE);

    LOG_FILE = NULL;
}

enum ListStatus FastListDump (const FastList *list_for_dump, const char *file_called,
                              const char *func_called,       const int line_called,
                              const char *list_name) {

    LOG_PRINT (LOG_FILE, "List[0x%p] \"%s\" from %s(%d) %s() \n", list_for_dump, list_name,
                          file_called, line_called, func_called);

    LOG_PRINT (LOG_FILE, "free = %d\n",      (list_for_dump -> controlItems).free);
    LOG_PRINT (LOG_FILE, "size = %zu\n",     (list_for_dump -> list_size));
    LOG_PRINT (LOG_FILE, "capacity = %zu\n", (list_for_dump -> capacity));

    LOG_PRINT (LOG_FILE, "Elements:\n");

    PrintFastListElem (list_for_dump);

    return LIST_STATUS_OK;
}

enum ListStatus PrintFastListElem (const FastList *list_for_print) {

    assert (list_for_print);

    for (size_t i = 0; i < (list_for_print -> capacity); i++) {

        LOG_PRINT (LOG_FILE, "Element %zu: \n",              i);
        LOG_PRINT (LOG_FILE, "value = " LIST_EL_FORMAT "\n", (list_for_print -> mainItems)[i].value);
        LOG_PRINT (LOG_FILE, "next = %d\n",                  (list_for_print -> mainItems)[i].next);
        LOG_PRINT (LOG_FILE, "prev = %d\n\n",                (list_for_print -> mainItems)[i].prev);
    }

    return LIST_STATUS_OK;
}

enum ListStatus LogPrintListError (const char *error_text) {

    assert (error_text);

    LOG_PRINT (LOG_FILE, "An error %s occurred. \n", error_text);

    return LIST_STATUS_OK;
}

enum ListStatus FastListGraphDump (const FastList *list_for_graph_dump) {

    LIST_VERIFY (list_for_graph_dump);

    FILE *graph_dump_file = fopen ("graph_dump_fast_list.dot", "w");

    if (graph_dump_file == NULL)
        return LIST_STATUS_FAIL;

    FastListDotFileBegin       (graph_dump_file);

    FastListDotFileInfo        (graph_dump_file, list_for_graph_dump);
    FastListDotFileOutputElems (graph_dump_file, list_for_graph_dump);

    FastListDotFileCenterElems (graph_dump_file, list_for_graph_dump);
    FastListDotFileDrawArrows  (graph_dump_file, list_for_graph_dump);

    FastListDotFileEnd         (graph_dump_file);


    fclose (graph_dump_file);
    graph_dump_file = NULL;

    return LIST_STATUS_OK;
}

enum ListStatus FastListDotFileBegin (FILE *dot_file) {

    assert (dot_file);

    fprintf(dot_file, "digraph G{\n"
                      "rankdir = LR;\n"
                      "graph [bgcolor = white];\n");

    return LIST_STATUS_OK;
}

enum ListStatus FastListDotFileEnd (FILE *dot_file) {

    assert (dot_file);

    fprintf(dot_file, "\n}\n");

    return LIST_STATUS_OK;
}

enum ListStatus FastListDotFileInfo (FILE *dot_file, const FastList *list_for_info) {

    assert (dot_file);

    fprintf(dot_file, "info [shape = record, style = filled, fillcolor = \"yellow\","
                      "label = \"FREE: %d | SIZE: %zu | CAPACITY: %zu\","
                      "fontcolor = \"black\", fontsize = 22];\n",
                      (list_for_info -> controlItems).free, list_for_info -> list_size,
                      list_for_info -> capacity);

    return LIST_STATUS_OK;
}

enum ListStatus FastListDotFileColorDummy (FILE *dot_file, const FastList *list_for_output_dummy) {

    assert (dot_file);
    assert (list_for_output_dummy);

    fprintf(dot_file, "fillcolor = gray, color = black, ");

    return LIST_STATUS_OK;
}

enum ListStatus FastListDotFileColorElem (FILE *dot_file_for_color, const FastList *list_for_choose_color,
                                          const size_t index) {

    assert (dot_file_for_color);
    assert (list_for_choose_color);

    if ((list_for_choose_color -> mainItems)[index].value == POISON)
        fprintf (dot_file_for_color, "fillcolor = \"crimson\", color = black,");

    else
        fprintf (dot_file_for_color, "fillcolor = \"lightgreen\", color = darkgreen,");

    return LIST_STATUS_OK;
}


enum ListStatus FastListDotFileOutputElems (FILE *dot_file, const FastList *list_for_output_elems) {

    assert (dot_file);
    assert (list_for_output_elems);

    for (size_t i = 0; i < (list_for_output_elems -> capacity); i++) {

        fprintf (dot_file, "%zu [shape=Mrecord, style=filled, ", i);

        if (i != DUMMY_ELEM_POS)
            FastListDotFileColorElem (dot_file, list_for_output_elems, i);

        else
            FastListDotFileColorDummy (dot_file, list_for_output_elems);

        fprintf (dot_file, " label=\" ");

        if ((list_for_output_elems -> mainItems)[i].value == POISON) {

            fprintf(dot_file, "index: %zu | value: POISON| next: %d| prev: %d\" ];\n",
                              i,
                              (list_for_output_elems -> mainItems)[i].next,
                              (list_for_output_elems -> mainItems)[i].prev);
        }

        else {

            fprintf(dot_file, "index: %zu | value: " LIST_EL_FORMAT "| next: %d| prev: %d\" ];\n",
                              i,
                              (list_for_output_elems -> mainItems)[i].value,
                              (list_for_output_elems -> mainItems)[i].next,
                              (list_for_output_elems -> mainItems)[i].prev);
        }
    }

    return LIST_STATUS_OK;
}

enum ListStatus FastListDotFileDrawArrows (FILE *dot_file_for_arrows,
                                           const FastList *list_for_draw_arrows) {

    assert (dot_file_for_arrows);
    assert (list_for_draw_arrows);

    for (size_t i = 1; i < (list_for_draw_arrows -> capacity); i++) {

        if ((list_for_draw_arrows -> mainItems)[i].next  == -1 &&
            (list_for_draw_arrows -> mainItems)[i].value == POISON)

            fprintf (dot_file_for_arrows, "%zu -> %d [weight = 0, color = \"red\"];\n",
                                          i, (list_for_draw_arrows -> mainItems)[i].prev);

        else {

            fprintf (dot_file_for_arrows, "%zu -> %d [weight = 0, color = \"blue\"];\n",
                                          i, (list_for_draw_arrows -> mainItems)[i].next);

            fprintf (dot_file_for_arrows, "%zu -> %d [weight = 0, color = \"green\"];\n",
                                          i, (list_for_draw_arrows -> mainItems)[i].prev);
        }
    }

    return LIST_STATUS_OK;
}

enum ListStatus FastListDotFileCenterElems (FILE *dot_file_for_center,
                                            const FastList *list_for_center_elems) {

    assert (dot_file_for_center);
    assert (list_for_center_elems);

    fprintf (dot_file_for_center, "info -> 0 [color = \"white\", style = invis];\n");

    for (size_t i = 0; i < (list_for_center_elems -> capacity) - 1; i++) {

        fprintf (dot_file_for_center, "%zu -> %zu [color = \"white\", style = invis];\n",
                                      i, i + 1);
    }

    return LIST_STATUS_OK;
}
