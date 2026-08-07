#ifndef NETWORK_H
#define NETWORK_H

#include "tools.h"
#include "file_io.h"

void network_init();
void network_cleanup();

bool download_file(const char* url, const char* output_path);

#endif

