###############################################################################
#                                                                             #
#            ▓█████ ▄▄▄█████▓ ██░ ██  ▄▄▄       ███▄    █ ▓█████              #
#            ▓█   ▀ ▓  ██▒ ▓▒▓██░ ██▒▒████▄     ██ ▀█   █ ▓█   ▀              #
#            ▒███   ▒ ▓██░ ▒░▒██▀▀██░▒██  ▀█▄  ▓██  ▀█ ██▒▒███                #
#            ▒▓█  ▄ ░ ▓██▓ ░ ░▓█ ░██ ░██▄▄▄▄██ ▓██▒  ▐▌██▒▒▓█  ▄              #
#            ░▒████▒  ▒██▒ ░ ░▓█▒░██▓ ▓█   ▓██▒▒██░   ▓██░░▒████▒             #
#            ░░ ▒░ ░  ▒ ░░    ▒ ░░▒░▒ ▒▒   ▓▒█░░ ▒░   ▒ ▒ ░░ ▒░ ░             #
#             ░ ░  ░    ░     ▒ ░▒░ ░  ▒   ▒▒ ░░ ░░   ░ ▒░ ░ ░  ░             #
#               ░     ░       ░  ░░ ░  ░   ▒      ░   ░ ░    ░                #
#                                                                             #
#            Ethane, a Research Project for hacking GoldSrc games.            #
#              Open-Source & Public Domain. Free as in freedom.               #
#                                                                             #
#            This is a research project (helped me learn some C++)            #
#            and I wanted to share it to the interweb. It's a hack            #
#            library made specifically for those who want to learn            #
#            how this stuff works. I've wrote lots of comments and            #
#               tried my best to *overexplain* everything here.               #
#                                                                             #
#                                  Have fun!                                  #
#                            ..: 2026 neotesk. :..                            #
#                                                                             #
###############################################################################

# ----------------------------[ PROJECT DETAILS ]---------------------------- #
    PROJECT_NAME    := Ethane
    PROJECT_VERSION := 0.0.1
    PROJECT_AUTHOR  := neotesk
    PROJECT_SOURCE  := https://github.com/neotesk/ethane
# --------------------------------------------------------------------------- #

# ----------------------------[ TARGET OPTIONS ]----------------------------- #
	TARGET_DIRECTORY := dist
	TARGET_LIBRARY   := libethane.so
	TARGET_INJECTOR  := injector
# --------------------------------------------------------------------------- #

# -----------------------------[ BUILD OPTIONS ]----------------------------- #
#           Make sure to run `make clean build` after changing the            #
#             variables below. Otherwise the changes won't work.              #
# --------------------------------------------------------------------------- #
	E_DEBUG           := true
	E_UI_SOUNDS       := true
	E_EXP_HIGHRRMOUSE := false
# --------------------------------------------------------------------------- #

# -----------------------------[ PROJECT FILES ]----------------------------- #
#             This project consist of two subprojects. One is the             #
#               cheat itself and the other one is the injector.               #
# --------------------------------------------------------------------------- #
	ETHANE_SOURCES := \
		src/core/main.cpp

	INJECTOR_SOURCES := \
		src/injector/main.cpp
# --------------------------------------------------------------------------- #

# --------------------------[ INCLUDE DIRECTORIES ]-------------------------- #
	INCLUDES_CXX := \
		lib \
		inc \
		src

	INCLUDES_CC :=
# --------------------------------------------------------------------------- #

# -------------------------------[ RESOURCES ]------------------------------- #
#           These resources only work for the cheat library itself.           #
#                  No resources are needed for the injector.                  #
# --------------------------------------------------------------------------- #
	RESOURCES :=
# --------------------------------------------------------------------------- #

###############################################################################
###############################################################################
####                                                                       ####
####         This is the part where you shouldn't touch. You might         ####
####        break things apart or even face unforeseen consequences.       ####
####                                                                       ####
###############################################################################
###############################################################################

