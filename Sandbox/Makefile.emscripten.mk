#
# Makefile to use with GLFW+emscripten
# See https://emscripten.org/docs/getting_started/downloads.html
# for installation instructions.
#
# This Makefile assumes you have loaded emscripten's environment.
# (On Windows, you may need to execute emsdk_env.bat or encmdprompt.bat ahead)
#
# Running `make -f Makefile.emscripten` will produce three files:
#  - web/index.html
#  - web/index.js
#  - web/index.wasm
#
# All three are needed to run the demo.

CC = emcc
CXX = em++
AR = emar

ifndef verbose
  SILENT = @
endif

CONFIG ?= Debug
PRJNAME = Sanbox

OUTDIR = ../bin/browser/$(CONFIG)
OBJDIR  = ../bin-int/browser/$(CONFIG)/$(PRJNAME)
SRCDIR = ./src

EXE = $(OUTDIR)/index.html
LIB = $(OUTDIR)/$(PRJNAME).a

INCLUDE_FLAGS = -I./src -I../Lucky/src
INCLUDE_LIB_FLAGS = -I../Vendors/SDL/include -I../Vendors/spdlog/include -I../Vendors/ImGui -I../Vendors/GLM

SUBDIRS = $(SRCDIR)
VPATH := $(SUBDIRS)

SOURCES = $(shell cd $(SRCDIR) && dir /s/b *.cpp)
OBJS    = $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SOURCES)))
LIBS = $(OUTDIR)/Lucky.a $(OUTDIR)/ImGui.a

CPPFLAGS =
LDFLAGS =
EMS =

##---------------------------------------------------------------------
## EMSCRIPTEN OPTIONS
##---------------------------------------------------------------------

# ("EMS" options gets added to both CPPFLAGS and LDFLAGS, whereas some options are for linker only)
EMS += -s DISABLE_EXCEPTION_CATCHING=1
LDFLAGS += -s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1

# Uncomment next line to fix possible rendering bugs with Emscripten version older then 1.39.0 (https://github.com/ocornut/imgui/issues/2877)
#EMS += -s BINARYEN_TRAP_MODE=clamp
#EMS += -s SAFE_HEAP=1    ## Adds overhead

# Emscripten allows preloading a file or folder to be accessible at runtime.
# The Makefile for this example project suggests embedding the misc/fonts/ folder into our application, it will then be accessible as "/fonts"
# See documentation for more details: https://emscripten.org/docs/porting/files/packaging_files.html
# (Default value is 0. Set to 1 to enable file-system and include the misc/fonts/ folder as part of the build.)
USE_FILE_SYSTEM ?= 0
ifeq ($(USE_FILE_SYSTEM), 0)
LDFLAGS += -s NO_FILESYSTEM=1
CPPFLAGS += -DIMGUI_DISABLE_FILE_FUNCTIONS
endif
ifeq ($(USE_FILE_SYSTEM), 1)
LDFLAGS += --no-heap-copy --preload-file ../Vendors/ImGui/misc/fonts@/fonts
endif

##---------------------------------------------------------------------
## FINAL BUILD FLAGS
##---------------------------------------------------------------------

CPPFLAGS += $(INCLUDE_FLAGS) $(INCLUDE_LIB_FLAGS)
ifeq ($(CONFIG), DEBUG)
CPPFLAGS += -O0 -g 
else
CPPFLAGS += -Os
endif
CPPFLAGS += -Wall -Wformat $(EMS)
# LDFLAGS += --shell-file ../libs/emscripten/shell_minimal.html
LDFLAGS += $(EMS)

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OBJDIR)/%.o: %.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ 

all: $(EXE)
	@echo Build complete for $(EXE)

$(OBJDIR):
	$(SILENT) mkdir "$@"

$(OUTDIR):
	$(SILENT) mkdir "$@"

$(LIB): $(OBJDIR) $(OUTDIR) $(OBJS)
	@echo $(notdir $<)
	$(SILENT) $(AR) rcs $(LIB) $(OBJS)

$(EXE): $(OUTDIR) $(OBJDIR) $(OBJS)
	@echo $(notdir $<)
	$(SILENT) $(CXX) -o $@ $(OBJS) $(LIBS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)

clean:
	$(SILENT) rm -rf $(OBJS) $(OUTDIR)