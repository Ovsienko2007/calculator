CC = g++
CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef \
   -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations \
   -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ \
   -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion\
   -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2\
   -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers\
   -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo\
   -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits\
   -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
EXECUTABLE_NAME = main.exe

BUILDDIR = Build
STACKDIR = Stack

SOURCES = $(wildcard *.cpp) $(STACKDIR)/stack.cpp
OBJECTS =$(patsubst %.cpp, %.o, $(SOURCES))

CXXFLAGSH =  -I $(STACKDIR) -DSHOW_DUMP -DCONSOLE_OUTPUT

all: $(EXECUTABLE_NAME)

.PHONY = all clean start start_out_to_file

$(EXECUTABLE_NAME): make_folder  $(OBJECTS)
	$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS)) -o ./$(BUILDDIR)/$(EXECUTABLE_NAME)


$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGSH) -c $^ -o ./$(BUILDDIR)/$@

make_folder:
	mkdir -p $(BUILDDIR)/$(STACKDIR)/

start:
	./$(DIR_BUILD)/$(EXECUTABLE_NAME)

clean:
	rm -rf ./$(BUILDDIR)/
	rm -f dump.txt