# ---------------------------[ COMPILER OPTIONS ]---------------------------- #
#           These compiler options are for Linux x86 32-bit because           #
#           we are targetting the pre-anniversary build of GoldSrc.           #
# --------------------------------------------------------------------------- #
	COMP_CXX_EXEC  := g++
	COMP_CXX_FLAGS := -fPIC -fpermissive -m32 -fno-gnu-unique

	COMP_CC_EXEC   := gcc
	COMP_CC_FLAGS  := -fPIC -fpermissive -m32

	COMP_LIBS      := dl
	COMP_LD_FLAGS  := -m32 -L/usr/lib32
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
#               Check if our build dependencies are installed.                #
# --------------------------------------------------------------------------- #
ifeq "$(shell which $(COMP_CXX_EXEC) 2> /dev/null)" ""
_1:
	$(error Build dependency '$(COMP_CXX_EXEC)' is not installed)
endif

ifeq "$(shell which $(COMP_CC_EXEC) 2> /dev/null)" ""
_2:
	$(error Build dependency '$(COMP_CC_EXEC)' is not installed)
endif

ifeq "$(shell which objcopy 2> /dev/null)" ""
_1:
	$(error Build dependency 'objcopy' is not installed)
endif
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
#                    Definitions, macros and other stuff.                     #
# --------------------------------------------------------------------------- #
ANSI_BOLD := $(shell printf "\e[1;98m")
ANSI_GRAY  := $(shell printf "\e[0;90m")
ANSI_RESET := $(shell printf "\e[0m")

TXT_LD    := "$(ANSI_BOLD)LD       $(ANSI_RESET)"
TXT_CC    := "$(ANSI_BOLD)CC       $(ANSI_RESET)"
TXT_CXX   := "$(ANSI_BOLD)CXX      $(ANSI_RESET)"
TXT_COPY  := "$(ANSI_BOLD)COPY     $(ANSI_RESET)"
TXT_RES   := "$(ANSI_BOLD)RESOURCE $(ANSI_RESET)"
TXT_MKDIR := "$(ANSI_BOLD)MKDIR    $(ANSI_RESET)"
TXT_RM    := "$(ANSI_BOLD)RM       $(ANSI_RESET)"
TXT_TOUCH := "$(ANSI_BOLD)TOUCH    $(ANSI_RESET)"
TXT_EMPTY := "         "
PAD_ZERO  := "    "

TMPDIR := $(TARGET_DIRECTORY)/.buildcache
IDXFIL := $(TMPDIR)/.index

OBJS_ETHANE   := $(ETHANE_SOURCES:%=$(TMPDIR)/%.o)
OBJS_INJECTOR := $(INJECTOR_SOURCES:%=$(TMPDIR)/%.o)
RESOURCE_OBJS := $(RESOURCES:%=$(TMPDIR)/%.resource)

LINKER_OUT_ETHANE   := $(OBJS_ETHANE) $(RESOURCE_OBJS)
LINKER_OUT_INJECTOR := $(OBJS_INJECTOR)
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
#                           Build flags management                            #
# --------------------------------------------------------------------------- #
ifeq "$(E_DEBUG)" "true"
	COMP_CXX_FLAGS += -DE_DEBUG=true
	COMP_CC_FLAGS += -DE_DEBUG=true
endif
ifeq "$(E_UI_SOUNDS)" "true"
	COMP_CXX_FLAGS += -DE_UI_SOUNDS=true
	COMP_CC_FLAGS += -DE_UI_SOUNDS=true
endif
ifeq "$(E_EXP_HIGHRRMOUSE)" "true"
	COMP_CXX_FLAGS += -DE_EXP_HIGHRRMOUSE=true
	COMP_CC_FLAGS += -DE_EXP_HIGHRRMOUSE=true
endif

COMP_CXX_FLAGS += -MMD -MP \
	"-DPROJECT_NAME=\"$(PROJECT_NAME)\"" \
	"-DPROJECT_VERSION=\"$(PROJECT_VERSION)\"" \
	"-DPROJECT_AUTHOR=\"$(PROJECT_AUTHOR)\"" \
	"-DPROJECT_SOURCE=\"$(PROJECT_SOURCE)\"" \
	"-DTARGET_LIBRARY=\"$(TARGET_LIBRARY)\"" \
	$(addprefix -I, $(INCLUDES_CXX))

