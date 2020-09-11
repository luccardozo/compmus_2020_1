
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

#include "../read_write_wav.h"

using namespace std;

//helper functions
float midi2freq(int m)
{
    return 440*pow(2,(m-69.0f)/12.0f);
}

float freq2midi(float freq)
{
    return 12*log2( freq/440) + 69;
}
//


class SoundProcessor
{
    public:

        virtual void process(float*, int)
        {
        }
};


class SineOscillator : public SoundProcessor
{
    public:

        float freq;
        float amp;
        float Fs;


        SineOscillator(int nota_midi, float _amp, float _Fs)
        {
            freq = midi2freq(nota_midi);
            amp = _amp;
            Fs = _Fs;
        }

        ~SineOscillator()
        {
        }

        void process(float* audio_buffer, int buffer_len)
        {
            cout << "sine class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;
            for (int n=0; n<buffer_len; n++)
            {
                //audio_buffer[n] += amp * sin(2*M_PI*freq*(((float)n)/Fs));
                audio_buffer[n] = amp * sin(2*M_PI*freq*(((float)n)/Fs));
            }
        }

};

class MusicNote
{
    public:
        float start_time; // pos in seconds
        float end_time; // pos in seconds
        SoundProcessor* sp;

    MusicNote(SoundProcessor *o, float s, float e):sp(o), start_time(s), end_time(e)
    {
    }
};



int main(int argc, char** argv)
{
    const float duration = 4; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote*> notes;

    //===============================
    SineOscillator* a = new SineOscillator(60+0, 0.5, Fs);
    SineOscillator* b = new SineOscillator(60+2, 0.5, Fs);
    SineOscillator* c = new SineOscillator(60+4, 0.5, Fs);


    float curTime = 0.5;
    notes.push_back(new MusicNote(a, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(b, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(c, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(c, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(b, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(a, curTime, curTime+0.45f)); curTime += 0.5f;
    curTime += 0.40f;
    notes.push_back(new MusicNote(a, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(c, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(b, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(a, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(c, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(b, curTime, curTime+0.45f)); curTime += 0.5f;
    notes.push_back(new MusicNote(b, curTime, curTime+0.45f)); curTime += 0.5f;

    //===============================
    // get max signal duration
    int max_pos = 0;
    for (int k=0; k<notes.size(); k++)
    {
        max_pos = std::max((float)max_pos, (float)round(notes[k]->end_time*Fs));
    }
    audio_buffer = new float[max_pos];
    memset(audio_buffer, 0, max_pos);

    cout << "maxPos: " << max_pos <<  endl;
    // write the notes into the audio buffer
    for (int k=0; k<notes.size(); k++)
    {
        int startPos = notes[k]->start_time*Fs;
        int endPos = notes[k]->end_time*Fs;
        cout << "startPos: " << startPos << endl;
        cout << "endPos: " << endPos << endl;
        cout << "opa" << endl;
        notes[k]->sp->process(audio_buffer + startPos, endPos-startPos);
    }

    // ============================
    // save output wave
    string wav_name = "lucas_song.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, max_pos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;
    return 0;
}



