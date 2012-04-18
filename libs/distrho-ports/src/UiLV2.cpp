// distrho lv2 ui

#include "DistrhoPlugin.h"

#if DISTRHO_PLUGIN_WANTS_UI
#include "PluginBase.h"
#include "UiBase.h"

#include "lv2-sdk/ui.h"

class DistrhoUiLv2 : public QObject
{
    Q_OBJECT

public:
    DistrhoUiLv2(LV2UI_Write_Function write_function_, LV2UI_Controller controller_, LV2UI_Widget* widget) :
        QObject(nullptr),
        write_function(write_function_),
        controller(controller_)
    {
        m_ui = createDistrhoUI();
        *widget = m_ui;

        controlPortOffset  = 0;
#if DISTRHO_PLUGIN_IS_SYNTH
        controlPortOffset += 1;
#endif
        controlPortOffset += DISTRHO_PLUGIN_NUM_INPUTS;
        controlPortOffset += DISTRHO_PLUGIN_NUM_OUTPUTS;

        connect(m_ui, SIGNAL(parameterChanged(uint32_t,float)), this, SLOT(pluginParameterChanged(uint32_t,float)));
    }

    virtual ~DistrhoUiLv2()
    {
        delete m_ui;
    }

    void lv2_port_event(uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
    {
        if (m_ui)
        {
            if (format == 0 && buffer_size == sizeof(float))
            {
                const float value = *(float*)buffer;
                m_ui->setParameterValue(port_index-controlPortOffset, value);
            }
        }
    }

private slots:
    void pluginParameterChanged(uint32_t index, float value)
    {
        if (write_function && controller)
            write_function(controller, index+controlPortOffset, sizeof(float), 0, &value);
    }

private:
    DistrhoUiBase* m_ui;
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;

    uint32_t controlPortOffset;
};

#include "UiLV2.moc"

// ---------------------------------------------------------------------------------------------

static LV2UI_Handle ui_instantiate(const LV2UI_Descriptor*, const char* plugin_uri, const char*, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget* widget, const LV2_Feature* const*)
{
    if (strcmp(plugin_uri, DISTRHO_PLUGIN_URI) == 0)
        return new DistrhoUiLv2(write_function, controller, widget);
    return nullptr;
}

static void ui_cleanup(LV2UI_Handle instance)
{
    delete (DistrhoUiLv2*)instance;
}

static void ui_port_event(LV2UI_Handle instance, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
    DistrhoUiLv2* ui = (DistrhoUiLv2*)instance;
    ui->lv2_port_event(port_index, buffer_size, format, buffer);
}

static const void* ui_extension_data(const char* /*uri*/)
{
    return nullptr;
}

static const LV2UI_Descriptor uidescriptor = {
    DISTRHO_PLUGIN_URI "#Qt4UI",
    ui_instantiate,
    ui_cleanup,
    ui_port_event,
    ui_extension_data
};

// ---------------------------------------------------------------------------------------------

DISTRHO_PLUGIN_EXPORT
const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
    return (index == 0) ? &uidescriptor : nullptr;
}

#endif // DISTRHO_PLUGIN_WANTS_UI