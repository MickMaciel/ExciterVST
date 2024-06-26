#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExciterAudioProcessorEditor::ExciterAudioProcessorEditor (ExciterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Initialize the slider
    levelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 30);
    levelSlider.setNumDecimalPlacesToDisplay(1);
    addAndMakeVisible(levelSlider);

    // Attach the slider to the parameter
    levelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "level", levelSlider);

    // Set the size of the editor window
    setSize (400, 300);
}

ExciterAudioProcessorEditor::~ExciterAudioProcessorEditor()
{
}

void ExciterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill the background with a solid color
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Set the color for the text and draw it
    g.setColour (juce::Colours::white);
    g.setFont (30.0f);
    g.drawFittedText ("E x c i t e r", getLocalBounds(), juce::Justification::centredTop, 1);
}

void ExciterAudioProcessorEditor::resized()
{
    // Center the slider in the editor
    int sliderWidth = 200;
    int sliderHeight = 200;
    levelSlider.setBounds((getWidth() - sliderWidth) / 2, (getHeight() - sliderHeight) / 2, sliderWidth, sliderHeight);
}
