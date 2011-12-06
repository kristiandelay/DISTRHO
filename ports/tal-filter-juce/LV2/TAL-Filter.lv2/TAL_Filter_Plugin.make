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
  OUTDIR := ../../../bin/TAL-Filter.lv2
  CPPFLAGS := $(DEPFLAGS) -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_LV2=1" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_Standalone=0" -D "LINUX=1" -D "NDEBUG=1" -I "../source" -I "../source/Engine" -I "." -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 `pkg-config --cflags freetype2` -O2 -mtune=generic -msse -ffast-math -fomit-frame-pointer -fvisibility=hidden -fPIC
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared -s `pkg-config --libs freetype2` -L"../../../libs" -lfreetype -lpthread -lrt -lX11 -lXext -ljuce-plugin-153
  LDDEPS :=
  RESFLAGS := -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_LV2=1" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_Standalone=0" -D "LINUX=1" -D "NDEBUG=1" -I "../source" -I "../source/Engine" -I "." -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source"
  TARGET := TAL_Filter_Plugin.so
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Debug)
  BINDIR := ../../../bin
  LIBDIR := ../../../bin
  OBJDIR := intermediate/Debug
  OUTDIR := ../../../bin/TAL-Filter.lv2
  CPPFLAGS := $(DEPFLAGS) -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_LV2=1" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_Standalone=0" -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -I "../source" -I "../source/Engine" -I "." -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -g `pkg-config --cflags freetype2` -O0 -ggdb -fPIC
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared `pkg-config --libs freetype2` -L"../../../libs" -lfreetype -lpthread -lrt -lX11 -lXext -ljuce-plugin-153_debug
  LDDEPS :=
  RESFLAGS := -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_LV2=1" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_Standalone=0" -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -I "../source" -I "../source/Engine" -I "." -I "../../../libs/juce-153/plugin" -I "../../../libs/juce-153/source"
  TARGET := TAL_Filter_Plugin_debug.so
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
	$(OBJDIR)/TalComponent.o \
	$(OBJDIR)/TalCore.o \
	$(OBJDIR)/Lfo.o \
	$(OBJDIR)/juce_LV2_Wrapper.o \

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
	@echo Linking TAL-Filter.lv2/TAL_Filter_Plugin
	-@$(CMD_MKBINDIR)
	-@$(CMD_MKLIBDIR)
	-@$(CMD_MKOUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning TAL-Filter.lv2/TAL_Filter_Plugin
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

$(OBJDIR)/juce_LV2_Wrapper.o: ../../../libs/juce-lv2/juce_LV2_Wrapper.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)

