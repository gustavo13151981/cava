#pragma once
#include <stdint.h>

// the size of the smallest input buffer (treble). this will determin the size of all other buffers.
// matches the BUFFER_SIZE in input/common.h
#define BUFFER_SIZE 1024

#include <fftw3.h>

struct cava_plan {
    int FFTbassbufferSize;
    int FFTmidbufferSize;
    int FFTtreblebufferSize;
    int number_of_bars;
    int audio_channels;
    int input_buffer_size;
    int rate;
    int bass_cut_off_bar;
    int treble_cut_off_bar;
    int height;
    int sens_init;
    // 11 int

    double sens;
    double g;
    // 2 dobule

    fftw_plan p_bass_l, p_bass_r;
    fftw_plan p_mid_l, p_mid_r;
    fftw_plan p_treble_l, p_treble_r;
    // 6 fftw_plan

    fftw_complex *out_bass_l, *out_bass_r;
    fftw_complex *out_mid_l, *out_mid_r;
    fftw_complex *out_treble_l, *out_treble_r;
    // 6 fftw_complex *

    double *bass_multiplier;
    double *mid_multiplier;
    double *treble_multiplier;

    double *in_bass_r_raw, *in_bass_l_raw;
    double *in_mid_r_raw, *in_mid_l_raw;
    double *in_treble_r_raw, *in_treble_l_raw;
    double *in_bass_r, *in_bass_l;
    double *in_mid_r, *in_mid_l;
    double *in_treble_r, *in_treble_l;
    double *prev_cava_out, *cava_mem;
    double *input_buffer, *cava_peak;

    double *eq;
    // 20 double *

    float *cut_off_frequency;
    // 1 float *
    int *FFTbuffer_lower_cut_off;
    int *FFTbuffer_upper_cut_off;
    int *cava_fall;
    // 3 int *
};
// cava_in size must be  4096 * number of channels and cava_out will be 1024 * number of channels
// number_of_bars is number of wanted bars per channel
// cava_execute assumes cava_in samples to be interleaved if more than one channel
// cava_in does not need to be filled up per execution
// new_samples is the number of samples in cava_in
// it is recomended to execute about every 20ms (or per 1024 * number of channels samples at 44100
// ramples rate) but depending on how data is read you might have a bit more data per execution some
// times framerate is your expected execution rate of cava_execute per second
extern struct cava_plan *cava_init(int number_of_bars, unsigned int rate, int channels,
                                   int max_height, int framerate);
extern void cava_execute(double *cava_in, int new_samples, double *cava_out, struct cava_plan *p);
extern void cava_destroy(struct cava_plan *p);