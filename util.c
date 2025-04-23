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

int gnuplot(const char* path, double* x, double* y, size_t size) {
  FILE* file;
  char buf[4096];

  sprintf(buf,"%s.data",path);
  file = fopen(buf, "w");
  if (file == NULL)
    goto error;
  for (size_t i = 0; i < size; i++)
    fprintf(file,"%e %e\n", x[i], y[i]);
  fclose(file);

  sprintf(buf,"%s.gp",path);
  file = fopen(buf,"w");
  if (file == NULL)
    goto error;
  fprintf(file,"plot '%s.data' with lines\n", path);
  fclose(file);

  return 0;

error:
  fprintf(stderr,"ERROR: could not open file '%s' for writing\n", buf);
  perror(NULL);
  return -1;
}

void prin2(const char* msg, const double* data, size_t size) {
  printf("%s\n",msg);
  for (size_t i = 0; i < size; i++) {
    printf("  %.5e\n",data[i]);
  }
}
