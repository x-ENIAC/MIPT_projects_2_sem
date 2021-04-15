CUR_PROG = hashtable
ARGS = 

# ifndef VERBOSE
# .SILENT:
# endif

CPP = g++
LD = g++

WARNINGS = -Wall -Wextra
STANDARD = 
OPTIMIZATION = -O1

CFLAGS = $(WARNINGS) $(STANDARD) $(OPTIMIZATION) -masm=intel

.PHONY : all run debug clean

all: hash_table.o work_with_file.o warnings.o list.o
	$(CPP) $(CFLAGS) -c main.cpp -o main.o
	$(LD) main.o hash_table.o work_with_file.o warnings.o list.o -o $(CUR_PROG)

hash_table.o: hash_table.cpp hash_table.h
	$(CPP) $(CFLAGS) -c hash_table.cpp -o hash_table.o

work_with_file.o: work_with_file.cpp work_with_file.h
	$(CPP) $(CFLAGS) -c work_with_file.cpp -o work_with_file.o

warnings.o: warnings.cpp warnings.h
	$(CPP) $(CFLAGS) -c warnings.cpp -o warnings.o

list.o: list.cpp list.h
	$(CPP) $(CFLAGS) -c list.cpp -o list.o

run: all
	./$(CUR_PROG) $(ARGS)

debug: all
	valgrind --tool=callgrind  ./$(CUR_PROG) $(ARGS)

clean:
	rm *.o
