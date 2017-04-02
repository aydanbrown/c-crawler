#include <stdio.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Usage: %s [filename] [url]\n", argv[0]);
		return 1;
	}
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char *url = argv[2];
	char *outfilename = argv[1];
	curl = curl_easy_init();

	if (curl)
	{
		fp = fopen(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}

	return 0;
}
