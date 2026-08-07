#include "network.h"
#include "file_io.h"

#include <stdbool.h>
#include <curl/curl.h>

void network_init() {
	curl_global_init(CURL_GLOBAL_DEFAULT);
}
void network_cleanup() {
	curl_global_cleanup();
}

bool download_file(const char* url, const char* output_path) {
	CURL* curl;
	CURLcode res;

	file_t output_file = file_open(output_path, WRITE_BINARY);
	if (output_file == NULL)
		return false;

	curl = curl_easy_init();
	if (!curl) {
		file_close(output_file);
		return false;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);

	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		file_close(output_file);
		return false;
	}

	curl_easy_cleanup(curl);

	file_close(output_file);

	return true;
}

