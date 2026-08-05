#include "file_io.h"

#if __WINDOWS
	#include <windows.h>
#else
	#include <sys/stat.h>
#endif

bool file_exists(const char* file) {
#if __WINDOWS
	DWORD attributes = GetFileAttributesA(file);
	return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
	struct stat file_stat;
	if (stat(file, &file_stat) != 0)
		return false;
	return S_ISREG(file_stat.st_mode);
#endif
}

bool directory_exists(const char* directory) {
#if __WINDOWS
	DWORD attributes = GetFileAttributesA(directory);
	return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
	struct stat dir_stat;
	if (stat(directory, &dir_stat) != 0)
		return false;
	return S_ISDIR(dir_stat.st_mode);
#endif
}

bool path_exists(const char* path) {
#if __WINDOWS
	DWORD attributes = GetFileAttributesA(path);
	return attributes != INVALID_FILE_ATTRIBUTES;
#else
	struct stat path_stat;
	return stat(path, &path_stat) == 0;
#endif
}

file_t file_open(const char* file, FileOptions_t options) {
#if __WINDOWS
	file_t fp = NULL;
	errno_t err = 1;

	switch (options) {
		case READ:
			err = fopen_s(&fp, file, "r");
			break;
		case READ_BINARY:
			err = fopen_s(&fp, file, "rb");
			break;
		case READ_WRITE:
			err = fopen_s(&fp, file, "r+");
			break;
		case READ_WRITE_BINARY:
			err = fopen_s(&fp, file, "r+b");
			break;
		case WRITE:
			err = fopen_s(&fp, file, "w");
			break;
		case WRITE_BINARY:
			err = fopen_s(&fp, file, "wb");
			break;
		case WRITE_READ:
			err = fopen_s(&fp, file, "w+");
			break;
		case WRITE_READ_BINARY:
			err = fopen_s(&fp, file, "w+b");
			break;
		case APPEND:
			err = fopen_s(&fp, file, "a");
			break;
		case APPEND_BINARY:
			err = fopen_s(&fp, file, "ab");
			break;
		case APPEND_READ:
			err = fopen_s(&fp, file, "a+");
			break;
		case APPEND_READ_BINARY:
			err = fopen_s(&fp, file, "a+b");
			break;
		default:
			return NULL;
	}
	if (err == 0)
		return fp;
	return NULL;
#else
	switch (options) {
		case READ:
			return fopen(file, "r");
		case READ_BINARY:
			return fopen(file, "rb");
		case READ_WRITE:
			return fopen(file, "r+");
		case READ_WRITE_BINARY:
			return fopen(file, "r+b");
		case WRITE:
			return fopen(file, "w");
		case WRITE_BINARY:
			return fopen(file, "wb");
		case WRITE_READ:
			return fopen(file, "w+");
		case WRITE_READ_BINARY:
			return fopen(file, "w+b");
		case APPEND:
			return fopen(file, "a");
		case APPEND_BINARY:
			return fopen(file, "ab");
		case APPEND_READ:
			return fopen(file, "a+");
		case APPEND_READ_BINARY:
			return fopen(file, "a+b");
		default:
			return NULL;
	}
#endif
}

void file_close(file_t file) {
	fclose(file);
}

size_t file_read(file_t file, char** out) {
	if (file == NULL)
		return 0;

#if __WINDOWS
	_fseeki64(file, 0, SEEK_END);
	long long int _file_size = _ftelli64(file);
#else
	fseeko(file, 0, SEEK_END);
	off_t _file_size = ftello(file);
#endif
	rewind(file);
	if (_file_size < 1)
		return 0;

	size_t file_size = (size_t)_file_size;

	char* buffer = malloc(sizeof(char) * (file_size + 1));
	if (buffer == NULL)
		return 0;

	size_t read_size = fread(buffer, sizeof(char), file_size, file);

	buffer[read_size] = '\0';
	if (read_size < file_size) {
		char* new_buffer = realloc(buffer, sizeof(char) * (read_size + 1));
		if (new_buffer != NULL) {
			buffer = new_buffer;
		}
	}

	*out = buffer;

	return read_size;
}

void file_write(file_t file, const char* buffer, size_t size) {
	if (file == NULL)
		return;

	fwrite(buffer, sizeof(char), size, file);
}

