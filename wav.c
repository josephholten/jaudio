#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PACKED __attribute__((packed))

struct PACKED riff_header_t {
  char chunk_id[4];
  int chunk_size;
  char format[4];
};

int main() {
  struct riff_header_t header = {
    .chunk_id = "RIFF",
    .chunk_size = 0, // fill in later
    .format = "WAVE",
  };

  int fd = open("1.wav", O_RDWR | O_CREAT, 00664);

  write(fd, &header, sizeof(header));

  close(fd);
}
