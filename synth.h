#pragma once

#include "wav.h"

short discretize(char* buf, double amplitude, short bits_per_sample);
double square_wave(double t, double freq);
double sin_wave(double t, double freq);
typedef double (*wave_fptr)(double,double);
void sample_wave(wave_fptr wave, double freq, struct wav_t* wav);

