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
EXECUTABLE_NAME_SPU     = SPU.exe
EXECUTABLE_NAME_ASM     = ASM.exe

BUILDDIR      = Build

READFILEDIR   = ReadFile
SOURCES_READ   =  $(wildcard $(READFILEDIR)/*.cpp)
OBJECTS_READ   =  $(patsubst %.cpp, %.o, $(SOURCES_READ))

SPUDIR        = SPU
STACKDIR      = Stack
SOURCES_SPU   = $(wildcard $(SPUDIR)/*.cpp) $(SPUDIR)/$(STACKDIR)/stack.cpp 
OBJECTS_SPU   = $(patsubst %.cpp, %.o, $(SOURCES_SPU))
CXXFLAGSH_SPU = -I$(SPUDIR)/$(STACKDIR) -DSHOW_DUMP

ASMDIR        = ASM
SOURCES_ASM   = $(wildcard $(ASMDIR)/*.cpp)
OBJECTS_ASM   = $(patsubst %.cpp, %.o, $(SOURCES_ASM))
CXXFLAGSH_ASM = -I$(READFILEDIR)/

.PHONY = all clean start start_out_to_file

all: make_folder $(OBJECTS_READ) $(EXECUTABLE_NAME_ASM) $(EXECUTABLE_NAME_SPU)
	@echo COMPILATION WAS FINISHED

$(OBJECTS_READ): %.o: %.cpp
	@$(CC) $(CFLAGS) -c $^ -o ./$(BUILDDIR)/$@

#____________________________SPU________________________________
$(EXECUTABLE_NAME_SPU): $(OBJECTS_SPU)
	@$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_SPU)) -o ./$(BUILDDIR)/$(EXECUTABLE_NAME_SPU)

$(OBJECTS_SPU): %.o: %.cpp
	@$(CC) $(CFLAGS) $(CXXFLAGSH_SPU) -c $^ -o ./$(BUILDDIR)/$@

#____________________________ASM________________________________
$(EXECUTABLE_NAME_ASM): $(OBJECTS_ASM)
	@$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_ASM)) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_READ))  -o ./$(BUILDDIR)/$(EXECUTABLE_NAME_ASM)

$(OBJECTS_ASM): %.o: %.cpp
	@$(CC) $(CFLAGS) $(CXXFLAGSH_ASM) -c $^ -o ./$(BUILDDIR)/$@

make_folder:
	@mkdir -p $(BUILDDIR)/$(SPUDIR)/$(STACKDIR)/
	@mkdir -p $(BUILDDIR)/$(ASMDIR)/
	@mkdir -p $(BUILDDIR)/$(READFILEDIR)/

start_spu:
	@./$(BUILDDIR)/$(EXECUTABLE_NAME_SPU)

start_asm:
	@./$(BUILDDIR)/$(EXECUTABLE_NAME_ASM)

clean:
	@rm -rf ./$(BUILDDIR)/
	@rm -f dump.txt