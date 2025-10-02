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
EXECUTABLE_NAME_DISASM  = DISASM.exe

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
SOURCES_ASM   = $(ASMDIR)/ASM.cpp
OBJECTS_ASM   = $(patsubst %.cpp, %.o, $(SOURCES_ASM))
CXXFLAGSH_ASM = -I$(READFILEDIR)/

DISASMDIR        = ASM
SOURCES_DISASM   = $(DISASMDIR)/DISASM.cpp
OBJECTS_DISASM   = $(patsubst %.cpp, %.o, $(SOURCES_DISASM))
CXXFLAGSH_DISASM = -I$(READFILEDIR)/

.PHONY = all clean start start_out_to_file

all: make_folder $(OBJECTS_READ) $(EXECUTABLE_NAME_SPU) $(EXECUTABLE_NAME_ASM)  $(EXECUTABLE_NAME_DISASM)

$(OBJECTS_READ): %.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o ./$(BUILDDIR)/$@

#____________________________SPU________________________________
$(EXECUTABLE_NAME_SPU): $(OBJECTS_SPU)
	$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_SPU)) -o ./$(BUILDDIR)/$(EXECUTABLE_NAME_SPU)

$(OBJECTS_SPU): %.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGSH_SPU) -c $^ -o ./$(BUILDDIR)/$@

#____________________________ASM________________________________
$(EXECUTABLE_NAME_ASM): $(OBJECTS_ASM)
	$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_ASM)) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_READ))  -o ./$(BUILDDIR)/$(EXECUTABLE_NAME_ASM)

$(OBJECTS_ASM): %.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGSH_ASM) -c $^ -o ./$(BUILDDIR)/$@

#____________________________DISASM________________________________
$(EXECUTABLE_NAME_DISASM): $(OBJECTS_DISASM)
	$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_DISASM)) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_READ))  -o ./$(BUILDDIR)/$(EXECUTABLE_NAME_DISASM)

$(OBJECTS_DISASM): %.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGSH_DISASM) -c $^ -o ./$(BUILDDIR)/$@

make_folder:
	mkdir -p $(BUILDDIR)/$(SPUDIR)/$(STACKDIR)/
	mkdir -p $(BUILDDIR)/$(ASMDIR)/
	mkdir -p $(BUILDDIR)/$(READFILEDIR)/

start_spu:
	./$(BUILDDIR)/$(EXECUTABLE_NAME_SPU)

start_asm:
	./$(BUILDDIR)/$(EXECUTABLE_NAME_ASM)

start_disasm:
	./$(BUILDDIR)/$(EXECUTABLE_NAME_DISASM)

clean:
	rm -rf ./$(BUILDDIR)/
	rm -f dump.txt