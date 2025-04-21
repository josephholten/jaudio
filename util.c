#include "util.h"
#include <unistd.h>
#include <stdio.h>

ssize_t rread(int fd, void* buf, size_t count, size_t max_tries) {
  size_t tries = 0;
  size_t bytes_read = 0;

  while (bytes_read < count && tries < max_tries) {
    ssize_t ret = read(fd,buf+bytes_read,count-bytes_read);
    // error or end of file
    if (ret <= 0) {
      return ret;
    }
    bytes_read += ret;
    tries++;
  }

  if (bytes_read == count) {
    return count;
  }

  return -1;
}

void gnuplot(const char* path, double* x, double* y, size_t size) {
  FILE* file;
  file = fopen(path, "w");
  for (size_t i = 0; i < size; i++)
    fprintf(file,"%e %e\n", x[i], y[i]);
  fclose(file);
}