COMP_CC_FLAGS += -MMD -MP \
	"-DPROJECT_NAME=\"$(PROJECT_NAME)\"" \
	"-DPROJECT_VERSION=\"$(PROJECT_VERSION)\"" \
	"-DPROJECT_AUTHOR=\"$(PROJECT_AUTHOR)\"" \
	"-DPROJECT_SOURCE=\"$(PROJECT_SOURCE)\"" \
	"-DTARGET_LIBRARY=\"$(TARGET_LIBRARY)\"" \
	$(addprefix -I, $(INCLUDES_CC))

COMP_LD_FLAGS += $(addprefix -l, $(COMP_LIBS))

COMP_LD_ETHANE := -Wl,-m,elf_i386 -shared $(COMP_LD_FLAGS) $(LINKER_OUT_ETHANE)
COMP_LD_INJECTOR := $(COMP_LD_FLAGS) $(LINKER_OUT_INJECTOR)

BUILD_ETHANE_PATH := $(TARGET_DIRECTORY)/$(TARGET_LIBRARY)
BUILD_INJECTOR_PATH := $(TARGET_DIRECTORY)/$(TARGET_INJECTOR)

TOTAL_OBJ := $(words $(OBJS_ETHANE) $(OBJS_INJECTOR) $(BUILD_ETHANE_PATH) \
	$(BUILD_INJECTOR_PATH))
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
#                              Build procedures                               #
# --------------------------------------------------------------------------- #
.PHONY: info help build clean $(BUILD_ETHANE_PATH) $(BUILD_INJECTOR_PATH)
all: build

help:
	@echo "$(PROJECT_NAME)'s Makefile! Version $(PROJECT_VERSION)"
	@echo -e "Open-Source & Public Domain. 2026 $(PROJECT_AUTHOR).\n"
	@echo -e "Usage: make ...command (default: build)\n"
	@echo "Commands:"
	@echo "    help      Prints this exact help page"
	@echo "    info      Prints a list of processed variables"
	@echo "    build     Builds the library"
	@echo "    clean     Deletes all build artifacts"

info:
	@echo "-------------- [Project Information] --------------"
	@echo "Name:        $(PROJECT_NAME)"
	@echo "Version:     $(PROJECT_VERSION)"
	@echo "Author:      $(PROJECT_AUTHOR)"
	@echo "Source URL:  $(PROJECT_SOURCE)"
	@echo -e "---------------------------------------------------\n"
	@echo "-------------- [Paths & Directories] --------------"
	@echo "Build Library Path:     $(BUILD_ETHANE_PATH)"
	@echo "Build Injector Path:    $(BUILD_INJECTOR_PATH)"
	@echo "Build Directory:        $(TARGET_DIRECTORY)"
	@echo "Build Cache Directory:  $(TMPDIR)"
	@echo "Build Index Path:       $(IDXFIL)"
	@echo -e "---------------------------------------------------\n"
	@echo "--------------- [Compiler Options] ----------------"
	@echo "C++ Compiler Executable: $(COMP_CXX_EXEC)"
	@echo -e "C99 Compiler Executable: $(COMP_CC_EXEC) \n"
	@echo "C++ Compiler Flags:"
	@$(foreach f,$(COMP_CXX_FLAGS),echo "    $(f)";)
	@echo "C99 Compiler Flags:"
	@$(foreach f,$(COMP_CC_FLAGS),echo "    $(f)";)
	@echo "Linker Flags:"
	@$(foreach f,$(COMP_LD_FLAGS),echo "    $(f)";)
	@echo "Linker Flags (for library):"
	@$(foreach f,$(COMP_LD_ETHANE),echo "    $(f)";)
	@echo "Linker Flags (for injector):"
	@$(foreach f,$(COMP_LD_INJECTOR),echo "    $(f)";)
	@echo -e "---------------------------------------------------\n"

