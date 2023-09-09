#pragma once
#include <atomic>
#include <mutex>
#include <vector>
#include <thread>

#include "audio_recorder.h"
#include "audio_sink.h"


// Visualizer class
// This class owns the SDL window and performs visual updates based on system audio
class AudioDisplayer : public AudioSink {
public:

    AudioDisplayer();
    ~AudioDisplayer();

    // The status of the initialization process
    // return: bool - whether the initialization was successful
    bool init_successful() const;

    // Update the visuals based on the most recent packet
    // return: bool - whether the update was successful
    bool update();

    // Copy a packet of data from the audio recorder
    // Must override the method from AudioSink
    // param: data - pointer to data values
    // param: channels - the number of audio channels
    // param: frames - the number of frames of data
    void copy_data(float* data, int channels, int frames) override;

private:

    AudioRecorder recorder;
    std::thread recording_thread;
    std::atomic_bool exit_recording_thread_flag = false;
    std::mutex packet_buffer_mutex;
    typedef std::vector<float> packet;
    packet packet_buffer; // Must use mutex to access


    void printValues();
};