#include "controllertts.h"


ControllerTts::ControllerTts(QObject *parent)
    : QObject{parent}
{

}



ControllerTts *ControllerTts::instance(QObject *parent)
{
    static ControllerTts _instance(parent);
    return &_instance;
}

void ControllerTts::init(){
    synth = piper_create(
        "/home/pi/Downloads/en_US-lessac-medium.onnx",
        "/home/pi/Downloads/en_US-lessac-medium.onnx.json",
        "/home/pi/Desktop/QtProjects/piper1-gpl/libpiper/install/espeak-ng-data"
        );
}

void ControllerTts::release(){
    piper_free(synth);
}

void ControllerTts::speak(const std::string &textToSpeak){
    std::string audoFile = "output.raw";
    std::ofstream audio_stream(audoFile, std::ios::binary);

    piper_synthesize_options options = piper_default_synthesize_options(synth);
    // Change options here:
    // options.length_scale = 2;
    // options.speaker_id = 5;

    piper_synthesize_start(synth, textToSpeak.c_str(),
                           &options /* NULL for defaults */);

    piper_audio_chunk chunk;
    while (piper_synthesize_next(synth, &chunk) != PIPER_DONE) {
        audio_stream.write(reinterpret_cast<const char *>(chunk.samples),
                           chunk.num_samples * sizeof(float));
    }


    playRawFile(audoFile);
}

bool ControllerTts::playRawFile(const std::string& filename) {
    const char* device = "default";
    snd_pcm_t* handle;

    // 1. Open PCM device for playback
    if (snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        std::cerr << "Error: Could not open playback device." << std::endl;
        return false;
    }

    // 2. Configure Hardware Parameters
    snd_pcm_hw_params_t* params;
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_FLOAT_LE);
    snd_pcm_hw_params_set_channels(handle, params, 1);

    unsigned int rate = 22050;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);

    if (snd_pcm_hw_params(handle, params) < 0) {
        std::cerr << "Error: Could not set hardware parameters." << std::endl;
        snd_pcm_close(handle);
        return false;
    }

    // 3. Open the raw file
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        snd_pcm_close(handle);
        return false;
    }

    // 4. Playback Loop
    // Use a buffer (e.g., 1024 frames) to stream the file
    const int bufferSize = 1024;
    std::vector<float> buffer(bufferSize);

    while (file.read(reinterpret_cast<char*>(buffer.data()), bufferSize * sizeof(float))) {
        int framesWritten = snd_pcm_writei(handle, buffer.data(), bufferSize);

        // Handle underruns (where the hardware buffer goes empty)
        if (framesWritten == -EPIPE) {
            snd_pcm_prepare(handle);
        } else if (framesWritten < 0) {
            std::cerr << "Error: Write failed." << std::endl;
            break;
        }
    }

    // 5. Cleanup
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    file.close();
    return true;
}
