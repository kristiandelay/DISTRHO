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
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "__cdecl=""" -D "NDEBUG=1" -I "../source" -I "../source/core" -I "../source/sys" -I "../source/vsti" -I "/usr/include" -I "../../../sdks/vstsdk2.4"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 -O2 -mtune=generic -msse -ffast-math -fomit-frame-pointer -fvisibility=hidden -fPIC
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared -s
  LDDEPS :=
  RESFLAGS := -D "LINUX=1" -D "__cdecl=""" -D "NDEBUG=1" -I "../source" -I "../source/core" -I "../source/sys" -I "../source/vsti" -I "/usr/include" -I "../../../sdks/vstsdk2.4"
  TARGET := sorollet.so
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Debug)
  BINDIR := ../../../bin
  LIBDIR := ../../../bin
  OBJDIR := intermediate/Debug
  OUTDIR := ../../../bin
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "__cdecl=""" -D "DEBUG=1" -D "_DEBUG=1" -I "../source" -I "../source/core" -I "../source/sys" -I "../source/vsti" -I "/usr/include" -I "../../../sdks/vstsdk2.4"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -g -O0 -ggdb -fPIC
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared
  LDDEPS :=
  RESFLAGS := -D "LINUX=1" -D "__cdecl=""" -D "DEBUG=1" -D "_DEBUG=1" -I "../source" -I "../source/core" -I "../source/sys" -I "../source/vsti" -I "/usr/include" -I "../../../sdks/vstsdk2.4"
  TARGET := sorollet_debug.so
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
	$(OBJDIR)/SorolletFilter.o \
	$(OBJDIR)/SorolletADSR.o \
	$(OBJDIR)/SorolletVoice.o \
	$(OBJDIR)/SorolletEQ.o \
	$(OBJDIR)/MathUtils.o \
	$(OBJDIR)/SorolletVSTi.o \
	$(OBJDIR)/SorolletVSTiProgram.o \
	$(OBJDIR)/main.o \
	$(OBJDIR)/audioeffectx.o \
	$(OBJDIR)/audioeffect.o \
	$(OBJDIR)/vstplugmain.o \

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
	@echo Linking sorollet
	-@$(CMD_MKBINDIR)
	-@$(CMD_MKLIBDIR)
	-@$(CMD_MKOUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning sorollet
ifeq ($(MKDIR_TYPE),posix)
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)
else
	-@if exist $(subst /,\,$(OUTDIR)/$(TARGET)) del /q $(subst /,\,$(OUTDIR)/$(TARGET))
	-@if exist $(subst /,\,$(OBJDIR)) del /q $(subst /,\,$(OBJDIR))
	-@if exist $(subst /,\,$(OBJDIR)) rmdir /s /q $(subst /,\,$(OBJDIR))
endif

$(OBJDIR)/SorolletFilter.o: ../source/core/SorolletFilter.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SorolletADSR.o: ../source/core/SorolletADSR.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SorolletVoice.o: ../source/core/SorolletVoice.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SorolletEQ.o: ../source/core/SorolletEQ.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MathUtils.o: ../source/sys/MathUtils.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SorolletVSTi.o: ../source/vsti/SorolletVSTi.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SorolletVSTiProgram.o: ../source/vsti/SorolletVSTiProgram.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/main.o: ../source/vsti/main.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/audioeffectx.o: ../../../sdks/vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/audioeffect.o: ../../../sdks/vstsdk2.4/public.sdk/source/vst2.x/audioeffect.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/vstplugmain.o: ../../../sdks/vstsdk2.4/public.sdk/source/vst2.x/vstplugmain.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)

