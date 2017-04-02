CC=gcc

default: build

build:
	gcc -o bin/crawler lib/crawler.c && \
	gcc -o bin/download lib/download.c -lcurl && \
	gcc -o bin/verify-article lib/verify-article.c && \
	gcc -o bin/url-extract lib/url-extract.c && \
	gcc -o bin/content-extract lib/content-extract.c

install:
	make build && \
	mkdir -p /var/crawler && \
	mkdir -p /var/crawler/bin && \
	cp bin/* /var/crawler/bin

clean:
	rm -f *.o

run:
	bin/crawler

create-db:
	gcc -o create-database.o core/create-database.c -I./mysql/include -L/usr/local/mysql/lib -lmysqlclient -lz && \
	./create-database.o

download:
	gcc -o download.o core/download.c -I./mysql/include -L/usr/local/mysql/lib -lmysqlclient -lcurl -lz && \
	./download.o
