#ifndef LIST_LOG_H
#define LIST_LOG_H


#include <stdio.h>

#include "fast_list_func.h"


#define LIST_NAME(list)           #list

#define FAST_LIST_DUMP(list)      FastListDump (list, __FILE__, __func__, __LINE__, LIST_NAME (list))

#define LOG_PRINT(LOG_FILE, ...)  do {                                \
                                      fprintf(LOG_FILE, __VA_ARGS__); \
                                      fflush (LOG_FILE);              \
                                                                      \
                                  } while (0)

#define LIST_EL_FORMAT            "%d"

#define LIST_DOT_FILE_NAME        "list_dot_file.dot"

const int MAX_IMAGE_NAME_LENGTH = 32;
const int MAX_COMMAND_LENGTH    = 128;

FILE *CreateLogFile (const char *log_file_name);

void CloseLogFile (void);

ListStatus ListImagesFolderCreate (void);

ListStatus ListImageCreate (void);

const char *ImageNameCreate (void);

const char *CommandToCreateImageCreate (const char *image_name); 

ListStatus FastListDump (const FastList *list_for_dump, const char *file_called,
                              const char *func_called,       const int line_called,
                              const char *list_name);

ListStatus PrintFastListElem (const FastList *list_for_print);

ListStatus LogPrintListError (const char *error_text);

//---------------------------------- Graph dump -------------------------------------------------------------

ListStatus FastListGraphDump (const FastList *list_for_graph_dump);

ListStatus FastListDotFileBegin (FILE *dot_file);

ListStatus FastListDotFileEnd (FILE *dot_file);

ListStatus FastListDotFileInfo (FILE *dot_file, const FastList *list_for_info);

ListStatus FastListDotFileColorElem (FILE *dot_file_for_color, const FastList *list_for_choose_color,
                                          const size_t index);

ListStatus FastListDotFileOutputElems (FILE *dot_file, const FastList *list_for_output_elems);

ListStatus FastListDotFileDrawArrows (FILE *dot_file_for_arrows,
                                           const FastList *list_for_draw_arrows);

ListStatus FastListDotFileCenterElems (FILE * dot_file_for_center,
                                            const FastList *list_for_center_elems);

ListStatus FastListDotFileColorDummy (FILE *dot_file, const FastList *list_for_output_dummy);


#endif
