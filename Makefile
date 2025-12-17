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

SOURCEDIR      = Source
INCLUDEDIR     = Include

BUILDDIR       = Build

READFILEDIR    = ReadFile
SOURCES_READ   = $(wildcard $(READFILEDIR)/*.cpp)
OBJECTS_READ   = $(patsubst %.cpp, %.o, $(SOURCES_READ))


SPUDIR        = SPU
STACKDIR      = Stack
SOURCES_SPU   = $(wildcard $(SPUDIR)/*.cpp) $(SPUDIR)/$(STACKDIR)/stack.cpp 
OBJECTS_SPU   = $(patsubst %.cpp, %.o, $(SOURCES_SPU))
CXXFLAGSH_SPU = -I$(SPUDIR)/$(STACKDIR) -DSHOW_DUMP


ASMDIR        = ASM
SOURCES_ASM   = $(wildcard $(ASMDIR)/$(SOURCEDIR)/*.cpp)
OBJECTS_ASM   = $(patsubst %.cpp, %.o, $(SOURCES_ASM))
INCLUDE_ASM   = $(ASMDIR)/$(INCLUDEDIR)/
CXXFLAGSH_ASM = -I$(READFILEDIR) -I$(INCLUDE_ASM)

.PHONY = all clean start_spu start_asm

all: print_start make_folder $(OBJECTS_READ) $(EXECUTABLE_NAME_ASM) $(EXECUTABLE_NAME_SPU)
	@echo ====COMPILATION WAS FINISHED====

print_start:
	@echo ==COMPILATION HAS BEEN STARTED==
#____________________________READ_______________________________
$(OBJECTS_READ): %.o: %.cpp
	@$(CC) $(CFLAGS) -c $^ -o ./$(BUILDDIR)/$@

#____________________________SPU________________________________
$(EXECUTABLE_NAME_SPU): $(OBJECTS_SPU)
	@$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_SPU)) -o ./$(BUILDDIR)/$(EXECUTABLE_NAME_SPU)
	@echo SPU was compiled!

$(OBJECTS_SPU): %.o: %.cpp
	@$(CC) $(CFLAGS) $(CXXFLAGSH_SPU) -c $^ -o ./$(BUILDDIR)/$@

#____________________________ASM________________________________
$(EXECUTABLE_NAME_ASM): $(OBJECTS_ASM)
	@$(CC) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_ASM)) $(addprefix ./$(BUILDDIR)/, $(OBJECTS_READ))  -o ./$(BUILDDIR)/$(EXECUTABLE_NAME_ASM)
	@echo ASM was compiled!

$(OBJECTS_ASM): %.o: %.cpp
	@$(CC) $(CFLAGS) $(CXXFLAGSH_ASM) -c $^ -o ./$(BUILDDIR)/$@

#_____________________CREAT FOLDERS_____________________________
make_folder:
	@mkdir -p $(BUILDDIR)/$(SPUDIR)/$(STACKDIR)/
	@mkdir -p $(BUILDDIR)/$(ASMDIR)/$(SOURCEDIR)/
	@mkdir -p $(BUILDDIR)/$(READFILEDIR)/
	@echo Folders were created!

#________________________STARTS_________________________________
start_spu:
	./$(BUILDDIR)/$(EXECUTABLE_NAME_SPU)

start_asm:
	./$(BUILDDIR)/$(EXECUTABLE_NAME_ASM) factorial.asm

start_img:
	python3 mk_img.py video.mp4

#_________________________CLEAN_________________________________
clean:	
	@rm -rf ./$(BUILDDIR)/
	@rm -f dump.txt
	@echo Cleaning completed!