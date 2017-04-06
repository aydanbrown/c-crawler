CC=gcc

default: build

build:
	gcc -o /crawler/lib/download lib/download.c -I./mysql/include -L/usr/local/mysql/lib -lmysqlclient -lcurl -lz && \
	gcc -o /crawler/lib/process lib/process.c && \
	gcc -o /crawler/lib/verify-article lib/verify-article.c && \
	gcc -o /crawler/lib/url-extract lib/url-extract.c && \
	gcc -o /crawler/lib/content-extract lib/content-extract.c

install:
	mkdir /crawler && \
	chmod 755 /crawler && \
	mkdir /crawler/lib && \
	mkdir /crawler/downloads && \
	mkdir /crawler/process && \
	make build

create-db:
	gcc -o create-database.o lib/create-database.c -I./mysql/include -L/usr/local/mysql/lib -lmysqlclient -lz && \
	./create-database.o

