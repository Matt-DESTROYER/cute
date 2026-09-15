#include <stdlib.h>
#include <string.h>
// cute add Matt-DESTROYER/file-io
#include <file-io.h>

const char* hello_world = "Hello, world!\n";

int main(int argc, char* argv[]) {
	file_t file = file_open("test.txt", WRITE);
	if (file != NULL)
		file_write(file, hello_world, strlen(hello_world));
	file_close(file);

	return EXIT_SUCCESS;
}
