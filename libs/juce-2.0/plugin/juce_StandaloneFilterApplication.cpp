/*
  ==============================================================================

   Custom Standalone Window

  ==============================================================================
*/

#include "juce_StandaloneFilterWindow.h"

//==============================================================================
/**
    A class that can be used to run a simple standalone application containing your filter.

    Just create one of these objects in your JUCEApplication::initialise() method, and
    let it do its work. It will create your filter object using the same createFilter() function
    that the other plugin wrappers use.
*/
class StandaloneFilterApplication : public JUCEApplication
{
public:

    //==============================================================================
    StandaloneFilterApplication()
        : window (nullptr),
          appProperties (nullptr)
    {
    }

    ~StandaloneFilterApplication()
    {
    }

    //==============================================================================
    void initialise (const String& commandLine)
    {
        DBG ("StandaloneFilterApplication::initialise");

        // set-up options
        PropertiesFile::Options options;
        options.applicationName = getApplicationName();
        options.filenameSuffix = "xml";
        options.folderName = ".distrho";
        options.osxLibrarySubFolder = "Application Support";
        options.commonToAllUsers = false;
        options.ignoreCaseOfKeyNames = true;

        // open up config
        appProperties = new ApplicationProperties();
        appProperties->setStorageParameters (options);

        // setup window name
        String pluginWindowName;
        pluginWindowName << JucePlugin_Name << " v" << JucePlugin_VersionString;
#ifdef JUCE_DEBUG
        pluginWindowName << " (DEBUG)";
#endif

        // create the window
        window = new StandaloneFilterWindow (pluginWindowName, Colour (32, 32, 32), appProperties->getUserSettings(), commandLine);

        window->toFront (true);
        window->setVisible (true);

        Process::setPriority (Process::HighPriority);
    }

    void shutdown()
    {
        DBG ("StandaloneFilterApplication::shutdown");

        if (window)
        {
            window->setVisible (false);
            deleteAndZero (window);
        }

        if (appProperties)
        {
            appProperties->saveIfNeeded ();
            appProperties->closeFiles ();
        }
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        DBG ("StandaloneFilterApplication::systemRequestedQuit");

        quit ();
    }

    void ladishSaveRequested()
    {
        window->saveState();
    }

    void unhandledException (const std::exception* e,
                             const String& sourceFilename,
                             const int lineNumber)
    {
    #ifndef JUCE_DEBUG
        AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                    "Unhandled Exception",
                                    "Something bad happened to the application.");
        quit ();
    #endif
    }

    //==============================================================================
    const String getApplicationName()
    {
        return JucePlugin_Name;
    }

    const String getApplicationVersion()
    {
        return JucePlugin_VersionString;
    }

    //==============================================================================
    bool moreThanOneInstanceAllowed()
    {
        return true;
    }

private:
    StandaloneFilterWindow* window;
    ApplicationProperties* appProperties;

    StandaloneFilterApplication (const StandaloneFilterApplication&);
    const StandaloneFilterApplication& operator= (const StandaloneFilterApplication&);
};

START_JUCE_APPLICATION(StandaloneFilterApplication)
