
project.name    = "HybridReverb2"
project.bindir  = "../../../bin"
project.libdir  = project.bindir
project.configs = { "Release", "Debug" }

package = newpackage()
package.name = project.name
package.kind = "exe"
package.language = "c++"

package.defines   = { "JucePlugin_Build_AU=0", "JucePlugin_Build_LV2=0", "JucePlugin_Build_RTAS=0", "JucePlugin_Build_VST=0", "JucePlugin_Build_Standalone=1" }
package.linkflags = { "no-symbols", "static-runtime" }

package.config["Release"].target     = project.name
package.config["Release"].objdir     = "intermediate/Release"
package.config["Release"].defines    = { "NDEBUG=1" }
package.config["Release"].buildflags = { "no-symbols", "optimize-speed" }
package.config["Release"].links      = { "juce-standalone-153" }

package.config["Debug"].target       = project.name .. "_debug"
package.config["Debug"].objdir       = "intermediate/Debug"
package.config["Debug"].defines      = { "DEBUG=1", "_DEBUG=1" }
package.config["Debug"].links        = { "juce-standalone-153_debug" }

if (windows) then
  package.defines = { package.defines, "WINDOWS=1" }
else
  package.config["Release"].buildoptions = { "-O2 -mtune=generic -msse -ffast-math -fomit-frame-pointer -funroll-loops -fopenmp -fvisibility=hidden -fPIC" }
--   package.config["Release"].linkoptions  = { "-Wl,-O1 -Wl,--as-needed" }
  package.config["Debug"].buildoptions   = { "-O0 -ggdb -msse -fPIC" }
  if (macosx) then
    package.defines = { package.defines, "MAC=1" }
  else
    package.defines = { package.defines, "LINUX=1" }
    package.links   = { "freetype", "pthread", "asound", "dl", "rt", "X11", "Xext", "gomp" }
    package.buildoptions = { "`pkg-config --cflags freetype2 fftw3f`" }
    package.linkoptions  = { "`pkg-config --libs freetype2 fftw3f`" }
  end
end

package.includepaths = {
  "../source",
  "../../../libs/juce-153/standalone",
  "../../../libs/juce-153/source"
}

package.libpaths = {
  "../../../libs"
}

package.files = {
  matchfiles (
    "../source/*.cpp",
    "../source/gui/*.cpp",
    "../source/libHybridConv/*.c",
    "../../../libs/juce-custom/Standalone/*.cpp"
  )
}