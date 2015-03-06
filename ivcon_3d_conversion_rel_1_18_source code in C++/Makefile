# Project: ivcon
# Makefile created by Dev-C++ 4.9.9.2

ifneq (,$(findstring Windows,$(OS)))
  EXEEXT = .exe
  RES = obj/ivcon_private.res
else
  EXEEXT =
  RES =
endif
CPP   = $(CROSS_COMPILE)g++
CC    = $(CROSS_COMPILE)gcc
WINDRES  = $(CROSS_COMPILE)windres
DLLTOOL  = $(CROSS_COMPILE)dlltool
RM    = rm -f
CP    = cp -f
MKDIR    = mkdir -p
ECHO     = @echo

OBJS = \
obj/main.o \
obj/ivconv.o \
obj/utils.o \
obj/asc.o \
obj/gmod.o \
obj/tds.o \
obj/tmat.o \
obj/txt.o \
obj/obj.o \
obj/sw_vehrd.o \
obj/ase.o \
obj/iv.o \
obj/hrc.o \
obj/smf.o \
obj/stl.o \
obj/xgl.o \
obj/tri.o \
obj/dxf.o \
obj/byu.o \
obj/wrl.o \
obj/pov.o \
obj/vla.o \
obj/ucd.o \
obj/tec.o \
obj/sw_maprd.o \
obj/swv.o \
obj/swm.o \
obj/cob.o \
$(RES)
LIBS = 
INCS = 
CXXINCS = 
BIN  = bin/ivcon$(EXEEXT)
CXXFLAGS = $(CXXINCS) -c -fmessage-length=0  
CFLAGS = $(INCS) -c -fmessage-length=0  
LDFLAGS = 

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJS) $(BIN)

all-before:
	$(MKDIR) obj
	$(MKDIR) bin


$(BIN): $(OBJS)
	-$(ECHO) 'Building target: $@'
	$(CPP) -o "$@" $(OBJS) $(LDFLAGS)
	-$(ECHO) 'Finished building target: $@'
	-$(ECHO) ' '

obj/%.o: src/%.cpp
	-$(ECHO) 'Building file: $<'
	$(CPP) $(CXXFLAGS) -o"$@" "$<"
	-$(ECHO) 'Finished building: $<'
	-$(ECHO) ' '

# Windows resources compilation
# Should depend on res/keeperfx_icon.ico, but currently we've assuming ICO files are alredy made 
obj/%.res: res/%.rc
	-$(ECHO) 'Building resource: $<'
	$(WINDRES) -i "$<" --input-format=rc -o "$@" -O coff
	-$(ECHO) 'Finished building: $<'
	-$(ECHO) ' '
