.PHONY: all clean

PLATFORM			?= PLATFORM_WEB

# Build mode for library: DEBUG or RELEASE
CONFIG			?= DEBUG

# Library type used: STATIC (.a) or SHARED (.so/.dll)
LIBTYPE				?= STATIC

# Build output name for the library
LIB_NAME			?= App

SDL_INCLUDE_PATH	?= ../Vendors/SDL/include
SDL_LIBRARY_PATH	?= ../Vendors/SDL/lib

PLATFORM_OS ?= WINDOWS

# Determine PLATFORM_OS when required
ifeq ($(OS),Windows_NT)
	PLATFORM_OS = WINDOWS
	ifndef PLATFORM_SHELL
		PLATFORM_SHELL = cmd
	endif
endif
ifeq ($(PLATFORM_OS),LINUX)
	ifndef PLATFORM_SHELL
		PLATFORM_SHELL = sh
	endif
endif

ifeq ($(PLATFORM_OS), WINDOWS)
	# Emscripten required variables
	EMSDK_PATH			?= C:/emsdk
	EMSCRIPTEN_PATH		?= $(EMSDK_PATH)/upstream/emscripten
	CLANG_PATH			:= $(EMSDK_PATH)/upstream/bin
	PYTHON_PATH			:= $(EMSDK_PATH)/python/3.9.2-nuget_64bit
	NODE_PATH			:= $(EMSDK_PATH)/node/16.20.0_64bit/bin
	export PATH			:= $(EMSDK_PATH);$(EMSCRIPTEN_PATH);$(CLANG_PATH);$(NODE_PATH);$(PYTHON_PATH);C:/msys64/mingw64/bin;$(PATH)
endif

# On HTML5 OpenGL ES 2.0 is used, emscripten translates it to WebGL 1.0
GRAPHICS = GRAPHICS_API_OPENGL_ES3
#GRAPHICS = GRAPHICS_API_OPENGL_ES3      # Uncomment to use ES3/WebGL2 (preliminary support).

# Define default C compiler and archiver to pack library: CC, AR
#------------------------------------------------------------------------------------------------
# HTML5 emscripten compiler
CC = emcc
AR = emar

SRC_PATH            ?= src
INTERM_PATH         ?= ../bin-int/$(CONFIG)/$(PLATFORM_OS)/browser
RELEASE_PATH        ?= ../bin/$(CONFIG)/browser

# Define compiler flags: CFLAGS
#------------------------------------------------------------------------------------------------
#  -O1                      defines optimization level
#  -g                       include debug information on compilation
#  -s                       strip unnecessary data from build --> linker
#  -Wall                    turns on most, but not all, compiler warnings
#  -std=c99                 defines C language mode (standard C from 1999 revision)
#  -std=gnu99               defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces      ignore invalid warning (GCC bug 53119)
#  -Wno-unused-value        ignore unused return values of some functions (i.e. fread())
#  -D_DEFAULT_SOURCE        use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
#  -D_GNU_SOURCE            access to lots of nonstandard GNU/Linux extension functions
#  -Werror=pointer-arith    catch unportable code that does direct arithmetic on void pointers
#  -fno-strict-aliasing     jar_xm.h does shady stuff (breaks strict aliasing)
CFLAGS = -Wall -D_GNU_SOURCE -Wno-error=unused-command-line-argument -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing $(CUSTOM_CFLAGS)

# NOTE: When using multi-threading in the user code, it requires -pthread enabled
# -Os                        # size optimization
# -O2                        # optimization level 2, if used, also set --memory-init-file 0
# -s USE_GLFW=3              # Use glfw3 library (context/input management) -> Only for linker!
# -s ALLOW_MEMORY_GROWTH=1   # to allow memory resizing -> WARNING: Audio buffers could FAIL!
# -s TOTAL_MEMORY=16777216   # to specify heap memory size (default = 16MB)
# -s USE_PTHREADS=1          # multithreading support
# -s FORCE_FILESYSTEM=1      # force filesystem to load/save files data
# -s ASSERTIONS=1            # enable runtime checks for common memory allocation errors (-O1 and above turn it off)
# --profiling                # include information for code profiling
# --memory-init-file 0       # to avoid an external memory initialization code file (.mem)
# --preload-file resources   # specify a resources folder for data compilation
CFLAGS += -std=c++17 -fexperimental-library -s WASM=1 -s USE_SDL=2 -s MAX_WEBGL_VERSION=2 -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap
ifeq ($(CONFIG),DEBUG)
	CFLAGS += -O0 --profiling
else
	CFLAGS += -Os
endif

# Define include paths for required headers: INCLUDE_PATHS
# NOTE: Several external required libraries (stb and others)
#------------------------------------------------------------------------------------------------
INCLUDE_PATHS = -I$(SRC_PATH) -I../Vendors/SDL/include -I../Vendors/spdlog/include -I../Lucky/src

# Define library paths containing required libs: LDFLAGS
# NOTE: This is only required for dynamic library generation
#------------------------------------------------------------------------------------------------
LDFLAGS = $(CUSTOM_LDFLAGS) -L. -L$(RELEASE_PATH)

# Define source code object files required
#------------------------------------------------------------------------------------------------
OBJS = 	$(INTERM_PATH)/Sandbox.o 
LIBS = 	$(RELEASE_PATH)/Lucky.a 

# Define processes to execute
#------------------------------------------------------------------------------------------------
# Default target entry
all: app

output_guard=@if not exist $(RELEASE_PATH)\ (mkdir "$(RELEASE_PATH)")
interm_guard=@if not exist $(INTERM_PATH)\ (mkdir "$(INTERM_PATH)")

# Compile library
# NOTE: Release directory is created if not exist
app: $(OBJS)
ifeq ($(PLATFORM),PLATFORM_WEB)
	$(output_guard)
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OBJS) $(LIBS) -o $(RELEASE_PATH)/App.html
	@echo "Application generated (App.html)!"
endif

$(INTERM_PATH)/Sandbox.o : $(SRC_PATH)/Sandbox.cpp
	$(interm_guard)
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -o $@

.PHONY: clean_shell_cmd clean_shell_sh

# Clean everything
clean:	clean_shell_$(PLATFORM_SHELL)
	@echo "removed all generated files!"

clean_shell_sh:
	rm -fv *.o $(RELEASE_PATH)/$(LIB_NAME).a

# Set specific target variable
clean_shell_cmd: SHELL=cmd
clean_shell_cmd:
	@if exist $(INTERM_PATH)\ (rmdir /S /Q "$(INTERM_PATH)") & \
	@if exist $(RELEASE_PATH)\ (rmdir /S /Q "$(RELEASE_PATH)") & \