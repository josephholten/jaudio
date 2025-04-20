#include "wav.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define KiB 1024
#define MiB KiB * KiB

int main() {
  struct wav_header_t header;
  short num_channels = 1;
  int sample_rate = 16000; // in Hz
  short bits_per_sample = sizeof(char);
  short bytes_per_sample = bits_per_sample / sizeof(char);
  wav_header_pcm_init(&header, num_channels, sample_rate, bits_per_sample);

  int buffer_size = 10*MiB;
  struct wav_data_t* data = wav_data_alloc(buffer_size);

  int freq = 220; // in Hz
  int duration = 5; // in s

  int samples_per_wave = sample_rate / freq;
  int num_waves = duration * freq;
  int num_samples = num_waves * samples_per_wave;
  int num_bytes = num_samples * bytes_per_sample;

  assert(num_bytes < buffer_size && "duration exeeds buffer size");

  char* buf = data->data;
  for (int wave = 0; wave < num_waves; wave++) {
    for (int i = wave*samples_per_wave; i < wave*samples_per_wave + samples_per_wave/2; i++) {
      buf[i] = 100;
    }
    for (int i = wave*samples_per_wave + samples_per_wave/2; i < (wave+1)*samples_per_wave; i++) {
      buf[i] = -100;
    }
  }

  int fd = open("1.wav", O_RDWR | O_CREAT, 00664);
  write(fd, &header, sizeof(header));
  write(fd, data, sizeof(data) + buffer_size);
  close(fd);
}
