#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/stat.h>

char dir[] = "/var/crawler";

int download(int id, char *file, char *url)
{
	char argv[3][100];
	sprintf(argv[0], "%s/bin/download", dir);
	sprintf(argv[1], "%s/output/", dir);
	sprintf(argv[2], "%s", url);

	pid_t pid = fork();
	if(pid == 0)
	{
		execv(argv[0], argv);
		exit(127);
	}
	else
	{
		waitpid(pid, 0, 0);
	}

	return 0;
}

int start_thread(int id, char *url)
{
	char output_dir[100];
	sprintf(output_dir, "%s/thread-%d", dir, id);
	printf("Output Directory: %s\n", output_dir);
	mkdir(output_dir, 777);

	char download_file[100];
	sprintf(download_file, "%s/page.html", output_dir);
	printf("Download File: %s\n", download_file);
	download(id, download_file, url);

	return 0;
}

int main()
{
	char url[] = "https://cboard.cprogramming.com/linux-programming/119903-execv-failed.html";
	printf("%s\n", url);
	start_thread(42, url);

	return 0;
}
