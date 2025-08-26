/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "ProtectYourEars.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor() : AudioProcessor (
                                                             BusesProperties()
                                                             .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                                             .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
                                                             params(apvts)
{
    
    lowCutFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
const juce::String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    params.prepareToPlay(sampleRate);
    params.reset();
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    
    double numSamples = Parameters::maxDelayTime / 1000 * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLineL.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineR.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineL.reset();
    delayLineR.reset();
    
    feedbackL = 0.0f;
    feedbackR = 0.0f;
    
    lowCutFilter.prepare(spec);
    lowCutFilter.reset();
    
    highCutFilter.prepare(spec);
    lowCutFilter.reset();
    
    lastLowCut = -1.0f;
    lastHighCut = -1.0f;
    
    tempo.reset();
    
    levelL.reset();
    levelR.reset();
    
    delayInSamples = 0.0f;
    targetDelay = 0.0f;
    fade = 1.0f;
    fadeTarget = 1.0f;
    coeff = 1.0f - std::exp(-1.0f / (0.05f * float(sampleRate)));
    wait = 0.0f;
    waitInc = 1.0f / (0.3f * float(sampleRate)); // 300ms
    
    /* crossfade implementation
    xFade = 0.0f;
    xFadeInc = static_cast<float>(1.0 / (0.05 * sampleRate)); // 50ms
    */
}

void DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();
    
    DBG("isBusesLayoutSupported, in: " << mainIn.getDescription() << " , out: " << mainOut.getDescription());
    
    if (mainIn == mono && mainOut == mono) { return true;}
    if (mainIn == mono && mainOut == stereo) { return true;}
    if (mainIn == stereo && mainOut == stereo) { return true;}
    
    return false;
}

void DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                            [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    params.update();
    
    tempo.update(getPlayHead());
    
    float syncedTime = float(tempo.getMillisecondsForNoteLength(params.delayNote));
    
    if (syncedTime > Parameters::maxDelayTime) {
        syncedTime = Parameters::maxDelayTime;
    }
    
    float sampleRate = float(getSampleRate());
    
    auto mainInput = getBusBuffer(buffer, true, 0);
    auto mainInputChannels = mainInput.getNumChannels();
    auto isMainInputStereo = mainInputChannels > 1;
    const float* inputDataL = mainInput.getReadPointer(0);
    const float* inputDataR = mainInput.getReadPointer(isMainInputStereo ? 1 : 0);
    
    auto mainOutput = getBusBuffer(buffer, false, 0);
    auto mainOutputChannels = mainOutput.getNumChannels();
    auto isMainOutputStereo = mainOutputChannels > 1;
    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(isMainOutputStereo ? 1 : 0);
    
    if (isMainInputStereo) {
        
        float maxL = 0.0f;
        float maxR = 0.0f;
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            params.smoothen();
            
            /* crossfade implementation
            if (xFade == 0.0f) {
                float delayTime = params.tempoSync ? syncedTime : params.delayTime;
                targetDelay = delayTime / 1000.0f * sampleRate;
                
                if (delayInSamples == 0.0f) {
                    delayInSamples = targetDelay;
                }
                
                else if (targetDelay != delayInSamples) {
                    xFade = xFadeInc;
                }
            }
            */
            
            float delayTime = params.tempoSync ? syncedTime : params.delayTime;
            float newTargetDelay = delayTime / 1000.0f * sampleRate;
            
            if (newTargetDelay != targetDelay) {
                targetDelay = newTargetDelay;
                
                if (delayInSamples == 0.0f) {
                    delayInSamples = targetDelay;
                }
                
                else {
                    wait = waitInc;
                    fadeTarget = 0.0f;
                }
            }
            
            if (params.lowCut != lastLowCut){
                lowCutFilter.setCutoffFrequency(params.lowCut);
                lastLowCut = params.lowCut;
            }
            
            if (params.highCut != lastHighCut){
                highCutFilter.setCutoffFrequency(params.highCut);
                lastHighCut = params.highCut;
            }
            
            float dryL = inputDataL[sample];
            float dryR = inputDataR[sample];
            
            float mono = (dryL + dryR) * 0.5f;
            
            delayLineL.write(mono * params.panL + feedbackR);
            delayLineR.write(mono * params.panR + feedbackL);
            
            float wetL = delayLineL.read(delayInSamples);
            float wetR = delayLineR.read(delayInSamples);
            
            fade += (fadeTarget - fade) * coeff;
            wetL *= fade;
            wetR *= fade;
            
            if (wait > 0.0f) {
                wait += waitInc;
                if (wait >= 1.0f) {
                    delayInSamples = targetDelay;
                    wait = 0.0f;
                    fadeTarget = 1.0f; // fade in
                }
            }
            
            /* crossfade implementation
            if (xFade > 0.0f) {
                float newL = delayLineL.read(targetDelay);
                float newR = delayLineR.read(targetDelay);
                
                wetL = (1.0f - xFade) * wetL + xFade * newL;
                wetR = (1.0f - xFade) * wetR + xFade * newR;
                
                xFade += xFadeInc;
                if (xFade >= 1.0f) {
                    delayInSamples = targetDelay;
                    xFade = 0.0f;
                }
            }
            */
            
            feedbackL = wetL * params.feedback;
            feedbackL = lowCutFilter.processSample(0, feedbackL);
            feedbackL = highCutFilter.processSample(0, feedbackL);
            
            feedbackR = wetR * params.feedback;
            feedbackR = lowCutFilter.processSample(1, feedbackR);
            feedbackR = highCutFilter.processSample(1, feedbackR);
            
            float mixL = dryL + wetL * params.mix;
            float mixR = dryR + wetR * params.mix;
            
            float outL = mixL * params.gain;
            float outR = mixR * params.gain;
            
            if (params.bypassed) {
                outL = dryL;
                outR = dryR;
            }
            
            outputDataL[sample] = outL;
            outputDataR[sample] = outR;
            
            maxL = std::max(maxL, std::abs(outL));
            maxR = std::max(maxR, std::abs(outR));
        }
        
        levelL.updateIfGreater(maxL);
        levelR.updateIfGreater(maxR);
        
    } else {
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            params.smoothen();
            
            float delayInSamples = params.delayTime / 1000.0f * sampleRate;
            
            float dry = inputDataL[sample];
            delayLineL.write(dry + feedbackL);
            
            float wet = delayLineL.read(delayInSamples);
            feedbackL = wet * params.feedback;
            
            float mix = dry + wet * params.mix;
            outputDataL[sample] = mix * params.gain;
            
        }
        
        
    }


#if JUCE_DEBUG
    protectYourEars(buffer);
#endif 
}

juce::AudioProcessorParameter* DelayAudioProcessor::getBypassParameter() const {
    
    return params.bypassParam;
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData){
    copyXmlToBinary(*apvts.copyState().createXml(), destData);

}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes){
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())){
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
