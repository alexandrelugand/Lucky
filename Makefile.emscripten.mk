# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifeq ($(config),debug)
  ImGui_config = Debug
  Lucky_config = Debug
  SandBox_config = Debug

else ifeq ($(config),release)
  ImGui_config = Release
  Lucky_config = Release
  SandBox_config = Release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := Lucky SandBox

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

ImGui:
ifneq (,$(ImGui_config))
	@echo "==== Building ImGui ($(ImGui_config)) ===="
	@${MAKE} --no-print-directory -C Vendors/ImGui -f Makefile.emscripten.mk CONFIG=$(ImGui_config)
endif

Lucky:
ifneq (,$(Lucky_config))
	@echo "==== Building Lucky ($(Lucky_config)) ===="
	@${MAKE} --no-print-directory -C Lucky -f Makefile.emscripten.mk CONFIG=$(Lucky_config)
endif

SandBox: ImGui Lucky
ifneq (,$(SandBox_config))
	@echo "==== Building App ($(SandBox_config)) ===="
	@${MAKE} --no-print-directory -C SandBox -f Makefile.emscripten.mk CONFIG=$(SandBox_config)
endif

clean:
	@${MAKE} --no-print-directory -C Lucky -f Makefile.emscripten.mk clean
	@${MAKE} --no-print-directory -C SandBox -f Makefile.emscripten.mk clean

help:
	@echo "Usage: make -f Makefile.emscripten.mk [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo "  wasm"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   Lucky"
	@echo "   App"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"