clean:
	@echo $(PAD_ZERO) $(TXT_RM) "$(TARGET_DIRECTORY)"
	@rm -rf "$(TARGET_DIRECTORY)"

__prep: $(TMPDIR)
	@echo "0" > "$(IDXFIL)"

build: __prep $(BUILD_ETHANE_PATH) $(BUILD_INJECTOR_PATH)
	@rm -f "$(IDXFIL)"

$(TARGET_DIRECTORY):
	@echo $(PAD_ZERO) $(TXT_MKDIR) "$(TARGET_DIRECTORY)"
	@mkdir -p "$(TARGET_DIRECTORY)"

$(TMPDIR) :$(TARGET_DIRECTORY)
	@echo $(PAD_ZERO) $(TXT_MKDIR) "$(TMPDIR)"
	@mkdir -p "$(TMPDIR)"

$(TMPDIR)/%.resource: %
	@mkdir -p "$(dir $@)"
	@echo $(PAD_ZERO) $(TXT_RES) "$<"
	@objcopy -I binary -O elf32-i386 -B i386 \
	    --rename-section .data=.rodata,alloc,load,readonly,data,contents \
	    "$<" "$@"
	@SYM_BASE="_binary_$(subst .,_,$(subst /,_,$(subst -,_,$<)))"; \
	objcopy --globalize-symbol $${SYM_BASE}_start \
	        --globalize-symbol $${SYM_BASE}_size $@

$(TMPDIR)/%.c.o: %.c
	@mkdir -p "$(dir $@)"
	@echo $$(($$(cat "$(IDXFIL)" 2>/dev/null || \
	    echo -1) + 1)) > "$(IDXFIL)"
	@\
	    percent=$$(($$(cat "$(IDXFIL)") * 100 / $(TOTAL_OBJ))); \
	    printf "$(ANSI_GRAY)%3d%%$(ANSI_RESET) " "$$percent"; \
	    echo $(TXT_CC) "$<"; \
	    $(COMP_CC_EXEC) $(COMP_CC_FLAGS) -c "$<" -o "$@" || exit 1

$(TMPDIR)/%.cpp.o: %.cpp
	@mkdir -p "$(dir $@)"
	@echo $$(($$(cat "$(IDXFIL)" 2>/dev/null || \
	    echo -1) + 1)) > "$(IDXFIL)"
	@\
	    percent=$$(($$(cat "$(IDXFIL)") * 100 / $(TOTAL_OBJ))); \
	    printf "$(ANSI_GRAY)%3d%%$(ANSI_RESET) " "$$percent"; \
	    echo $(TXT_CXX) "$<"; \
	    $(COMP_CXX_EXEC) $(COMP_CXX_FLAGS) -c "$<" -o "$@" || exit 1

$(BUILD_ETHANE_PATH): $(RESOURCE_OBJS) $(OBJS_ETHANE) $(OBJS_INJECTOR)
	@echo $$(($$(cat "$(IDXFIL)" 2>/dev/null || \
	    echo -1) + 1)) > "$(IDXFIL)"
	@\
	    percent=$$(($$(cat "$(IDXFIL)") * 100 / $(TOTAL_OBJ))); \
	    printf "$(ANSI_GRAY)%3d%%$(ANSI_RESET) " "$$percent"; \
	    echo $(TXT_LD) "$@"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(COMP_LD_ETHANE) -o $@ || exit 1

$(BUILD_INJECTOR_PATH): $(OBJS_INJECTOR) $(BUILD_ETHANE_PATH)
	@echo $$(($$(cat "$(IDXFIL)" 2>/dev/null || \
	    echo -1) + 1)) > "$(IDXFIL)"
	@\
	    percent=$$(($$(cat "$(IDXFIL)") * 100 / $(TOTAL_OBJ))); \
	    printf "$(ANSI_GRAY)%3d%%$(ANSI_RESET) " "$$percent"; \
	    echo $(TXT_LD) "$@"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(COMP_LD_INJECTOR) -o $@ || exit 1

ALL_DEPS := $(OBJS:.o=.d)
-include $(wildcard $(ALL_DEPS))
# --------------------------------------------------------------------------- #