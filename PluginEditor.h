#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ExciterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ExciterAudioProcessorEditor (ExciterAudioProcessor&);
    ~ExciterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ExciterAudioProcessor& audioProcessor;

    juce::Slider levelSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExciterAudioProcessorEditor)
};
