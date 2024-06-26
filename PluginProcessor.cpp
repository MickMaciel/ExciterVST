#include "PluginProcessor.h"
#include "PluginEditor.h"

ExciterAudioProcessor::ExciterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                      #if ! JucePlugin_IsMidiEffect
                       #if JucePlugin_IsSynth
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       #else
                        .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       #endif
                      #endif
                       ),
       parameters(*this, nullptr, juce::Identifier("ExciterAudioProcessor"),
       {
           std::make_unique<juce::AudioParameterFloat>("level", "Level", 0.0f, 10.0f, 1.0f)
       })
#endif
{
}

ExciterAudioProcessor::~ExciterAudioProcessor()
{
}

void ExciterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void ExciterAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ExciterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
     || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
        return true;

    return false;
}
#endif

void ExciterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get the current value of the level parameter
    float level = parameters.getRawParameterValue("level")->load();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            float outputSample;

            if (inputSample > 1.0f / level)
                outputSample = 1.0f - expf(-inputSample);
            else if (inputSample < -1.0f / level)
                outputSample = -1.0f + expf(inputSample);
            else
                outputSample = inputSample * level;

            channelData[sample] = outputSample;
        }
    }
}

juce::AudioProcessorEditor* ExciterAudioProcessor::createEditor()
{
    return new ExciterAudioProcessorEditor (*this);
}

bool ExciterAudioProcessor::hasEditor() const
{
    return true;
}

const juce::String ExciterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ExciterAudioProcessor::acceptsMidi() const
{
    return JucePlugin_WantsMidiInput;
}

bool ExciterAudioProcessor::producesMidi() const
{
    return JucePlugin_ProducesMidiOutput;
}

bool ExciterAudioProcessor::isMidiEffect() const
{
    return JucePlugin_IsMidiEffect;
}

double ExciterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ExciterAudioProcessor::getNumPrograms()
{
    return 1;
}

int ExciterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExciterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ExciterAudioProcessor::getProgramName (int index)
{
    return {};
}

void ExciterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void ExciterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    parameters.state.writeToStream(stream);
}

void ExciterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);
    parameters.state.readFromStream(stream);
}
