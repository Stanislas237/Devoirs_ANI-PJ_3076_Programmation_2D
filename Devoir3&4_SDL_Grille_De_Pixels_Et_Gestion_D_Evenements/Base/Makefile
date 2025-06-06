# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
INCLUDES += -Isrc -I../External/Libs/SDL3/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS += -lpthread -ldl -lSDL3
LDDEPS +=
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../Build/bin/Debug-linux-x86_64/Base
TARGET = $(TARGETDIR)/Base
OBJDIR = ../Build/obj/Debug-linux-x86_64/Base
DEFINES += -DNKENTSEU_STATIC -DNKENTSEU_DEBUG -D_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -g -Wall -Wextra -Wpedantic -Wshadow -Werror
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -g -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Werror
ALL_LDFLAGS += $(LDFLAGS) -L../External/Libs/SDL3/mingw/lib -L/usr/lib64 -m64 -static-libstdc++
define POSTBUILDCMDS
endef

else ifeq ($(config),release_x64)
TARGETDIR = ../Build/bin/Release-linux-x86_64/Base
TARGET = $(TARGETDIR)/Base
OBJDIR = ../Build/obj/Release-linux-x86_64/Base
DEFINES += -DNKENTSEU_STATIC -DNKENTSEU_RELEASE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -Wall -Wextra -Wpedantic -Wshadow -Werror
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Werror
ALL_LDFLAGS += $(LDFLAGS) -L../External/Libs/SDL3/mingw/lib -L/usr/lib64 -m64 -s -static-libstdc++
define POSTBUILDCMDS
	@echo Running postbuild commands
	cp -rf ../External/Libs/SDL3/msvc/x64/SDL3.dll ../Build/bin/Release-linux-x86_64/Base
endef

else ifeq ($(config),custom_x64)
TARGETDIR = ../Build/bin/Custom-linux-x86_64/Base
TARGET = $(TARGETDIR)/Base
OBJDIR = ../Build/obj/Custom-linux-x86_64/Base
DEFINES += -DNKENTSEU_STATIC
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wall -Wextra -Wpedantic -Wshadow -Werror
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Werror
ALL_LDFLAGS += $(LDFLAGS) -L../External/Libs/SDL3/mingw/lib -L/usr/lib64 -m64 -s -static-libstdc++
define POSTBUILDCMDS
endef

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Base.o
GENERATED += $(OBJDIR)/Color.o
GENERATED += $(OBJDIR)/Screen.o
OBJECTS += $(OBJDIR)/Base.o
OBJECTS += $(OBJDIR)/Color.o
OBJECTS += $(OBJDIR)/Screen.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking Base
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning Base
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/Base.o: src/Base/Base.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Color.o: src/Base/Color.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Screen.o: src/Base/Screen.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif