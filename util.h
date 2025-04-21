#pragma once
#include <stddef.h>
#include <sys/types.h>

ssize_t rread(int fd, void* buf, size_t count, size_t max_tries);
int gnuplot(const char* path, double* x, double* y, size_t size);
