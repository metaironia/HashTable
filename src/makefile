.PHONY: all clean rebuild
CC=g++
CFLAGS = -O3 -mavx -mavx2 -masm=intel
WARNINGS=-Wshadow -Winit-self -Wcast-align -Wundef -Wfloat-equal -Winline -Wredundant-decls \
 	 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default \
	 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy \
	 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
	 -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing \
	 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -DNDEBUG -D_EJUDGE_CLIENT_SIDE 
SOURCES= main.cpp HashTable/hash_table_func.cpp HashTable/hash_table_log.cpp HashTable/hash_func.cpp \
	 DoubleLinkedList/fast_list_func.cpp DoubleLinkedList/fast_list_log.cpp \
         TextPreparer/text_preparer.cpp	 		 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hash_table_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -DNDEBUG $(CFLAGS) $^ -o $@

%.o : %.cpp
	$(CC) -DNDEBUG -c $(CFLAGS) $< -o $@                    

clean:
	 rm -f *.o hash_table_test.exe
	 rm -f DoubleLinkedList/*.o
	 rm -f HashTable/*.o
	 rm -f TextPreparer/*.o

rebuild: clean all



