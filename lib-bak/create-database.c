#include <stdio.h>
#include <mysql.h>

MYSQL *con;

#define num_of_queries 8

char *queries[num_of_queries] = {

"DROP DATABASE IF EXISTS crawler",

"CREATE DATABASE crawler",

"USE crawler",

"CREATE TABLE urls (\
id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,\
url VARCHAR(500) NOT NULL,\
status TINYINT UNSIGNED NOT NULL DEFAULT 0,\
retrieved TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)",

"CREATE TABLE content(\
id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,\
url VARCHAR(500) NOT NULL,\
title VARCHAR(200),\
h1 VARCHAR(500),\
h2 VARCHAR(500),\
h3 VARCHAR(500),\
h4 VARCHAR(500),\
h5 VARCHAR(500),\
h6 VARCHAR(500),\
p VARCHAR(10000),\
retrieved TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,\
FULLTEXT(title),\
FULLTEXT(h1),\
FULLTEXT(h2),\
FULLTEXT(h3),\
FULLTEXT(h4),\
FULLTEXT(h5),\
FULLTEXT(h6),\
FULLTEXT(p),\
FULLTEXT(title, h1, h2, h3, h4, h5, h6, p))",

"INSERT INTO urls (url) VALUES (\"https://www.theguardian.com/international\")",

"INSERT INTO urls (url) VALUES (\"https://dev.mysql.com/doc/refman/5.7/en/numeric-type-overview.html\")",

"INSERT INTO urls (url) VALUES (\"https://github.com/\")"

};

void end_with_err()
{
	printf("%s\n", mysql_error(con));

	mysql_close(con);

	exit(1);
}

int main(void)
{
	con = mysql_init(NULL);

	printf("Connecting...\n");

	if(mysql_real_connect(con, "localhost", "root", "bAKd92@jkd)@JUd88@&f(@n", NULL, 0, NULL, 0) == NULL)
	{
		end_with_err();
	}
	
	printf("Starting Queries...\n");

	for(int q = 0; q < num_of_queries; q++)
	{
		printf("QUERY: %s\n", queries[q]);
		if(mysql_query(con, queries[q]))
		{
			end_with_err();
		}
	}

	mysql_close(con);

	return 0;
}
