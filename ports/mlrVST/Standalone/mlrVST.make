# C++ Console Executable Makefile autogenerated by premake
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
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_Standalone=1" -I "../source/Source" -I "../source/JuceLibraryCode" -I "../source/oscpack" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/standalone" -I "../../../libs/juce-153/source" -I "."
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O2 -O2 -march=native -msse -ffast-math -fvisibility=hidden -static
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -s -L"/usr/X11R6/lib" -L"/usr/lib" -L"../../../libs" -lfreetype -lpthread -lasound -lrt -lX11 -lXext -ljuce-standalone-153
  LDDEPS :=
  RESFLAGS := -D "LINUX=1" -D "NDEBUG=1" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_Standalone=1" -I "../source/Source" -I "../source/JuceLibraryCode" -I "../source/oscpack" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/standalone" -I "../../../libs/juce-153/source" -I "."
  TARGET := mlrVST
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Debug)
  BINDIR := ../../../bin
  LIBDIR := ../../../bin
  OBJDIR := intermediate/Debug
  OUTDIR := ../../../bin
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_Standalone=1" -I "../source/Source" -I "../source/JuceLibraryCode" -I "../source/oscpack" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/standalone" -I "../../../libs/juce-153/source" -I "."
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -O0 -ggdb -static
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L"/usr/X11R6/lib" -L"/usr/lib" -L"../../../libs" -lfreetype -lpthread -lasound -lrt -lX11 -lXext -ljuce-standalone-153_debug
  LDDEPS :=
  RESFLAGS := -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JucePlugin_Build_VST=0" -D "JucePlugin_Build_AU=0" -D "JucePlugin_Build_RTAS=0" -D "JucePlugin_Build_Standalone=1" -I "../source/Source" -I "../source/JuceLibraryCode" -I "../source/oscpack" -I "/usr/include" -I "/usr/include/freetype2" -I "../../../libs/juce-153/standalone" -I "../../../libs/juce-153/source" -I "."
  TARGET := mlrVST_debug
 BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
	$(OBJDIR)/AudioSample.o \
	$(OBJDIR)/ChannelProcessor.o \
	$(OBJDIR)/OSCHandler.o \
	$(OBJDIR)/PluginEditor.o \
	$(OBJDIR)/PluginProcessor.o \
	$(OBJDIR)/SampleStrip.o \
	$(OBJDIR)/SampleStripControl.o \
	$(OBJDIR)/mlrVSTLookAndFeel.o \
	$(OBJDIR)/BinaryData.o \
	$(OBJDIR)/IpEndpointName.o \
	$(OBJDIR)/NetworkingUtils.o \
	$(OBJDIR)/UdpSocket.o \
	$(OBJDIR)/OscOutboundPacketStream.o \
	$(OBJDIR)/OscPrintReceivedElements.o \
	$(OBJDIR)/OscReceivedElements.o \
	$(OBJDIR)/OscTypes.o \
	$(OBJDIR)/juce_StandaloneFilterWindow.o \
	$(OBJDIR)/juce_StandaloneFilterApplication.o \

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
	@echo Linking mlrVST
	-@$(CMD_MKBINDIR)
	-@$(CMD_MKLIBDIR)
	-@$(CMD_MKOUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning mlrVST
ifeq ($(MKDIR_TYPE),posix)
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)
else
	-@if exist $(subst /,\,$(OUTDIR)/$(TARGET)) del /q $(subst /,\,$(OUTDIR)/$(TARGET))
	-@if exist $(subst /,\,$(OBJDIR)) del /q $(subst /,\,$(OBJDIR))
	-@if exist $(subst /,\,$(OBJDIR)) rmdir /s /q $(subst /,\,$(OBJDIR))
endif

$(OBJDIR)/AudioSample.o: ../source/Source/AudioSample.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChannelProcessor.o: ../source/Source/ChannelProcessor.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OSCHandler.o: ../source/Source/OSCHandler.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginEditor.o: ../source/Source/PluginEditor.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginProcessor.o: ../source/Source/PluginProcessor.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SampleStrip.o: ../source/Source/SampleStrip.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SampleStripControl.o: ../source/Source/SampleStripControl.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/mlrVSTLookAndFeel.o: ../source/Source/mlrVSTLookAndFeel.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData.o: ../source/JuceLibraryCode/BinaryData.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/IpEndpointName.o: ../source/oscpack/ip/IpEndpointName.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/NetworkingUtils.o: ../source/oscpack/ip/posix/NetworkingUtils.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UdpSocket.o: ../source/oscpack/ip/posix/UdpSocket.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscOutboundPacketStream.o: ../source/oscpack/osc/OscOutboundPacketStream.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscPrintReceivedElements.o: ../source/oscpack/osc/OscPrintReceivedElements.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscReceivedElements.o: ../source/oscpack/osc/OscReceivedElements.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OscTypes.o: ../source/oscpack/osc/OscTypes.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_StandaloneFilterWindow.o: ../../../libs/juce-custom/Standalone/juce_StandaloneFilterWindow.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_StandaloneFilterApplication.o: ../../../libs/juce-custom/Standalone/juce_StandaloneFilterApplication.cpp
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)

