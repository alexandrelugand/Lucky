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

# verbose=1
ifndef verbose
  SILENT = @
endif

CONFIG ?= Debug
PRJNAME = Lucky

OUTDIR = ../bin/browser/$(CONFIG)
OBJDIR  = ../bin-int/browser/$(CONFIG)/$(PRJNAME)
SRCDIR = ./src/Lucky
VENDORS = ../Vendors

EXE = $(OUTDIR)/$(PRJNAME).html
LIB = $(OUTDIR)/$(PRJNAME).a

INCLUDE_FLAGS = -I./src
INCLUDE_LIB_FLAGS = -I$(VENDORS)/SDL/include -I$(VENDORS)/spdlog/include -I$(VENDORS)/ImGui -I$(VENDORS)/ImGui/backends -I$(VENDORS)/GLM -I$(VENDORS)/stb -I$(VENDORS)/entt/include

SUBDIRS = $(SRCDIR)
SUBDIRS += $(SRCDIR)/Application
SUBDIRS += $(SRCDIR)/Core
SUBDIRS += $(SRCDIR)/Core/ImGui
SUBDIRS += $(SRCDIR)/Renderer
SUBDIRS += $(SRCDIR)/Scene
SUBDIRS += $(SRCDIR)/Platforms/OpenGL
SUBDIRS += $(SRCDIR)/Platforms/Windows
SUBDIRS += $(SRCDIR)/Platforms/Windows
SUBDIRS += $(VENDORS)/stb/stb
VPATH := $(SUBDIRS)

SOURCES = $(shell cd $(SRCDIR) && dir /s/b *.cpp)
SOURCES += $(shell cd ../Vendors/stb && dir /s/b *.cpp)
OBJS    = $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

CPPFLAGS =
LDFLAGS =
EMS =

##---------------------------------------------------------------------
## EMSCRIPTEN OPTIONS
##---------------------------------------------------------------------

# ("EMS" options gets added to both CPPFLAGS and LDFLAGS, whereas some options are for linker only)
EMS += -s DISABLE_EXCEPTION_CATCHING=1
LDFLAGS += -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 --use-preload-plugins

# Uncomment next line to fix possible rendering bugs with Emscripten version older then 1.39.0 (https://github.com/ocornut/imgui/issues/2877)
#EMS += -s BINARYEN_TRAP_MODE=clamp
#EMS += -s SAFE_HEAP=1    ## Adds overhead

# Emscripten allows preloading a file or folder to be accessible at runtime.
# The Makefile for this example project suggests embedding the misc/fonts/ folder into our application, it will then be accessible as "/fonts"
# See documentation for more details: https://emscripten.org/docs/porting/files/packaging_files.html
# (Default value is 0. Set to 1 to enable file-system and include the misc/fonts/ folder as part of the build.)
USE_FILE_SYSTEM ?= 1
ifeq ($(USE_FILE_SYSTEM), 0)
LDFLAGS += -s NO_FILESYSTEM=1
endif
ifeq ($(USE_FILE_SYSTEM), 1)
LDFLAGS += --no-heap-copy -s FORCE_FILESYSTEM=1
endif

USE_INDEX_DB_FILE_SYSTEM ?= 0
ifeq ($(USE_INDEX_DB_FILE_SYSTEM), 1)
LDFLAGS += -lidbfs.js
endif

##---------------------------------------------------------------------
## FINAL BUILD FLAGS
##---------------------------------------------------------------------

CPPFLAGS += $(INCLUDE_FLAGS) $(INCLUDE_LIB_FLAGS) -s USE_SDL=2 -s USE_SDL_IMAGE=2
ifeq ($(CONFIG), Debug)
CPPFLAGS += -O0 -g -DDEBUG
else
CPPFLAGS += -Os -DNDEBUG
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

all: $(LIB)
	@echo Build complete for $(LIB)

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