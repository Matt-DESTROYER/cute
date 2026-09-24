#include <file-io.h>

#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if __WINDOWS
	#include <windows.h>
#else
	#include <spawn.h>
	#include <sys/wait.h>

	extern char** environ;
#endif

char* format(const char* template_string, ...) {
	if (template_string == NULL)
		return NULL;

	va_list args;

	va_start(args, template_string);
	int length = vsnprintf(NULL, 0, template_string, args);
	va_end(args);

	if (length < 1)
		return NULL;

	char* buffer = (char*)malloc(sizeof(char) * (length + 1));
	if (buffer == NULL)
		return NULL;

	va_start(args, template_string);
	int written = vsnprintf(buffer, length + 1, template_string, args);
	va_end(args);

	if (written != length) {
		free(buffer);
		return NULL;
	}

	return buffer;
}

// (End not inclusive)
char* bounded_strdup(const char* string, size_t start, size_t end) {
	if (string == NULL || start == end)
		return NULL;

	int reverse = 0;
	if (start > end) {
		reverse = 1;

		size_t temp = start;
		start = end;
		end = temp;
	}

	size_t new_length = end - start;
	char* new_string = malloc(sizeof(char) * (new_length + 1));
	if (new_string == NULL)
		return NULL;

	memcpy(new_string, string + start, sizeof(char) * new_length);
	new_string[new_length] = '\0';

	if (reverse == 1) {
		for (size_t i = 0; i < new_length / 2; i++) {
			size_t end_idx = new_length - i - 1;
			char temp = new_string[i];
			new_string[i] = new_string[end_idx];
			new_string[end_idx] = temp;
		}
	}

	return new_string;
}

void flatten_directory_name(char* buffer) {
	size_t i = 0;
	while (buffer[i] != '\0') {
		if (buffer[i] == '/')
			buffer[i] = '_';

		i++;
	}
}

bool fetch_package(char* repo_url, char* package_name, char* location, char* version) {
	char* _package_name = bounded_strdup(package_name, 0, strlen(package_name));
	if (_package_name == NULL) {
		return false;
	}
	flatten_directory_name(_package_name);
	char* package_folder = format("%s/%s", location, _package_name);
	free(_package_name);
	if (package_folder == NULL)
		return false;

	if (directory_exists(package_folder)) {
		free(package_folder);
		return true;
	}

#if __WINDOWS
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char* command;
	if (version == NULL) {
		command = format("git clone --depth 1 -q \"%s\" \"%s\"", repo_url, package_folder);
	} else {
		command = format("git clone --depth 1 --branch v%s -q \"%s\" \"%s\"", version, repo_url, package_folder);
	}

	if (CreateProcessA(
		NULL, // parse application from command line args
		command, // command line args
		NULL, // process handle
		NULL, // thread handle
		FALSE, // set handle inheritance false
		0, // no creation flags
		NULL, // use parent's environment block
		NULL, // use parent's starting directory
		&si,
		&pi
	)) {
		free(command);

		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD exit_code;
		if (GetExitCodeProcess(pi.hProcess, &exit_code)) {
			exit_code = (int)exit_code;
			
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			return exit_code == 0;
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else {
		free(command);
		return 1;
	}
#else
	pid_t pid;
	int status;

	if (version == NULL) {
		char* argv[] = {
			"git", "clone", "--depth", "1", "-q",
			repo_url, package_folder, NULL
		};

		if (posix_spawnp(&pid, "git", NULL, NULL, argv, environ) == 0) {
			waitpid(pid, &status, 0);
			if (WIFEXITED(status)) {
				return WEXITSTATUS(status) == 0;
			}
		}
	} else {
		char* branch = format("v%s", version);

		char* argv[] = {
			"git", "clone", "--depth", "1", "--branch", branch, "-q",
			repo_url, package_folder, NULL
		};

		if (posix_spawnp(&pid, "git", NULL, NULL, argv, environ) == 0) {
			waitpid(pid, &status, 0);
			if (WIFEXITED(status)) {
				return WEXITSTATUS(status) == 0;
			}
		}
	}
#endif

	free(package_folder);

	return 1;
}
