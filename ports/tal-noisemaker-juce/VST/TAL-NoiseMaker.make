# C++ Shared Library Makefile autogenerated by premake
# Don't edit this file! Instead edit `premake.lua` then rerun `make`

ifndef CONFIG
  CONFIG=Release
endif

# if multiple archs are defined turn off automated dependency generation
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Release)
  BINDIR := ../../../bin
  LIBDIR := ../../../bin
  OBJDIR := intermediate/Release
  OUTDIR := ../../../bin
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCE_USE_VSTSDK_2_4=1" -I "../source" -I "../source/Engine" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source" -I "../../../sdks/vstsdk2.4" -I "." -I "./intermediate" -I "./intermediate/Release"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O2 -O2 -march=native -msse -ffast-math -fvisibility=hidden -static
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared -s -L"/usr/X11R6/lib" -L"/usr/lib" -L"../../../libs" -lfreetype -lpthread -lrt -lX11 -lXext -ljuce-plugin-153
  LDDEPS :=
  RESFLAGS := -D "LINUX=1" -D "NDEBUG=1" -D "JUCE_USE_VSTSDK_2_4=1" -I "../source" -I "../source/Engine" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source" -I "../../../sdks/vstsdk2.4" -I "." -I "./intermediate" -I "./intermediate/Release"
  TARGET := libTAL-NoiseMaker.so
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Debug)
  BINDIR := ../../../bin
  LIBDIR := ../../../bin
  OBJDIR := intermediate/Debug
  OUTDIR := ../../../bin
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCE_USE_VSTSDK_2_4=1" -I "../source" -I "../source/Engine" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source" -I "../../../sdks/vstsdk2.4" -I "." -I "./intermediate" -I "./intermediate/Release"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -g -O0 -ggdb -static
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared -L"/usr/X11R6/lib" -L"/usr/lib" -L"../../../libs" -lfreetype -lpthread -lrt -lX11 -lXext -ljuce-plugin-153_debug
  LDDEPS :=
  RESFLAGS := -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCE_USE_VSTSDK_2_4=1" -I "../source" -I "../source/Engine" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source" -I "../../../sdks/vstsdk2.4" -I "." -I "./intermediate" -I "./intermediate/Release"
  TARGET := libTAL-NoiseMaker_debug.so
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
	$(OBJDIR)/TalComponent.o \
	$(OBJDIR)/TalCore.o \
	$(OBJDIR)/Lfo.o \
	$(OBJDIR)/juce_VST_Wrapper.o \

MKDIR_TYPE := msdos
CMD := $(subst \,\\,$(ComSpec)$(COMSPEC))
ifeq (,$(CMD))
  MKDIR_TYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  MKDIR_TYPE := posix
endif
ifeq ($(MKDIR_TYPE),posix)
  CMD_MKBINDIR := mkdir -p $(BINDIR)
  CMD_MKLIBDIR := mkdir -p $(LIBDIR)
  CMD_MKOUTDIR := mkdir -p $(OUTDIR)
  CMD_MKOBJDIR := mkdir -p $(OBJDIR)
else
  CMD_MKBINDIR := $(CMD) /c if not exist $(subst /,\\,$(BINDIR)) mkdir $(subst /,\\,$(BINDIR))
  CMD_MKLIBDIR := $(CMD) /c if not exist $(subst /,\\,$(LIBDIR)) mkdir $(subst /,\\,$(LIBDIR))
  CMD_MKOUTDIR := $(CMD) /c if not exist $(subst /,\\,$(OUTDIR)) mkdir $(subst /,\\,$(OUTDIR))
  CMD_MKOBJDIR := $(CMD) /c if not exist $(subst /,\\,$(OBJDIR)) mkdir $(subst /,\\,$(OBJDIR))
endif

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking TAL-NoiseMaker
	-@$(CMD_MKBINDIR)
	-@$(CMD_MKLIBDIR)
	-@$(CMD_MKOUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning TAL-NoiseMaker
ifeq ($(MKDIR_TYPE),posix)
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)
else
	-@if exist $(subst /,\,$(OUTDIR)/$(TARGET)) del /q $(subst /,\,$(OUTDIR)/$(TARGET))
	-@if exist $(subst /,\,$(OBJDIR)) del /q $(subst /,\,$(OBJDIR))
	-@if exist $(subst /,\,$(OBJDIR)) rmdir /s /q $(subst /,\,$(OBJDIR))
endif

$(OBJDIR)/TalComponent.o: ../source/TalComponent.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TalCore.o: ../source/TalCore.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Lfo.o: ../source/Engine/Lfo.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper.o: ../../../libs/juce-153/source/src/audio/plugin_client/VST/juce_VST_Wrapper.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
