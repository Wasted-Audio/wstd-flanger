/*
 * Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later
 */

#include "DistrhoUI.hpp"
#include "ResizeHandle.hpp"
#include "veramobd.hpp"

START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------

class ImGuiPluginUI : public UI
{
    float ffeedback = 0.0f;
    float fintensity = 20.0f;
    float fmix = 50.0f;
    float fspeed = 20.0f;

    ResizeHandle fResizeHandle;

    // ----------------------------------------------------------------------------------------------------------------

public:
   /**
      UI class constructor.
      The UI should be initialized to a default state that matches the plugin side.
    */
    ImGuiPluginUI()
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

        ImGuiIO& io(ImGui::GetIO());

        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = true;
        fc.OversampleH = 1;
        fc.OversampleV = 1;
        fc.PixelSnapH = true;

        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 16.0f, &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 21.0f, &fc);
        io.Fonts->Build();
        io.FontDefault = io.Fonts->Fonts[1];

        fResizeHandle.hide();
    }

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // DSP/Plugin Callbacks

   /**
      A parameter has changed on the plugin side.@n
      This is called by the host to inform the UI about parameter changes.
    */
    void parameterChanged(uint32_t index, float value) override
    {
        switch (index) {
            case 0: {
                ffeedback = value;
                break;
            }
            case 1: {
                fintensity = value;
                break;
            }
            case 2: {
                fmix = value;
                break;
            }
            case 3: {
                fspeed = value;
                break;
            }

            default: return;
        }

        repaint();
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Widget Callbacks

   /**
      ImGui specific onDisplay function.
    */
    void onImGuiDisplay() override
    {

        const float width = getWidth();
        const float height = getHeight();
        const float margin = 0.0f;

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);


        style.Colors[ImGuiCol_TitleBgActive] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.40f);
        style.Colors[ImGuiCol_WindowBg] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.10f);

        ImGuiIO& io(ImGui::GetIO());
        ImFont* defaultFont = ImGui::GetFont();
        ImFont* titleBarFont = io.Fonts->Fonts[2];

        ImGui::PushFont(titleBarFont);
        if (ImGui::Begin("WSTD FLANGR", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::PushFont(defaultFont);
            auto ImGuiKnob_Flags = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick;
            auto ImGuiKnob_FlagsDB = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
            auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(0.03f / 3.6f, 0.76f, 0.74f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(0.03f / 3.6f, 0.76f, 0.84f));
            if (ImGuiKnobs::Knob("Intensity", &fintensity, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(1, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        fintensity = 20.0f;

                }
                setParameterValue(1, fintensity);
            }
            ImGui::SameLine();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(1.6f / 3.6f, 0.77f, 0.64f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(1.6f / 3.6f, 0.77f, 0.74f));
            if (ImGuiKnobs::Knob("Speed", &fspeed, 0.0f, 20.0f, 0.05f, "%.1fHz", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 21))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(3, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        fspeed = 2.0f;

                }
                setParameterValue(3, fspeed);
            }
            ImGui::SameLine();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(2.04f / 3.6f, 0.83f, 0.64f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(2.04f / 3.6f, 0.83f, 0.84f));
            if (ImGuiKnobs::Knob("Feedback", &ffeedback, -100.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SpaceBipolar, 100, ImGuiKnob_Flags))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(0, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        ffeedback = 0.0f;

                }
                setParameterValue(0, ffeedback);
            }
            ImGui::SameLine();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();


            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(0.47f / 3.6f, 0.75f, 0.64f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(0.47f / 3.6f, 0.75f, 0.84f));
            if (ImGuiKnobs::Knob("Mix", &fmix, 0.0f, 100.0f, 1.0f, "%.0f%%", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_Flags, 11))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(2, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        fmix = 50.0f;

                }
                setParameterValue(2, fmix);
            }
            ImGui::SameLine();

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            if (ImGui::IsItemDeactivated())
            {
            editParameter(0, false);
            editParameter(1, false);
            editParameter(2, false);
            editParameter(3, false);

            }
        }
        ImGui::End();
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImGuiPluginUI)
};

// --------------------------------------------------------------------------------------------------------------------

UI* createUI()
{
    return new ImGuiPluginUI();
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
