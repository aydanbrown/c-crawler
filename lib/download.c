#include <stdio.h>
#include <unistd.h>
#include <mysql.h>
#include <curl/curl.h>

MYSQL *con;

unsigned int batch_count = 0;
unsigned int download_count = 0;

void end_with_err()
{
	printf("%s\n", mysql_error(con));

	mysql_close(con);

	exit(1);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}

int download(char *filename, char *url)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	curl = curl_easy_init();

	if(curl)
	{
		fp = fopen(filename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		return 0;
	}

	return 1;
}

void start_database()
{
	con = mysql_init(NULL);

	if(con == NULL) { end_with_err(); }

	if(mysql_real_connect(con, "localhost", "root", "bAKd92@jkd)@JUd88@&f(@n", "crawler", 0, NULL, 0) == NULL) { end_with_err(); }
}

void download_batch()
{
	if(mysql_query(con, "SELECT * FROM urls WHERE status = 0 LIMIT 10")) { end_with_err(); }

	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL) { end_with_err(); }

	MYSQL_ROW row;

	while((row = mysql_fetch_row(result)))
	{
		char *id = row[0];
		char *url = row[1];
		
		download_count++;

		printf("Download %d: %s - %s\n", download_count, id, url);
		
		char filename[50];
		sprintf(filename, "downloads/page-%s.html", id);

		if(download(filename, url))
		{
			printf("Failed to download file\n");
		}
		else
		{
			printf("File downloaded\n");
			char update_status_query[50];
			sprintf(update_status_query, "UPDATE urls SET status = 1 WHERE id=%s", id);
			mysql_query(con, update_status_query);
		}
	}
}

// TODO: Load mysql password from file

int main(void)
{
	start_database();

	while(1)
	{
		batch_count++;
		printf("Starting Batch %d\n", batch_count);
		int start_download_count = download_count;
		download_batch();
		printf("Finished Batch %d\n", batch_count);
		if(start_download_count == download_count)
		{
			printf("Nothing was downloaded, delaying next batch...\n");
			sleep(5);
		}
	}
}
