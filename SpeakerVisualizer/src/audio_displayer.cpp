#pragma once
#include "audio_displayer.h"
#include <iostream>
#include <thread>
#include <vector>


AudioDisplayer::AudioDisplayer() : recorder()
{
    /*Initializes recorder via ': recorder()' and checks if the initialization was successful.
    * If the recorder initialized successfully, start a new thread from the record function.
    */
    if (recorder.init_successful()) {
        recording_thread = std::thread(&AudioRecorder::record, &recorder, (AudioSink*)this, std::ref(exit_recording_thread_flag));
    }
}

AudioDisplayer::~AudioDisplayer()
{
    // Stop recording thread before implicitly destroying AudioRecorder
    if (recording_thread.joinable()) {
        exit_recording_thread_flag = true;
        recording_thread.join();
    }
}

bool AudioDisplayer::init_successful() const {
    return (recording_thread.joinable());
}

bool AudioDisplayer::update()
{
    printValues();
    return true;
}


void AudioDisplayer::copy_data(float* data, int channels, int frames) {
    std::lock_guard<std::mutex>read_guard(packet_buffer_mutex);

    if (data) {
        packet_buffer = packet(data, data + channels * frames);
    }
    else {
        // use an empty vector if there is no data (silence)
        packet_buffer = packet();
    }
}


static inline float maxfl(float a, float b) {
    return a > b ? a : b;
}

static inline float absfl(float a)
{
    return a > 0 ? a : -a;
}

// Function to create a volume bar based on a float value from 0 to 1
std::string createVolumeBar(float value, int barWidth = 40) {
    // Ensure value is within [0, 1] range

    int barLength = static_cast<int>(barWidth * value);
    int emptyLength = barWidth - barLength;

    // Create the volume bar
    std::string bar;
    bar += "[";
    for (int i = 0; i < barLength; ++i) {
        bar += "#";
    }
    for (int i = 0; i < emptyLength; ++i) {
        bar += " ";
    }
    bar += "]";

    return bar;
}

void AudioDisplayer::printValues() {

    std::unique_lock<std::mutex>read_guard(packet_buffer_mutex);
    if (!packet_buffer.empty()) {
        int dispSize = 1000;
        
        float vol_left = 0;

        auto amplitude = packet_buffer.begin();
        
        //Iterate through the entire buffer
        //Find the highest value in the buffer for both the left and right channel
        for (unsigned long i = 0; i < packet_buffer.size(); i += 2)
        {
            vol_left = maxfl(vol_left, absfl(*amplitude));
            amplitude++;
        }

        // Clear the console


        // Create and display the volume bar
        std::string volumeBar = createVolumeBar(vol_left);
        std::cout << "Volume: " << volumeBar << " " << vol_left << std::endl;
    }
    else { // silence

    }
    read_guard.unlock();    
}